import os
import json
import time
import boto3
from datetime import datetime, timezone, timedelta
from collections import defaultdict

athena = boto3.client("athena")

METRICS = {
    "temperature": {"name": "Temperatura", "unit": "°C"},
    "humidity": {"name": "Humedad", "unit": "%"},
    "pressure": {"name": "Presión", "unit": "hPa"},
    "altitude": {"name": "Altitud", "unit": "m"},
    "wind_speed": {"name": "Velocidad del viento", "unit": "m/s"},
    "rpm": {"name": "RPM", "unit": "rpm"},
    "voltage": {"name": "Voltaje", "unit": "V"},
    "memory_usage": {"name": "Memoria", "unit": "%"},
}


def _env_bool(name, default=False):
    value = os.environ.get(name)
    if value is None:
        return default
    return str(value).strip().lower() in ("1", "true", "yes", "y", "on")


def _log_event(name, payload):
    """
    Log estructurado para CloudWatch.
    Nos permitirá auditar cuántas particiones se consultaron,
    qué ventana pidió el dashboard y qué queryId generó Athena.
    """
    if not _env_bool("ENABLE_DIAGNOSTIC_LOGS", True):
        return

    safe_payload = {"event": name}
    safe_payload.update(payload or {})
    print(json.dumps(safe_payload, ensure_ascii=False, default=str))


def _sql_literal(value):
    """
    Escapa comillas simples para reducir riesgo de romper el SQL.
    El parámetro metric no se escapa aquí porque se valida contra METRICS.
    """
    return str(value).replace("'", "''")


def _get_allowed_origin(event):
    headers = event.get("headers") or {}
    origin = headers.get("origin") or headers.get("Origin")
    allow_env = (os.environ.get("ALLOWED_ORIGINS") or os.environ.get("ALLOWED_ORIGIN") or "*").strip()

    if allow_env == "*":
        return "*"

    allowed = [x.strip() for x in allow_env.split(",") if x.strip()]
    if not origin:
        return allowed[0] if allowed else "*"

    return origin if origin in allowed else (allowed[0] if allowed else "*")


def _cors_headers(event):
    allow_origin = _get_allowed_origin(event)
    return {
        "Access-Control-Allow-Origin": allow_origin,
        "Access-Control-Allow-Methods": "GET,OPTIONS",
        "Access-Control-Allow-Headers": "content-type",
        "Access-Control-Max-Age": "86400",
        "Content-Type": "application/json",
    }


def _response(event, status, payload):
    return {
        "statusCode": status,
        "headers": _cors_headers(event),
        "body": json.dumps(payload, ensure_ascii=False),
    }


def _parse_ts_local(ts_str):
    """
    Formato típico actual del campo timestamp:
    "15/05/26 17:04:38"
    """
    for fmt in ("%d/%m/%y %H:%M:%S", "%d/%m/%y %H:%M"):
        try:
            return datetime.strptime(ts_str, fmt)
        except Exception:
            pass
    return None


def _minute_buckets_between(start_dt, end_dt):
    """
    Construye las particiones exactas por:
    year, month, day, hour, minute.

    La tabla Athena está proyectada hasta minute. Por eso,
    si no filtramos hour/minute, Athena puede proyectar muchos
    prefijos S3 innecesarios. Esta función reduce el universo
    de prefijos al rango real solicitado.
    """
    start_minute = start_dt.replace(second=0, microsecond=0)
    end_minute = end_dt.replace(second=0, microsecond=0)

    buckets = defaultdict(set)
    cur = start_minute

    while cur <= end_minute:
        key = (
            f"{cur:%Y}",
            f"{cur:%m}",
            f"{cur:%d}",
            f"{cur:%H}",
        )
        buckets[key].add(f"{cur:%M}")
        cur = cur + timedelta(minutes=1)

    return buckets


