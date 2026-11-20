# Dashboard de la turbina

Proyecto React/Vite recuperado del ZIP original del portátil. Su dist original coincide byte por byte con el respaldo publicado de S3. El código de la aplicación se conserva sin cambios; esta guía y el comparador se agregaron durante la recuperación.

## Archivos principales

| Archivo o carpeta | Función |
|---|---|
| src/App.jsx | Consulta GetLatest y muestra último dato, estado ESP-NOW y ocho tarjetas de variables. |
| src/Historico.jsx | Consulta GetTimeseries, selecciona variable/rango y grafica puntos con Recharts. |
| src/main.jsx | Monta la aplicación React en el HTML. |
| src/index.css, src/App.css, src/assets/ | Estilos y recursos conservados del proyecto. App.css y algunos recursos de plantilla no se importan actualmente; se preservan como parte del original. |
| public/ | Iconos que se copian a la salida de compilación. |
| package.json y package-lock.json | Scripts, dependencias declaradas y versiones resueltas. |
| vite.config.js y eslint.config.js | Configuración original de compilación y revisión de reglas. |
| .env.example | Configuración pública recuperada: dos URL Lambda y clientId. |
| scripts/verificar_compilacion.mjs | Compara un dist local con las huellas del respaldo publicado y guarda un informe. |

## Dependencias recuperadas

El archivo de bloqueo registra React/React DOM 19.2.6, Recharts 3.8.1, Vite 8.0.13 y @vitejs/plugin-react 6.0.2. Se preservó el lock original. Node 24.x es compatible con todos los requisitos engines.node declarados en ese lock; no se conoce la versión exacta de Node usada en mayo. Node y npm se necesitan para este frontend; R/Quarto siguen siendo el entorno de la tesis.

## Compilación local

Ejecute estos comandos en la pestaña Terminal de RStudio, desde la raíz del repositorio. También funcionan en una terminal de VS Code. No se ejecutan en la consola de R.

```sh
cd cloud/frontend/turbina-dashboard
node --version
npm --version
npm ci --include=dev
node -e "const f=require('node:fs'); if(!f.existsSync('.env.local')) f.copyFileSync('.env.example','.env.local');"
npm run build
node scripts/verificar_compilacion.mjs
```

Ejecute cada paso tras comprobar que el anterior terminó correctamente. npm ci instala según package-lock.json y descarga las dependencias necesarias para su plataforma. La instrucción de configuración crea .env.local solo si aún no existe. El comando build genera dist/; no despliega archivos en AWS.

El comparador guarda un JSON en trazabilidad/verificaciones_frontend_local/ dentro del repositorio. Si informa ARCHIVOS_IDENTICOS_AL_RESPALDO_S3 y los fuentes están sin modificar, los archivos del dist evaluado coinciden con la referencia. Ese resultado se interpreta como comprobación de la recompilación únicamente si el build anterior acaba de terminar correctamente. El comparador no ejecuta por sí mismo el compilador ni evalúa conexiones a AWS.

Si aparecen diferencias, conserve el JSON y compártalo para revisar versiones, configuración y archivos. La copia publicado/ se conserva como referencia. No se afirma de antemano que una recompilación en otro entorno produzca los mismos bytes.

## Configuración del frontend

| Variable | Uso |
|---|---|
| VITE_LATEST_URL | URL completa de GetLatest, incluido ?clientId=ESP32C6-AWS. |
| VITE_TIMESERIES_URL | URL base de GetTimeseries, sin parámetros; Historico.jsx agrega metric, minutes y clientId. |
| VITE_CLIENT_ID | Identificador utilizado en las consultas históricas. |

Los valores de ejemplo ya estaban embebidos en el sitio publicado y se contrastaron con las Function URLs recuperadas. .env.example conserva los bytes de los .env y .env.production originales, que eran idénticos. No contiene claves de acceso ni certificados. Para cambiar el dispositivo, ajuste tanto VITE_CLIENT_ID como el clientId de VITE_LATEST_URL: el código original no los unifica automáticamente.

Vite incorpora las variables VITE_ al cliente al compilar. Un .env.production local o variables VITE_ ya definidas en la terminal pueden tomar precedencia sobre .env.local; revíselas si la salida difiere. Documentación: https://vite.dev/guide/env-and-mode y https://docs.npmjs.com/cli/v11/commands/npm-ci/.

## Revisión estática del original

La sintaxis y los imports locales fueron comprobados. npm run lint conserva tres incumplimientos: no-unused-vars en App.jsx:1 y react-hooks/set-state-in-effect en Historico.jsx:121 y :160. No se aplicaron correcciones para mantener el respaldo original. El script build no ejecuta ESLint; su resultado debe comprobarse aparte.

## Visualización y repositorio

Después de la compilación puede ejecutar npm run preview. Abrir la aplicación realiza consultas a las URL configuradas; esta revisión no las ejecutó. El uso de rangos de 24 horas/7 días depende del límite configurado en GetTimeseries, todavía por confirmar.

Para versionar, conserve fuentes, public/, lock, configuración, README, ejemplo y comparador. node_modules/, dist/ y archivos de entorno locales quedan ignorados. El respaldo publicado se guarda aparte en ../publicado/. La carpeta cloud/ se incorpora en el repositorio privado de la tesis sin reemplazar los capítulos.

Se agrega .gitattributes para conservar los finales de línea LF observados en los fuentes y evitar diferencias de huellas debidas a conversiones de Git en Windows.
