# Alcance y trazabilidad de la entrega

## Correspondencia con los archivos recibidos

| Archivo adjunto | Raíz original | Carpeta de entrega | Fuente principal |
| --- | --- | --- | --- |
| `ESP1.zip` | `clase-UART` | `ESP1` | `main/main7.c` |
| `ESP2.zip` | `Proyecto_Integrador` | `ESP2` | `main/main7.c` |
| `ESP3.zip` | `AWS_IoT_Core` | `ESP3` | `main/main2.c` |

Los nombres internos de proyecto de CMake se conservan. Las rutas antiguas que aparecen en el cuerpo de la tesis se corresponden con esta tabla; los anexos actualizados indican las rutas del repositorio.

## Cambios de preparación

1. Se conservaron los fuentes, componentes y tablas de particiones de los tres nodos. Los `sdkconfig` recibidos se entregan como `sdkconfig.defaults` para versionar su configuración de referencia.
2. En `ESP3/main/main2.c` se sustituyó el bloque de credenciales Wi-Fi/AWS por la inclusión de `config_local.h`. Se conservaron el puerto 8883, los reintentos y el resto del código.
3. En el encabezado de la biblioteca auxiliar Wi-Fi de ESP2 se sustituyeron las credenciales por una configuración local opcional. `ESP2/main/main7.c` no se modificó.
4. En `ESP3/main/CMakeLists.txt` se añadió una comprobación de existencia de la configuración local y los certificados antes del registro del componente.
5. Se añadieron README, plantillas, documentación de interfaces y subida, metadatos de cita, exclusiones de Git y avisos de terceros.

Se excluyeron los directorios de compilación, binarios, configuraciones personales de editor, contraseñas y archivos de identidad del dispositivo. No se incluyeron copias de los ZIP originales ni diferencias de texto que revelen valores privados.

## Material disponible y pendiente

| Elemento | Estado en esta entrega |
| --- | --- |
| Firmware ESP1, ESP2 y ESP3 | Incluido. |
| Componentes locales utilizados por el firmware | Incluidos, con sus avisos conservados. |
| Configuración de referencia y plantillas privadas | Incluidas; cada despliegue debe completar sus valores locales. |
| Certificado y llave privada de dispositivo | Aportados únicamente en el equipo de trabajo; excluidos del repositorio. |
| Funciones Lambda `GetLatest`, `GetTimeseries`, `ActualizarUltimoDatoDynamo` | No adjuntas; pendientes de incorporación por el autor. |
| Dashboard React/Vite y sus dependencias | No adjuntos; pendientes de incorporación por el autor. |
| Reglas IoT, políticas y definición reproducible de recursos AWS | No adjuntas como configuración desplegable. |
| Proyecto Quarto y PDF de la tesis | Se entregan por separado; no forman parte de este ZIP de firmware. |

Cuando se incorporen Lambda y frontend, conviene darles carpetas propias, documentar dependencias y configuración mediante ejemplos sin secretos, y actualizar el README y el Anexo D. No se crearon carpetas vacías que simulen contener esos componentes.

## Verificación de esta preparación

Se compararon los archivos conservados con los originales. Los fuentes principales de ESP1 y ESP2 y los componentes no intervenidos permanecen idénticos por bytes. Para ESP3 se verificó que el cambio del fuente queda limitado al bloque de configuración. Los registros de comprobación se conservan en `VERIFICACION_ENTREGA.json`.

También se comprobaron los enlaces locales Markdown, la correspondencia del ejemplo JSON con las claves emitidas por ESP2 y la exclusión de archivos privados por Git. Estas verificaciones evalúan la preparación del paquete.

**No se ejecutó una nueva compilación con ESP-IDF ni una prueba sobre placas o AWS.** Las versiones de referencia proceden de los archivos recibidos; no deben presentarse como resultado de una compilación nueva de esta entrega.