def _build_partition_where(start_partition_dt, end_partition_dt):
    """
    Devuelve un WHERE de particiones del tipo:

    (
      year='2026' AND month='05' AND day='18'
      AND hour='15'
      AND minute IN ('10','11','12')
    )
    OR
    (
      year='2026' AND month='05' AND day='18'
      AND hour='16'
      AND minute IN ('00','01')
    )

    Esto reemplaza el filtro anterior que solo llegaba hasta day.
    """
    buckets = _minute_buckets_between(start_partition_dt, end_partition_dt)

    clauses = []
    total_minute_partitions = 0

    for (year, month, day, hour), minutes_set in sorted(buckets.items()):
        minutes = sorted(minutes_set)
        total_minute_partitions += len(minutes)

        minutes_sql = ", ".join([f"'{m}'" for m in minutes])

        clauses.append(
            "("
            f"year='{year}' "
            f"AND month='{month}' "
            f"AND day='{day}' "
            f"AND hour='{hour}' "
            f"AND minute IN ({minutes_sql})"
            ")"
        )

    if not clauses:
        return "1=0", 0, 0

    return " OR ".join(clauses), len(clauses), total_minute_partitions


def _run_athena_query(sql, database, workgroup, output):
    """
    Ejecuta Athena, espera hasta ATHENA_MAX_WAIT_SECONDS y devuelve:
    - queryId
    - estado
    - filas o razón de error
    - metadatos de ejecución
    """
    q = athena.start_query_execution(
        QueryString=sql,
        QueryExecutionContext={"Database": database},
        ResultConfiguration={"OutputLocation": output},
        WorkGroup=workgroup,
    )

    qid = q["QueryExecutionId"]
    max_wait = int(os.environ.get("ATHENA_MAX_WAIT_SECONDS", "25"))

    t0 = time.time()
    last_execution = None

    while True:
        execution = athena.get_query_execution(QueryExecutionId=qid)["QueryExecution"]
        last_execution = execution
        state = execution["Status"]["State"]

        if state in ("SUCCEEDED", "FAILED", "CANCELLED"):
            break

        if time.time() - t0 > max_wait:
            try:
                athena.stop_query_execution(QueryExecutionId=qid)
            except Exception:
                pass

            metadata = {
                "queryId": qid,
                "state": "TIMEOUT",
                "elapsedSeconds": round(time.time() - t0, 3),
                "maxWaitSeconds": max_wait,
            }
            return qid, "TIMEOUT", None, metadata

        time.sleep(0.6)

    stats = (last_execution or {}).get("Statistics", {}) if last_execution else {}
    status = (last_execution or {}).get("Status", {}) if last_execution else {}

    metadata = {
        "queryId": qid,
        "state": state,
        "elapsedSeconds": round(time.time() - t0, 3),
        "dataScannedBytes": stats.get("DataScannedInBytes"),
        "engineExecutionTimeMs": stats.get("EngineExecutionTimeInMillis"),
        "totalExecutionTimeMs": stats.get("TotalExecutionTimeInMillis"),
        "queryQueueTimeMs": stats.get("QueryQueueTimeInMillis"),
        "serviceProcessingTimeMs": stats.get("ServiceProcessingTimeInMillis"),
    }

    if state != "SUCCEEDED":
        reason = status.get("StateChangeReason", "")
        return qid, state, reason, metadata

    rows = []
    token = None
    first = True

    while True:
        if token:
            resp = athena.get_query_results(QueryExecutionId=qid, NextToken=token)
        else:
            resp = athena.get_query_results(QueryExecutionId=qid)

        rset = resp["ResultSet"]["Rows"]

        if first and rset:
            rset = rset[1:]
            first = False

        for row in rset:
            data = row.get("Data", [])
            ts_str = data[0].get("VarCharValue") if len(data) > 0 and data[0] else None
            val_str = data[1].get("VarCharValue") if len(data) > 1 and data[1] else None
            rows.append((ts_str, val_str))

        token = resp.get("NextToken")
        if not token:
            break

    metadata["resultRows"] = len(rows)
    return qid, "SUCCEEDED", rows, metadata


