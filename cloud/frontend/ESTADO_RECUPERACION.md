# Estado del frontend: fuentes recuperados

Se recibió turbina-dashboard.zip del portátil del autor, con src/, public/, package.json, package-lock.json, configuración de Vite/ESLint, variables de entorno y dist/. Los cinco archivos de dist/ son idénticos por SHA-256 al respaldo publicado descargado de S3. Ambos archivos de entorno contienen los mismos tres valores públicos usados por la aplicación.

El proyecto editable está en turbina-dashboard/. La copia publicada de referencia está en publicado/. Los fuentes de la aplicación, los recursos, el archivo de bloqueo y las configuraciones Vite/ESLint se conservaron byte por byte. Se documentaron el README, la exclusión de dependencias instaladas y la configuración de ejemplo.

## Lo que hace el código

App.jsx consulta GetLatest y presenta ocho variables, con un intervalo programado de consulta de 30 segundos. Historico.jsx consulta GetTimeseries, permite elegir variable y rango, y muestra las series con Recharts. main.jsx monta la aplicación React; los CSS y public/ aportan estilos y recursos.

El histórico ofrece 15 minutos, 1 hora, 24 horas y 7 días. Las dos primeras opciones tienen refresco automático nominal de 60 segundos. La capacidad efectiva de aceptar rangos largos depende de MAX_QUERY_MINUTES en Lambda; su valor desplegado no se incluyó en la exportación anterior y sigue pendiente de confirmar.

## Alcance de la verificación

Se verificaron la integridad del ZIP, la identidad de dist con S3, la sintaxis de cinco módulos JS/JSX, los imports locales y la correspondencia de las variables con las Function URLs recuperadas. ESLint del código original arroja tres errores de reglas: un import React sin usar y dos llamadas a setState dentro de efectos. Se conservan documentados sin intervenir la aplicación.

No se ejecutó una recompilación de Vite en esta revisión. El respaldo de node_modules trae módulos nativos para Windows; no es un entorno trasladable directamente al sistema de revisión. Se proporciona la guía para reinstalar las dependencias y comparar un dist nuevo en el computador del autor. No se probó el sistema contra AWS.
