# Software cloud recuperado de la tesis

Las funciones Lambda y el dashboard publicado se recuperaron de AWS en us-east-2 el 20 de noviembre de 2026 (fecha UTC de los respaldos). Los fuentes Python y los cinco archivos web se conservan byte por byte. La organización y la documentación de este complemento se agregaron después de la recuperación.

| Carpeta | Contenido |
|---|---|
| lambda/ActualizarUltimoDatoDynamo | Fuente Python que normaliza la telemetría y guarda el último estado en DynamoDB. |
| lambda/GetLatest | Fuente Python que entrega el último registro por clientId mediante HTTP/JSON. |
| lambda/GetTimeseries | Fuente Python que consulta el histórico con Athena y devuelve puntos de la serie. |
| frontend/publicado | Los cinco archivos del sitio publicado, recuperados de S3. |
| frontend/ESTADO_RECUPERACION.md | Estado y alcance del rescate del dashboard. |

## Alcance comprobado

Se verificaron las huellas de los respaldos y la identidad de los archivos copiados. Las dos direcciones Lambda presentes en el JavaScript del dashboard coinciden con las Function URLs exportadas de GetLatest y GetTimeseries. Esta comprobación relaciona los artefactos recuperados; no demuestra por sí sola una ejecución correcta del sistema.

Los fuentes originales React/Vite siguen pendientes. También faltan valores de configuración omitidos en la exportación y completar la documentación de recursos y permisos de AWS para reproducir el conjunto. No se inventaron dependencias, valores privados ni resultados de pruebas.

Las Lambda corresponden a $LATEST; el respaldo no contiene alias ni versiones publicadas. Sus fechas LastModified son evidencia de procedencia, no prueba suficiente de identidad con cada ejecución de mayo. Durante esta revisión no se desplegaron ni invocaron funciones y no se hicieron consultas a Athena.

Consulte docs/RECUPERACION_LAMBDA_20261120.md y docs/RECUPERACION_FRONTEND_20261120.md.
