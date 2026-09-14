# ESP2 — Integración y procesamiento local

Proyecto ESP-IDF para ESP32-C6, asociado al **Anexo B**. Carpeta original: `Proyecto_Integrador`. El archivo [main/main7.c](main/main7.c) conserva íntegramente el código recibido.

## Archivos y funciones

| Archivo o carpeta | Función |
| --- | --- |
| `main/main7.c` | Recepción ESP-NOW, sensores, RTC, microSD, OLED y transmisión UART. |
| `components/bme680/` | Controlador para temperatura, humedad y presión. |
| `components/ssd1306/` | Controlador de la pantalla OLED. |
| `components/wifi_connect_lib/` | Biblioteca auxiliar de conexión a un punto de acceso; no es invocada por `main7.c` en esta versión. |
| `partitions.csv` | Tabla de particiones recibida del proyecto. |
| `sdkconfig.defaults` y archivos CMake | Configuración de ESP-IDF y compilación. |

`on_receive_espnow()` recibe las variables de ESP1 y actualiza su estado. `data_task()` consulta el reloj DS3231, lee el BME680 y coordina el registro de muestras y la salida JSON. Las funciones `sd_card_init()`, `sd_write_sample()` y `reset_sd_file()` administran el CSV y los acumuladores. `display_task()` alterna información en la pantalla; `uart_receive_task()` atiende la entrada UART. El temporizador y la interrupción del anemómetro permiten calcular la velocidad del viento. `app_main()` inicializa los subsistemas y crea las tareas.

## Conexiones según el fuente

| Elemento | Configuración |
| --- | --- |
| Bus I²C | SDA GPIO5, SCL GPIO6. |
| OLED SSD1306 | Dirección `0x3C`, 128 × 64 píxeles. |
| BME680 | Dirección `0x77`. |
| RTC DS3231 | Dirección `0x68`. |
| Sensor de velocidad del viento | GPIO7; factor `ESCALA_VELOCIDAD = 0.47`. |
| microSD por SPI | MOSI GPIO23, MISO GPIO22, SCLK GPIO21, CS GPIO3. |
| UART hacia ESP3 | UART1, TX GPIO15, RX GPIO9, 9600 bit/s, 8N1. |
| LED | GPIO2. |
| Entrada ESP-NOW | Canal 1. |

El programa contiene la constante `ESP1_MAC`; consérvela o ajústela para identificar su montaje. La función receptora comprueba el tamaño del mensaje, pero no aplica un filtro de autorización por esa MAC. La placa no necesita conectarse a un punto de acceso para el flujo ESP-NOW de esta versión.

## Condiciones de operación que deben conocerse

- El estado de ESP-NOW se considera conectado cuando la última recepción ocurrió hace menos de tres segundos.
- La tarea consulta sensores aproximadamente cada segundo. El JSON se emite después de 10 muestras acumuladas, siempre que la lectura del BME680 sea válida y el archivo de microSD esté abierto. Un fallo de esos recursos puede detener la emisión de JSON.
- `/sd/data.csv` se abre en modo de escritura al iniciar y se vuelve a crear después de cada bloque de 10 muestras. El montaje tiene `format_if_mount_failed = true`. Utilice una tarjeta dedicada y respalde cualquier contenido necesario antes de las pruebas.
- El RTC aporta el `timestamp`. Existen funciones para ajustar fecha y hora, pero `app_main()` no las invoca automáticamente. Verifique previamente la fecha y hora del DS3231.
- Se conservan los factores y desplazamientos de ajuste del prototipo, entre ellos temperatura, humedad, presión y voltaje. Revise su calibración para otro montaje.

La microSD actúa como almacenamiento temporal del bloque de muestras; este código no implementa una cola confirmada por AWS ni conserva un historial indefinido en el CSV.

## Compilar y cargar

Esta carpeta conserva su nombre descriptivo con espacios. Antes de compilar, prepare una copia con ruta corta desde el proyecto RStudio de la raíz:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP2")
```

El comando muestra el destino. Entre en él desde una terminal con **ESP-IDF 5.4.1** y ejecute:

```sh
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Sustituya COM5 por el puerto de su placa. Para ESP3, prepare primero la configuración local y sus certificados según este README. La copia de compilación no sustituye el fuente que se versiona: realice las modificaciones en esta carpeta y vuelva a prepararla.

Consulte [Configuración y compilación](../../docs/CONFIGURACION_Y_COMPILACION.md).

La biblioteca Wi-Fi auxiliar tiene una plantilla `components/wifi_connect_lib/include/config_local.example.h`. Solo si va a utilizar esa biblioteca para conectarse a un punto de acceso, cópiela como `config_local.h` en la misma carpeta y complete sus valores. Ese archivo local queda excluido de Git. No es necesario para el flujo actual de `main7.c`.
