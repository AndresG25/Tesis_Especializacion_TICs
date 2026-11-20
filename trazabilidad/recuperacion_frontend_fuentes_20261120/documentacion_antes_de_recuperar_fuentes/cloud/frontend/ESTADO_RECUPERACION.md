# Estado de recuperación del dashboard

## Material recuperado

La carpeta publicado/ contiene exactamente los cinco archivos recibidos en dashboard_publicado_7jb5gwt2.zip, con 559.527 bytes en total (aproximadamente 546,4 KiB). Todas sus huellas SHA-256 coinciden con el manifiesto del respaldo.

| Archivo | Función |
|---|---|
| index.html | Página de entrada; carga el JavaScript y la hoja de estilos. |
| assets/index-BGNGohYv.js | Aplicación compilada, con su lógica y bibliotecas agrupadas. |
| assets/index-nqMpL4T3.css | Hoja de estilos compilada. |
| favicon.svg | Icono referenciado por index.html. |
| icons.svg | Recurso gráfico conservado en el respaldo. |

Origen registrado: s3://turbina-dashboard-web-andresito25/. La exportación previa de CloudFront identifica d26s2ift3srciv.cloudfront.net como distribución asociada. La descarga del dashboard se registró el 20 de noviembre de 2026 a las 05:20:38 UTC. No se consultó el sitio en vivo durante esta revisión.

## Funcionalidad visible en el código compilado

El dashboard consulta GetLatest para las tarjetas de último dato y GetTimeseries para las series históricas. Ambas direcciones coinciden con las exportadas de Lambda. El clientId incluido es ESP32C6-AWS.

Presenta temperatura, humedad, presión, altitud, velocidad del viento, RPM, voltaje y uso de memoria. El código programa la consulta de último dato cada 30 segundos. Para el histórico ofrece rangos de 15 minutos, 1 hora, 24 horas y 7 días; los dos primeros tienen actualización automática nominal de 60 segundos y los otros se consultan manualmente. Estas son características leídas del código, no mediciones de comportamiento en ejecución.

La presencia de opciones de 24 horas y 7 días no certifica que el backend acepte esos rangos. GetTimeseries aplica MAX_QUERY_MINUTES con valor por defecto de 180 minutos; el valor real de esa variable se omitió en la exportación. Debe confirmarse antes de documentar el alcance efectivo de las consultas. Se conserva el código tal como se recibió.

## Material pendiente

No hay archivos .map, directivas sourceMappingURL ni campos sourcesContent en el JavaScript o el CSS. El ZIP tampoco incluye src/, package.json, archivo de bloqueo de dependencias o configuración de Vite. Por tanto, este respaldo conserva el sitio compilado, pero no permite recuperar de forma exacta los archivos originales, comentarios ni versiones completas del proyecto de desarrollo.

Se debe localizar la carpeta original que contenga package.json y src/, junto con public/ y los archivos de configuración cuando existan. docs/buscar_frontend.R ayuda a encontrar candidatos en el computador. No se reconstruyó ni se presentó una aplicación nueva como si fuera la original.

Las direcciones del backend permanecen en el JavaScript original. Abrir el sitio servido localmente puede generar consultas reales a AWS; no se incluye una previsualización que las ejecute automáticamente como parte de esta revisión.