def lambda_handler(event, context):
    method = (
        event.get("requestContext", {}).get("http", {}).get("method")
        or event.get("httpMethod")
        or "GET"
    )

    if method.upper() == "OPTIONS":
        return {"statusCode": 204, "headers": _cors_headers(event), "body": ""}

    try:
        database = (os.environ.get("ATHENA_DATABASE") or "iot_db").strip()
        table = (os.environ.get("ATHENA_TABLE") or "raw_processed").strip()
        workgroup = (os.environ.get("ATHENA_WORKGROUP") or "primary").strip()
        output = (os.environ.get("ATHENA_OUTPUT") or "").strip()

        if not output.startswith("s3://"):
            return _response(
                event,
                500,
                {
                    "ok": False,
                    "error": "CONFIG_ERROR",
                    "message": "ATHENA_OUTPUT debe ser s3://.../query-results/",
                },
            )

        local_tz_offset_hours = int(os.environ.get("TZ_OFFSET_HOURS", "-5"))
        local_tz = timezone(timedelta(hours=local_tz_offset_hours))

        # Importante:
        # Si los prefijos S3 fueron creados en hora Colombia, deja este valor en -5.
        # Si los prefijos S3 fueron creados con hora UTC, configura PARTITION_TZ_OFFSET_HOURS=0.
        partition_tz_offset_hours = int(
            os.environ.get("PARTITION_TZ_OFFSET_HOURS", str(local_tz_offset_hours))
        )
        partition_tz = timezone(timedelta(hours=partition_tz_offset_hours))

        qs = event.get("queryStringParameters") or {}

        client_id = (qs.get("clientId") or qs.get("clientid") or "").strip()
        metric = (qs.get("metric") or "").strip()

        try:
            minutes = int((qs.get("minutes") or "60").strip())
        except Exception:
            return _response(
                event,
                400,
                {
                    "ok": False,
                    "error": "BAD_MINUTES",
                    "message": "minutes debe ser un número entero positivo",
                },
            )

        max_query_minutes = int(os.environ.get("MAX_QUERY_MINUTES", "180"))

        if not client_id:
            return _response(
                event,
                400,
                {
                    "ok": False,
                    "error": "MISSING_PARAM",
                    "message": "Falta clientId",
                },
            )

        if metric not in METRICS:
            return _response(
                event,
                400,
                {
                    "ok": False,
                    "error": "BAD_METRIC",
                    "message": f"metric inválida. Usa: {', '.join(METRICS.keys())}",
                },
            )

        if minutes <= 0:
            return _response(
                event,
                400,
                {
                    "ok": False,
                    "error": "BAD_MINUTES",
                    "message": "minutes debe ser > 0",
                },
            )

        if minutes > max_query_minutes:
            return _response(
                event,
                400,
                {
                    "ok": False,
                    "error": "RANGE_TOO_LARGE",
                    "message": (
                        f"Por control de costos, minutes no puede superar "
                        f"{max_query_minutes}. Ajusta MAX_QUERY_MINUTES si necesitas "
                        f"consultas históricas más largas."
                    ),
                    "minutes": minutes,
                    "maxQueryMinutes": max_query_minutes,
                },
            )

        now_utc = datetime.now(timezone.utc)

        # Ventana local para interpretar el campo timestamp y entregar respuesta al dashboard.
        now_local = now_utc.astimezone(local_tz)
        from_local = now_local - timedelta(minutes=minutes)

        # Ventana de particiones para construir year/month/day/hour/minute.
        # Por defecto usa la misma zona horaria local. Si los prefijos están en UTC,
        # define PARTITION_TZ_OFFSET_HOURS=0.
        now_partition = now_utc.astimezone(partition_tz)
        from_partition = now_partition - timedelta(minutes=minutes)

        part_where, hour_partition_groups, minute_partitions = _build_partition_where(
            from_partition,
            now_partition,
        )

        ppm = int(os.environ.get("POINTS_PER_MINUTE", "10"))
        max_rows = min(50000, max(600, minutes * ppm))

        client_id_sql = _sql_literal(client_id)

        sql = f"""
        SELECT
          "timestamp" AS ts_str,
          CAST({metric} AS double) AS value
        FROM {database}.{table}
        WHERE clientid = '{client_id_sql}'
          AND ({part_where})
        ORDER BY "timestamp" DESC
        LIMIT {max_rows}
        """

        _log_event(
            "gettimeseries_request",
            {
                "clientId": client_id,
                "metric": metric,
                "minutes": minutes,
                "fromLocal": from_local.isoformat(),
                "toLocal": now_local.isoformat(),
                "fromPartition": from_partition.isoformat(),
                "toPartition": now_partition.isoformat(),
                "localTzOffsetHours": local_tz_offset_hours,
                "partitionTzOffsetHours": partition_tz_offset_hours,
                "hourPartitionGroups": hour_partition_groups,
                "minutePartitions": minute_partitions,
                "maxRows": max_rows,
            },
        )

        _log_event(
            "athena_sql_prepared",
            {
                "database": database,
                "table": table,
                "workgroup": workgroup,
                "output": output,
                "hourPartitionGroups": hour_partition_groups,
                "minutePartitions": minute_partitions,
                "metric": metric,
                "minutes": minutes,
            },
        )

        qid, state, payload, metadata = _run_athena_query(
            sql=sql,
            database=database,
            workgroup=workgroup,
            output=output,
        )

        _log_event(
            "athena_query_finished",
            {
                "queryId": qid,
                "state": state,
                "clientId": client_id,
                "metric": metric,
                "minutes": minutes,
                "hourPartitionGroups": hour_partition_groups,
                "minutePartitions": minute_partitions,
                "dataScannedBytes": metadata.get("dataScannedBytes"),
                "engineExecutionTimeMs": metadata.get("engineExecutionTimeMs"),
                "totalExecutionTimeMs": metadata.get("totalExecutionTimeMs"),
                "elapsedSeconds": metadata.get("elapsedSeconds"),
                "resultRows": metadata.get("resultRows"),
            },
        )

        if state == "TIMEOUT":
            return _response(
                event,
                504,
                {
                    "ok": False,
                    "error": "TIMEOUT",
                    "message": (
                        f"Athena query timeout > "
                        f"{os.environ.get('ATHENA_MAX_WAIT_SECONDS', '25')}s"
                    ),
                    "queryId": qid,
                    "clientId": client_id,
                    "metric": metric,
                    "minutes": minutes,
                    "hourPartitionGroups": hour_partition_groups,
                    "minutePartitions": minute_partitions,
                },
            )

        if state != "SUCCEEDED":
            return _response(
                event,
                500,
                {
                    "ok": False,
                    "error": "ATHENA_ERROR",
                    "message": str(payload),
                    "queryId": qid,
                    "clientId": client_id,
                    "metric": metric,
                    "minutes": minutes,
                },
            )

        rows = payload or []

        points = []

        for ts_str, val_str in rows:
            if not ts_str or val_str is None:
                continue

            dt_local = _parse_ts_local(ts_str)
            if not dt_local:
                continue

            dt_local = dt_local.replace(tzinfo=local_tz)

            if dt_local < from_local or dt_local > now_local:
                continue

            try:
                val = float(val_str)
            except Exception:
                continue

            points.append(
                {
                    "ts": dt_local.strftime("%Y-%m-%d %H:%M:%S"),
                    "tsEpoch": int(dt_local.timestamp()),
                    "value": val,
                }
            )

        points.sort(key=lambda x: x["tsEpoch"])

        return _response(
            event,
            200,
            {
                "ok": True,
                "queryId": qid,
                "clientId": client_id,
                "metric": metric,
                "metricName": METRICS[metric]["name"],
                "unit": METRICS[metric]["unit"],
                "minutes": minutes,
                "fromLocal": from_local.isoformat(),
                "toLocal": now_local.isoformat(),
                "hourPartitionGroups": hour_partition_groups,
                "minutePartitions": minute_partitions,
                "count": len(points),
                "points": points,
            },
        )

    except Exception as e:
        _log_event(
            "gettimeseries_internal_error",
            {
                "message": str(e),
            },
        )

        return _response(
            event,
            500,
            {
                "ok": False,
                "error": "INTERNAL",
                "message": str(e),
            },
        )