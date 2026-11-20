# Tesis de IoT y Big Data en AWS

Proyecto integrado de **Carlos Andrés Gómez Flórez**: documento editable de la tesis y firmware de los tres nodos ESP32-C6, administrados desde un único proyecto de RStudio con Git.

**Tesis:** Desarrollo de una arquitectura escalable de Big Data en AWS alimentada mediante un sistema de IoT.  
**Repositorio:** [AndresG25/Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs)  
**Contacto:** [andres25@utp.edu.co](mailto:andres25@utp.edu.co)

## Empezar en RStudio

Abra **`Tesis_Especializacion_TICs.Rproj` en la raíz**. La tesis y los tres nodos pertenecen a este mismo proyecto y al mismo repositorio Git.

Desde la ventana mostrada por el autor, siga [Iniciar en RStudio](docs/INICIAR_EN_RSTUDIO.md): **New Project → Version Control → Git**, clone el repositorio y copie dentro de ese clon el contenido de esta entrega. Conserve la carpeta `.git` creada por la clonación.

## Organización

| Carpeta o archivo | Contenido |
| --- | --- |
| `Tesis_Especializacion_TICs.Rproj` | Punto de entrada único de RStudio. |
| [tesis](tesis/) | Capítulos QMD, anexos, bibliografía, figuras, configuración Quarto y estilos LaTeX. |
| [ESP1](ESP1/README.md) | `Firmware nodo ESP1 - Variables Generador`: GPS, voltaje, RPM y envío ESP-NOW. |
| [ESP2](ESP2/README.md) | `Firmware nodo 2 - Variables ambientables e integración con ESP1`: integración local, sensores, RTC, microSD, OLED y JSON por UART. |
| [ESP3](ESP3/README.md) | `Firmware nodo 3 e integración con AWS_IoT_Core`: recepción UART y publicación MQTT/TLS. |
| [docs](docs/) | Inicio con Git, configuración del firmware, interfaces y mapa de rutas. |
| [trazabilidad](trazabilidad/LEEME_TRAZABILIDAD.md) | Inventarios de entrega, comprobaciones y registros históricos. |
| `control_cambios/`, `control_diagramacion/`, `control_final/` | Informes y diferencias de las revisiones de la tesis. |
| `referencia_original/` | Proyecto y PDF originales conservados como referencia. |

Los nombres internos se conservan **exactamente como fueron adjuntados**, incluido «ambientables». Las descripciones funcionales utilizan «variables ambientales».

## Generar la tesis desde R

En la consola de RStudio, con el proyecto de la raíz abierto:

```r
source("renderizar_tesis.R", encoding = "UTF-8")
```

El PDF queda en `tesis/output/`. Cada ejecución guarda localmente los fuentes activos, las versiones del entorno, el estado de Git, los mensajes de compilación y copias del PDF y del TeX. Los registros por ejecución se excluyen de Git. El PDF ya renderizado se incluye en el ZIP de entrega.

Para comprobar la integridad inicial:

```r
source("verificar_entrega.R", encoding = "UTF-8")
```

Los scripts usan R base, Quarto y LaTeX. No instalan software automáticamente. Consulte [Abrir en RStudio](ABRIR_EN_RSTUDIO.md).

## Guardar cambios en GitHub

Después de clonar y copiar los archivos, compruebe la configuración local:

```r
source("configurar_git.R", encoding = "UTF-8")
```

El script configura el remoto si falta y conserva un repositorio ya existente. No crea commits ni publica archivos. En la pestaña **Git** de RStudio, revise las diferencias, seleccione los archivos, escriba un mensaje de **Commit** y utilice **Push**. La [guía de inicio](docs/INICIAR_EN_RSTUDIO.md) explica la primera subida y el trabajo diario.

## Compilar el firmware

Las carpetas descriptivas contienen espacios. Para preparar una copia con rutas compatibles con ESP-IDF:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP1")
```

Puede seleccionar `ESP2` o `ESP3`. El comando muestra la carpeta de compilación y la versión correspondiente de ESP-IDF: **5.4.1** para ESP1/ESP2 y **5.4.3** para ESP3. Si la ruta del repositorio contiene espacios o tildes, indique un destino nuevo como `C:/esp_tesis/ESP1`.

Edite los archivos versionados en las carpetas descriptivas y prepare una nueva copia al compilar. Complete localmente las credenciales y los certificados de ESP3 según [Configuración y compilación](docs/CONFIGURACION_Y_COMPILACION.md). Los algoritmos del firmware se conservan; esta integración no constituye una nueva prueba del sistema físico.

## Alcance y acceso

La entrega incluye tesis y firmware ESP1–ESP3. Las funciones Lambda, el frontend React/Vite y las configuraciones desplegables de recursos AWS siguen pendientes porque no se adjuntaron sus fuentes. El [alcance documentado](docs/ALCANCE_Y_TRAZABILIDAD.md) y el Anexo D indican esta disponibilidad.

El repositorio permanece privado. Los evaluadores pueden solicitar acceso al autor indicando su usuario de GitHub. Las contraseñas, certificados de dispositivo, llaves privadas y resultados de compilación se mantienen fuera de Git.

Consulte [CITATION.cff](CITATION.cff), [LICENSING.md](LICENSING.md) y [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) para autoría, cita y componentes de terceros.
