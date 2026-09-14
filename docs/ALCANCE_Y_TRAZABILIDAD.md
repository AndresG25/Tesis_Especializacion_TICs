# Alcance y trazabilidad del repositorio integrado

## Incluido

- Proyecto Quarto completo de la tesis, figuras, capturas, bibliografía y estilos.
- Un proyecto RStudio en la raíz que abarca la tesis y los tres nodos ESP.
- Firmware de ESP1, ESP2 y ESP3 dentro de los nombres descriptivos adjuntados por el autor.
- Componentes locales, configuraciones de referencia, plantillas y documentación.
- Scripts para renderizar, registrar el estado de Git, comprobar la entrega y preparar copias locales de compilación.
- Informes de revisiones anteriores y referencias originales.

Las rutas exactas se registran en [RUTAS_PROYECTOS.csv](RUTAS_PROYECTOS.csv). Los nodos principales conservan `main7.c` en ESP1/ESP2 y `main2.c` en ESP3. Los algoritmos y componentes coinciden con los revisados previamente; únicamente se mantiene la separación de credenciales que ya se había aplicado.

## Pendiente de incorporar

Los fuentes de las funciones Lambda `GetLatest`, `GetTimeseries` y `ActualizarUltimoDatoDynamo`, el frontend React/Vite y las configuraciones desplegables de AWS no se adjuntaron. El Anexo D declara este alcance. El repositorio no contiene implementaciones supuestas ni carpetas vacías que simulen esos componentes.

## Configuración privada

El certificado de dispositivo, la llave privada y las contraseñas se proporcionan localmente. No se incluyen en el ZIP ni en Git. Se conserva el certificado público AmazonRootCA1 y las plantillas de configuración.

## Cambios de esta integración

Se preservan los nuevos nombres internos y se actualizan las rutas de los README y de los anexos A–C. La tesis queda bajo `tesis/`; ESP1, ESP2 y ESP3 están a su lado. El `.Rproj`, los scripts y las reglas de Git quedan en la raíz. Las referencias a rutas antiguas en los capítulos se conservan como parte de la descripción del proyecto original; el mapa de rutas relaciona la organización actual.

La comprobación detallada está en `trazabilidad/entrega/verificacion_integracion.json`. Los registros previos se conservan en las carpetas de control y en `trazabilidad/historico_firmware/` y `trazabilidad/historico_rstudio/`.

El repositorio remoto se conecta desde el equipo del autor. La preparación del ZIP no realiza commits ni envía cambios a GitHub.
