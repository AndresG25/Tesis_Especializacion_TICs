# GetLatest

Consulta en DynamoDB el último registro del dispositivo solicitado y entrega una respuesta HTTP JSON.

## Entrada y comportamiento

Petición GET con queryStringParameters.clientId; también atiende OPTIONS para CORS.

La tabla se selecciona mediante DDB_TABLE_NAME o TABLE_NAME; el respaldo del código es turbina_latest. La clave DDB_TABLE observada en el inventario de entorno no se lee en esta versión.

## Archivo y entorno

- Fuente: `lambda_function.py`, recuperado de AWS sin cambios en sus bytes.
- Runtime exportado: `python3.12`.
- Handler: `lambda_function.lambda_handler`.
- Memoria: 256 MB. Timeout: 3 s.
- Versión: `$LATEST`. Última modificación informada por AWS: `2026-05-16T05:25:53.000+0000`.
- Variables de entorno cuyos nombres se recuperaron: `ALLOWED_ORIGIN`, `DDB_TABLE`.
- Los valores reales de esas variables no se incluyeron en el respaldo.
- No hay capas Lambda declaradas. El código importa boto3 y bibliotecas estándar de Python; no se capturó la versión exacta del SDK del runtime.

`configuracion.example.json` es una referencia para completar la configuración, no una receta automática ni una copia de sus valores efectivos. Las referencias originales de AWS están en trazabilidad/recuperacion_lambda_20261120/respaldo_aws.

## Observación de configuración

El inventario contiene ALLOWED_ORIGIN y DDB_TABLE. El código no consulta DDB_TABLE: busca DDB_TABLE_NAME o TABLE_NAME y, si no existen, utiliza turbina_latest. Esta diferencia se documenta; no se ha corregido ni desplegado cambio alguno. Para reproducir el código, confirmar la tabla y utilizar una clave que el código realmente lea.
