# ActualizarUltimoDatoDynamo

Recibe la telemetría entregada por una regla de AWS IoT Core, normaliza números y booleanos y guarda el último estado por clientId en DynamoDB.

## Entrada y comportamiento

Evento JSON de IoT con clientId, timestamp y variables de telemetría. El comentario del código documenta el tópico sensor/data.

Escribe mediante PutItem en la tabla indicada por TABLE_NAME; el valor de respaldo del código es turbina_latest.

## Archivo y entorno

- Fuente: `lambda_function.py`, recuperado de AWS sin cambios en sus bytes.
- Runtime exportado: `python3.12`.
- Handler: `lambda_function.lambda_handler`.
- Memoria: 128 MB. Timeout: 3 s.
- Versión: `$LATEST`. Última modificación informada por AWS: `2026-05-15T20:11:28.000+0000`.
- Variables de entorno cuyos nombres se recuperaron: `TABLE_NAME`.
- Los valores reales de esas variables no se incluyeron en el respaldo.
- No hay capas Lambda declaradas. El código importa boto3 y bibliotecas estándar de Python; no se capturó la versión exacta del SDK del runtime.

`configuracion.example.json` es una referencia para completar la configuración, no una receta automática ni una copia de sus valores efectivos. Las referencias originales de AWS están en trazabilidad/recuperacion_lambda_20261120/respaldo_aws.
