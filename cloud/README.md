# Software cloud de la tesis

| Carpeta | Contenido |
|---|---|
| lambda/ActualizarUltimoDatoDynamo | Fuente que normaliza telemetría y actualiza el último estado en DynamoDB. |
| lambda/GetLatest | Fuente que entrega el último dato mediante HTTP/JSON. |
| lambda/GetTimeseries | Fuente que consulta series históricas mediante Athena. |
| frontend/turbina-dashboard | Proyecto editable React/Vite recuperado del portátil, con archivo de bloqueo y guía de compilación. |
| frontend/publicado | Los cinco archivos recuperados de S3, idénticos al dist del ZIP del portátil. |

Los fuentes de aplicación recuperados se conservan sin modificar. Los README, la documentación de trazabilidad y el comparador de compilación se agregan como apoyo a la reproducción. No se actualizan versiones de dependencias.

La recuperación de los códigos Lambda y del frontend está completada en los respaldos recibidos. La reproducibilidad integral aún requiere comprobar la compilación local, completar los valores de configuración omitidos de Lambda y documentar los recursos y permisos de AWS necesarios. No se declara cerrada toda la observación A4 por la sola recuperación de estos archivos.

La evidencia está en trazabilidad/recuperacion_lambda_20261120/, trazabilidad/recuperacion_frontend_20261120/ y trazabilidad/recuperacion_frontend_fuentes_20261120/. Las versiones anteriores de la documentación se conservan como evidencia histórica.
