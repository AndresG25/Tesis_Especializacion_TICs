# ESP3 — Pasarela UART a AWS IoT Core

Proyecto ESP-IDF para ESP32-C6, asociado al **Anexo C**. Carpeta original: `AWS_IoT_Core`. El archivo [main/main2.c](main/main2.c) conserva la lógica recibida; las credenciales se trasladaron a un archivo de configuración local.

## Archivos y funciones

| Archivo o elemento | Función |
| --- | --- |
| `main/main2.c` | Conexión Wi-Fi, cliente MQTT/TLS, recepción UART y publicación de mensajes. |
| `main/config_local.example.h` | Plantilla de SSID, contraseña, endpoint, tópico e identificador del cliente. |
| `main/certs/AmazonRootCA1.pem` | Certificado público de autoridad raíz recibido con el proyecto. |
| `main/certs/README.md` | Nombres y ubicación de los archivos privados que se deben añadir localmente. |
| `partitions.csv` | Tabla de particiones recibida. |
| `sdkconfig.defaults` y archivos CMake | Configuración base y recursos que se incorporan al firmware. |

`wifi_init_sta()` configura la interfaz Wi-Fi y sus eventos. `mqtt_app_start()` crea el cliente MQTT con TLS y autenticación mediante certificado de dispositivo. `uart_receive_task()` reconstruye mensajes delimitados por llaves y solicita su publicación en AWS IoT Core. `verify_and_format_json()` comprueba delimitadores y tamaño; no valida un esquema JSON completo. `app_main()` coordina la inicialización y las tareas.

## Preparación local obligatoria

1. Copie `main/config_local.example.h` como `main/config_local.h`.
2. Complete los cinco valores con la configuración de su despliegue. `AWS_IOT_ENDPOINT` debe ser el **nombre de host**, sin `mqtts://` ni rutas, porque el código utiliza el campo `.hostname` del cliente MQTT.
3. Añada su certificado de dispositivo en `main/certs/device_cert.crt` y su llave privada en `main/certs/private_key.key`. Deben corresponder entre sí y a la identidad de AWS IoT que tenga autorizada la conexión y publicación.
4. Confirme que el tópico y el identificador del cliente coincidan con la política y las reglas del despliegue de la tesis.

Los archivos locales anteriores no se incluyen en esta entrega y están excluidos mediante `.gitignore`. Conserve sus originales en su entorno de trabajo. El sistema de compilación indica qué archivo falta antes de continuar.

El certificado público de raíz se conserva como recurso del proyecto. En el código recibido, la verificación del servidor se configura con `esp_crt_bundle_attach`; no se cambió esa selección del almacén de confianza.

## Conexiones y funcionamiento

UART1 utiliza **RX GPIO22, TX GPIO15, 9600 bit/s, 8N1**. Conecte TX GPIO15 de ESP2 a RX GPIO22 de ESP3 y una tierra común. El enlace inverso, si se utiliza, conecta TX GPIO15 de ESP3 a RX GPIO9 de ESP2.

La publicación utiliza el puerto **8883**, QoS 1 y `retain = 0`. El mensaje recibido se conserva sin añadir un campo `clientId`. La incorporación de ese identificador a los datos almacenados, si el despliegue la requiere, corresponde a la configuración de AWS.

Si MQTT está desconectado, el código descarta el mensaje recibido; no dispone de cola persistente ni de recuperación del histórico desde ESP2. `sync_time()` usa el reloj local y no ejecuta sincronización SNTP: revise la hora y la configuración de zona horaria antes de evaluar la conexión TLS.

## Compilar y cargar

Esta carpeta conserva su nombre descriptivo con espacios. Antes de compilar, prepare una copia con ruta corta desde el proyecto RStudio de la raíz:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP3")
```

El comando muestra el destino. Entre en él desde una terminal con **ESP-IDF 5.4.3** y ejecute:

```sh
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Sustituya COM5 por el puerto de su placa. Para ESP3, prepare primero la configuración local y sus certificados según este README. La copia de compilación no sustituye el fuente que se versiona: realice las modificaciones en esta carpeta y vuelva a prepararla.

Consulte [Configuración y compilación](../../docs/CONFIGURACION_Y_COMPILACION.md).
