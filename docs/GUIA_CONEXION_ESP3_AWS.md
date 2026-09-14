# Reproducción de la conexión ESP3 con AWS IoT Core

Esta guía acompaña al Anexo C y organiza la preparación y comprobación del enlace usado en la tesis. Está basada en el firmware entregado y en las fuentes oficiales enlazadas en cada etapa. La elaboración de esta guía incluyó revisión del código y de la configuración; no incluyó una nueva compilación ESP-IDF, prueba con placa ni conexión a la cuenta AWS del autor. Las evidencias históricas están en el capítulo 5. Quien reproduzca el sistema debe registrar su propia ejecución con la plantilla incluida.

El relato del autor sitúa el desarrollo inicial del enlace en aproximadamente tres meses: intentos con MicroPython, continuación en C/ESP-IDF y apoyo de DeepSeek. Ese relato describe el caso del proyecto y no establece una comparación general entre lenguajes ni entre asistentes de IA.

## 1. Identificar el proyecto y el entorno

Proyecto: `ESP3/Firmware nodo 3 e integración con AWS_IoT_Core/`.

| Recurso | Uso en esta versión |
|---|---|
| `main/main2.c` | Firmware activo de ESP3. |
| `main/config_local.example.h` | Plantilla de los cinco parámetros del despliegue. |
| `main/CMakeLists.txt` | Dependencias y archivos de certificados incorporados al ejecutable. |
| `sdkconfig.defaults` | Configuración recibida, con `CONFIG_IDF_INIT_VERSION="5.4.3"` y objetivo `esp32c6`. |
| `partitions.csv` | Particiones del proyecto. |

Para este nodo, el README y la configuración recibida señalan **ESP-IDF 5.4.3**. La introducción de la tesis menciona 5.4.1 como entorno de desarrollo; conserve esta distinción y registre la versión efectivamente utilizada en cada repetición. No se presupone equivalencia de resultados entre versiones.

En una terminal de ESP-IDF ejecute `idf.py --version`. Registre también la placa, su puerto y el commit del repositorio. En Windows use una ruta de compilación sin espacios ni tildes. La carpeta descriptiva se mantiene como fuente del proyecto.

## 2. Preparar la identidad y el endpoint de AWS

Si reutiliza la instalación de la tesis, consulte primero sus valores existentes. Para una instalación nueva, trabaje en la cuenta y región donde se crearán los recursos.

1. En AWS IoT Core cree o identifique el objeto (*thing*) de la ESP3.
2. Cree o seleccione su certificado de cliente. Descargue el certificado y su llave privada; active el certificado y asócielo al objeto.
3. Adjunte al certificado una política IoT que permita las operaciones del nodo. La asociación con el objeto identifica el dispositivo; la política autoriza sus acciones.
4. Obtenga el endpoint de datos **ATS** de esa cuenta y región. Guarde únicamente el nombre de host, sin `mqtts://`, sin puerto y sin ruta. El firmware utiliza `.broker.address.hostname` y define el puerto 8883 por separado.

