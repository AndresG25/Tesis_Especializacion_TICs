import os
import json
import decimal
import boto3
from datetime import datetime, timezone, timedelta

ddb = boto3.resource("dynamodb")

def _json_default(o):
    if isinstance(o, decimal.Decimal):
        # DynamoDB usa Decimal; lo convertimos a float/int
        if o % 1 == 0:
            return int(o)
        return float(o)
    return str(o)

def _get_allowed_origin(event):
    # Origen que manda el navegador
    headers = event.get("headers") or {}
    origin = headers.get("origin") or headers.get("Origin")  # por si viene con mayúscula

    # Puedes poner "*" o lista separada por comas
    allow_env = (os.environ.get("ALLOWED_ORIGINS") or os.environ.get("ALLOWED_ORIGIN") or "*").strip()

    if allow_env == "*":
        return "*"  # prototipo abierto

    allowed = [x.strip() for x in allow_env.split(",") if x.strip()]
    if not origin:
        # curl sin Origin
        return allowed[0] if allowed else "*"

    # Si coincide exacto, devolvemos ese (lo que necesita el browser)
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
        "body": json.dumps(payload, default=_json_default, ensure_ascii=False),
    }

def lambda_handler(event, context):
    # 1) Preflight CORS
    method = (event.get("requestContext", {}).get("http", {}).get("method")
              or event.get("httpMethod")
              or "GET")

    if method.upper() == "OPTIONS":
        return {
            "statusCode": 204,
            "headers": _cors_headers(event),
            "body": ""
        }

    try:
        # 2) Config tabla (OJO: aquí fue donde te “contaminaste” con cloudfront)
        table_name = (os.environ.get("DDB_TABLE_NAME")
                      or os.environ.get("TABLE_NAME")
                      or "turbina_latest").strip()

        if not table_name or "http" in table_name or "cloudfront" in table_name:
            return _response(event, 500, {
                "ok": False,
                "error": "CONFIG_ERROR",
                "message": f"TABLE_NAME inválida: '{table_name}'. Debe ser algo como 'turbina_latest'."
            })

        table = ddb.Table(table_name)

        # 3) Params
        qs = event.get("queryStringParameters") or {}
        client_id = (qs.get("clientId") or qs.get("clientid") or "").strip()
        if not client_id:
            return _response(event, 400, {"ok": False, "error": "MISSING_PARAM", "message": "Falta clientId"})

        # 4) Dynamo GetItem
        resp = table.get_item(Key={"clientId": client_id})
        item = resp.get("Item")
        if not item:
            return _response(event, 404, {"ok": False, "error": "NOT_FOUND", "message": f"No existe clientId={client_id}"})

        # 5) Respuesta
        item["ok"] = True
        item["clientId"] = client_id

        return _response(event, 200, item)

    except Exception as e:
        # IMPORTANTE: aun en error devolvemos CORS
        return _response(event, 500, {"ok": False, "error": "INTERNAL", "message": str(e)})