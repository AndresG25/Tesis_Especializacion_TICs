# Recuperación del dashboard — control documental

## Verificaciones realizadas

- ZIP recibido: dashboard_publicado_7jb5gwt2.zip; su lectura y comprobación CRC terminaron sin errores.
- Cinco archivos web, todos incluidos en el manifiesto; sus tamaños y SHA-256 coinciden.
- Los recursos locales referenciados por index.html están presentes.
- No se encontraron mapas de fuentes externos ni referencias o contenido de mapas embebidos en JavaScript/CSS.
- Las direcciones de GetLatest y GetTimeseries coinciden exactamente con las Function URLs del respaldo Lambda, descontando los parámetros de consulta.
- Los cinco archivos publicados y los tres fuentes Python se conservaron byte por byte.
- El JavaScript se comprobó sintácticamente con node --check, sin ejecutarlo. No se realizaron pruebas contra AWS ni mediciones de funcionamiento del sistema.

El informe de correspondencias, rutas, tamaños y huellas está en trazabilidad/recuperacion_frontend_20261120/verificacion_frontend.json. Se conserva allí el ZIP recibido y su manifiesto original. La evidencia previa de las Lambda permanece en trazabilidad/recuperacion_lambda_20261120/.

## Relación con la observación A4

| Componente | Resultado de esta recuperación | Pendiente |
|---|---|---|
| Funciones Lambda | Tres códigos fuente, paquetes originales, metadatos y documentación disponibles. | Completar valores de configuración e instrucciones de reproducción de recursos y permisos. |
| Dashboard publicado | Cinco archivos íntegros, con correspondencia comprobada de sus dos endpoints Lambda. | Localizar el proyecto original React/Vite para editar y recompilar con trazabilidad. |
| Reproducibilidad del conjunto | Se amplía el material disponible para el repositorio y el Anexo D. | No marcar la observación como totalmente subsanada hasta completar los elementos anteriores. |

## Punto de configuración por confirmar

La interfaz ofrece 15 minutos, 1 hora, 24 horas y 7 días. El fuente GetTimeseries limita minutes mediante MAX_QUERY_MINUTES y usa 180 como valor por defecto. La variable figura en los metadatos, pero su valor no se exportó. Esto impide confirmar desde el respaldo si estaban habilitados 24 horas y 7 días. Es una comprobación documental pendiente; no se modifica la función ni el dashboard.

El análisis del bundle describe intervalos programados de consulta. No se deben confundir con la frecuencia de muestreo de los ESP32, latencia extremo a extremo, continuidad demostrada de datos o capacidad validada de la plataforma.

## Búsqueda del proyecto editable

El título HTML es turbina-dashboard: úselo como pista de búsqueda, sin asumir que sea el nombre exacto de la carpeta. En RStudio, con este complemento incorporado al proyecto de la tesis, ejecute:

```r
source("docs/buscar_frontend.R", encoding = "UTF-8")
```

El script usa funciones de R base y no necesita paquetes adicionales. Solo lee carpetas y package.json; omite node_modules y otras dependencias o cachés. Busca en el perfil de usuario y muestra candidatos con React, Vite y src/. Si el proyecto estaba fuera de ese perfil, ejecute buscar_frontend_tesis(raices = "D:/Proyectos") con la ruta real. Las carpetas inaccesibles y los enlaces simbólicos no se recorren. El script no fue ejecutado en el computador Windows del autor durante esta revisión.

Una vez identificado el candidato, conservar src/, public/ si existe, package.json, el archivo de bloqueo y la configuración original. Preparar el respaldo sin node_modules ni credenciales; documentar la configuración mediante ejemplos. No sustituir archivos originales por una reconstrucción del bundle.

## Integración y tesis

Las carpetas cloud/, docs/ y trazabilidad/ van en la raíz del repositorio, al nivel de ESP1/, ESP2/, ESP3/ y tesis/. Esta entrega no cambia .qmd, numeración, figuras ni scripts de renderizado. GitHub se actualiza mediante la revisión, commit y push locales del autor.