Referencias: [crear certificados](https://docs.aws.amazon.com/iot/latest/developerguide/device-certs-create.html) y [asociar objeto y política al certificado](https://docs.aws.amazon.com/iot/latest/developerguide/attach-to-cert.html).

Opcionalmente, con AWS CLI ya autenticada, consulte el endpoint; sustituya `REGION` por su región:

```bash
aws iot describe-endpoint --endpoint-type iot:Data-ATS --region REGION --query endpointAddress --output text
```

El comando requiere permiso `iot:DescribeEndpoint`. [Referencia oficial del comando](https://docs.aws.amazon.com/cli/latest/reference/iot/describe-endpoint.html).

## 3. Hacer coincidir cliente, tópico y política

La [plantilla de política de publicación](plantillas/politica_esp3_publicacion.json) permite únicamente conectar un identificador de cliente y publicar en un tópico exacto. Sustituya los cuatro marcadores `REEMPLAZAR_REGION`, `REEMPLAZAR_CUENTA`, `REEMPLAZAR_CLIENT_ID` y `REEMPLAZAR_TOPICO`. Es una plantilla para configurar una instalación; no es una copia de la política privada del autor.

| Operación | Recurso que debe coincidir |
|---|---|
| `iot:Connect` | ARN `client/` con el mismo identificador de `AWS_IOT_CLIENT_ID`. |
| `iot:Publish` | ARN `topic/` con el mismo tópico de `AWS_IOT_TOPIC`. |

Adjunte la política al certificado activo. Los manejadores de eventos del firmware incluyen eventos de suscripción, pero el programa entregado no solicita suscripciones: para su publicación UART no se necesitan permisos de suscripción. La sesión del cliente de pruebas de la consola tiene su propia autorización de usuario. [Políticas IoT de publicación y suscripción](https://docs.aws.amazon.com/iot/latest/developerguide/pub-sub-policy.html).

La regla IoT que envía datos a DynamoDB o S3 debe seleccionar el mismo tópico. Cambiarlo solo en el firmware puede dejar mensajes visibles en el broker sin activar el almacenamiento. Conserve el tópico existente al repetir el despliegue de la tesis.

## 4. Incorporar la configuración local

En la carpeta versionada del proyecto ESP3, copie `main/config_local.example.h` como `main/config_local.h` y complete:

| Parámetro | Valor |
|---|---|
| `WIFI_SSID` | Nombre de la red disponible para la placa. |
| `WIFI_PASS` | Contraseña de esa red. |
| `AWS_IOT_ENDPOINT` | Host ATS obtenido en el paso 2. |
| `AWS_IOT_TOPIC` | Tópico exacto autorizado y utilizado por las reglas. |
| `AWS_IOT_CLIENT_ID` | Identificador exacto autorizado por la política. |

El código configura Wi-Fi en modo estación y un umbral de autenticación WPA2-PSK. Coloque los archivos con estos nombres:

- `main/certs/device_cert.crt`: certificado de la ESP3.
- `main/certs/private_key.key`: llave privada correspondiente a ese certificado.
- `main/certs/AmazonRootCA1.pem`: recurso público de raíz ya incluido en el proyecto.

El encabezado local y las credenciales privadas están excluidos de Git por la configuración existente. Su incorporación local es necesaria para compilar; no deben formar parte del commit. El firmware embebe las credenciales en el ejecutable, por lo que también deben controlarse las copias de compilación.

## 5. Comprobar confianza, formato de certificados y reloj

En `mqtt_app_start()` la identidad del cliente se configura mediante `.credentials.authentication.certificate` y `.key`. La verificación del servidor utiliza **`esp_crt_bundle_attach`**; el archivo `AmazonRootCA1.pem` se incorpora como recurso, pero no está asignado directamente como certificado de verificación del broker. La configuración recibida habilita `CONFIG_MBEDTLS_CERTIFICATE_BUNDLE` y el conjunto completo de certificados. [Documentación del almacén de confianza de ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v5.4.3/esp32c6/api-reference/protocols/esp_crt_bundle.html).

El cliente MQTT recibe los certificados como cadenas PEM. Antes de compilar en otro entorno, compruebe su terminación nula y la forma de incorporación en CMake: la configuración recibida usa `EMBED_FILES`. Si es necesario ajustar la incorporación de texto PEM mediante `EMBED_TXTFILES`, registre el cambio y repita la compilación. No atribuya un error de enlace o de lectura PEM a AWS sin revisar primero estos recursos. ESP-MQTT documenta los formatos PEM/DER y sus requisitos de longitud. [Configuración de ESP-MQTT](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32c6/api-reference/protocols/mqtt.html).

La función `sync_time()` del fuente recibido consulta y reasigna el reloj local; **no implementa sincronización SNTP**. Además, `sdkconfig.defaults` contiene `CONFIG_MBEDTLS_HAVE_TIME=y`, pero no habilita `CONFIG_MBEDTLS_HAVE_TIME_DATE`. Registre esa condición y no tome una conexión satisfactoria como evidencia de que el dispositivo comprobó la vigencia temporal del certificado del servidor. Para incorporar esa comprobación se debe disponer de hora correcta antes de iniciar TLS y revisar la configuración de mbedTLS. Esta guía conserva el firmware recibido y deja identificada la mejora.

## 6. Preparar una copia de compilación

Después de completar los recursos locales, abra el proyecto RStudio habitual. En la **consola de R** ejecute:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP3", "C:/esp_tesis/ESP3_final")
```

El destino debe ser nuevo. Si ya existe, elija otro nombre para conservar la configuración anterior. El script copia el proyecto y comprueba que los archivos copiados coincidan con el origen. La copia preparada puede contener las credenciales locales; úsela únicamente para la compilación.

Abra una **terminal ESP-IDF 5.4.3** en ese destino y ejecute:

```bash
idf.py --version
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Sustituya `COM5` por el puerto real. En caso de error, conserve la primera causa relevante del log; no avance al paso siguiente sin resolverla. Si cambió una configuración en la copia, documente el ajuste que deba regresar al fuente versionado y vuelva a preparar la copia. No copie de regreso las credenciales.

## 7. Conectar UART y comprobar la publicación

UART1 de ESP3 está configurada a **9600 bit/s, 8N1**, con **RX GPIO22** y **TX GPIO15**. Para el recorrido usado en la tesis conecte TX GPIO15 de ESP2 a RX GPIO22 de ESP3 y una referencia de tierra común. Use los niveles lógicos de las placas. La salida TX de ESP3 solo se necesita si se añade comunicación de retorno.

En la consola de AWS IoT Core abra el cliente de pruebas MQTT y suscríbase al tópico exacto de `AWS_IOT_TOPIC` antes de generar telemetría. Use el sistema ESP1–ESP2–ESP3 para la prueba completa. ESP2 debe generar su mensaje real; abrir el monitor de ESP3 por sí solo no genera telemetría UART.

| Etapa | Evidencia que se debe guardar |
|---|---|
| Wi-Fi | Dirección IP obtenida y evento de conexión. |
| MQTT/TLS | Mensaje `MQTT conectado a AWS IoT Core!` del manejador `MQTT_EVENT_CONNECTED`. |
| UART | Mensaje `Payload:` con la trama recibida desde ESP2. |
| Publicación | Identificador devuelto al solicitar el envío y evento `MQTT_EVENT_PUBLISHED` con `Publicación confirmada`. |
| Recepción en AWS | Captura del cliente MQTT con tópico, contenido y momento de la prueba. |

El código solicita **QoS 1**, con `retain = 0`. El evento de publicación confirma la recepción en el broker; la comprobación de DynamoDB, S3, Athena y el dashboard corresponde a las etapas posteriores documentadas en los capítulos 4 y 5. El retorno de `esp_mqtt_client_publish()` por sí solo no acredita ese almacenamiento.

## 8. Diagnosticar por etapa

La tabla orienta comprobaciones; no atribuye estas causas a los intentos históricos del autor, cuyos logs completos no se suministraron para esta guía.

| Situación observada | Comprobación siguiente |
|---|---|
| CMake informa un archivo faltante | Nombres y ubicación de `config_local.h`, certificado y llave dentro de `main/`. |
| Error al incorporar o enlazar certificados | Rutas CMake, formato PEM, terminación de cadenas y símbolos de los recursos embebidos. |
| No se obtiene IP | SSID, contraseña, tipo de autenticación y disponibilidad de la red. |
| Fallo de transporte o negociación TLS | Host ATS, DNS, salida por 8883, certificado/llave, confianza del servidor y configuración del reloj. |
| MQTT no conecta o se desconecta | Certificado activo, política adjunta, identificador autorizado y otro dispositivo usando el mismo identificador. |
| MQTT conecta pero no hay mensaje | Recepción UART; el nodo publica cuando recibe una trama admitida. |
| Se solicita publicación pero no aparece confirmación | Eventos de error, permisos de publicación y estado de la conexión. |
| El cliente de pruebas no muestra la telemetría | Tópico, región y cuenta de la sesión de pruebas; suscripción iniciada antes del envío. |
| Hay mensaje en IoT Core pero no en almacenamiento | Regla IoT, selección del tópico, acción configurada y permisos del servicio de destino. |

El ensamblador UART entregado utiliza delimitadores y un buffer de 512 bytes, y no valida un esquema JSON completo. Durante una desconexión MQTT descarta las nuevas tramas; no dispone de cola persistente de recuperación. Estas condiciones deben conservarse en la interpretación de las pruebas.

## 9. Registrar la repetición

Copie [registro_prueba_esp3.md](plantillas/registro_prueba_esp3.md) a una carpeta de ejecución local y complete únicamente los resultados observados. Incluya commit, versión de ESP-IDF, fecha, configuración no sensible, logs y captura de recepción. Las fuentes de la guía y sus plantillas pueden versionarse; los registros privados y las credenciales permanecen en el entorno local.

El criterio de éxito del enlace es una trama real de ESP2 recibida por ESP3, confirmada por el cliente MQTT y observada con el mismo contenido en AWS IoT Core. La reproducción del sistema completo requiere comprobar después sus destinos de almacenamiento y visualización.
