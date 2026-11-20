# GetTimeseries

Construye consultas de Athena filtradas por dispositivo, variable y particiones temporales hasta el minuto; devuelve puntos de la serie histórica.

## Entrada y comportamiento

Petición GET con clientId, metric y minutes; también atiende OPTIONS para CORS.

Usa ATHENA_DATABASE, ATHENA_TABLE, ATHENA_WORKGROUP y ATHENA_OUTPUT. Limita el rango con MAX_QUERY_MINUTES y distingue zona horaria local y de particiones. ATHENA_OUTPUT debe configurarse; el código no dispone de un bucket de resultados por defecto.

## Archivo y entorno

- Fuente: `lambda_function.py`, recuperado de AWS sin cambios en sus bytes.
- Runtime exportado: `python3.12`.
- Handler: `lambda_function.lambda_handler`.
- Memoria: 256 MB. Timeout: 60 s.
- Versión: `$LATEST`. Última modificación informada por AWS: `2026-05-28T20:19:24.000+0000`.
- Variables de entorno cuyos nombres se recuperaron: `ALLOWED_ORIGIN`, `ATHENA_DATABASE`, `ATHENA_MAX_WAIT_SECONDS`, `ATHENA_OUTPUT`, `ATHENA_TABLE`, `ATHENA_WORKGROUP`, `ENABLE_DIAGNOSTIC_LOGS`, `MAX_QUERY_MINUTES`, `PARTITION_TZ_OFFSET_HOURS`, `POINTS_PER_MINUTE`.
- Los valores reales de esas variables no se incluyeron en el respaldo.
- No hay capas Lambda declaradas. El código importa boto3 y bibliotecas estándar de Python; no se capturó la versión exacta del SDK del runtime.

`configuracion.example.json` es una referencia para completar la configuración, no una receta automática ni una copia de sus valores efectivos. Las referencias originales de AWS están en trazabilidad/recuperacion_lambda_20261120/respaldo_aws.

## Parámetros que aparecen en el código

Los valores de respaldo son: base iot_db, tabla raw_processed, workgroup primary, rango máximo 180 minutos, espera de Athena 25 s, 10 puntos por minuto y zona local -5. PARTITION_TZ_OFFSET_HOURS toma la zona local si no se define. Estos son valores del código, no los valores de entorno recuperados, que se omitieron. ENABLE_DIAGNOSTIC_LOGS controla el registro de diagnóstico.

El código genera consultas de Athena; la revisión de este paquete fue estática y no ejecutó esas consultas.
