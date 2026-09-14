# ESP1 — Adquisición de campo

Proyecto ESP-IDF para ESP32-C6, asociado al **Anexo A** de la tesis. Carpeta original: `clase-UART`. El archivo [main/main7.c](main/main7.c) conserva íntegramente el código recibido.

## Archivos y funciones

| Archivo o elemento | Función |
| --- | --- |
| `main/main7.c` | Inicialización, adquisición y envío de las variables de campo. |
| `components/minmea/` | Biblioteca de análisis de sentencias NMEA del receptor GPS. |
| `CMakeLists.txt` y `main/CMakeLists.txt` | Registro del proyecto, fuente principal y dependencias de ESP-IDF. |
| `sdkconfig.defaults` | Configuración recibida, con objetivo ESP32-C6 y flash de 2 MB. |

`gps_task()` lee el GPS por UART y obtiene posición y estado de fijación. `voltage_task()` adquiere la señal mediante ADC y aplica el cálculo de voltaje y el filtro incluidos en el prototipo. El sensor Hall y `timer_callback()` calculan las RPM con una ventana de un segundo y dos pulsos por revolución configurados. `espnow_task()` envía la estructura compartida aproximadamente cada dos segundos a ESP2. `app_main()` inicia los periféricos y las tareas.

## Conexiones según el fuente

| Elemento | Configuración |
| --- | --- |
| GPS | UART1, RX GPIO20, TX GPIO21, 9600 bit/s. |
| Voltaje | ADC1, `ADC_CHANNEL_0`; verificar el pin correspondiente en la placa utilizada y su circuito de acondicionamiento. |
| Sensor Hall de RPM | GPIO22. |
| LED | GPIO2. |
| Comunicación con ESP2 | ESP-NOW, canal 1, par configurado sin cifrado ESP-NOW. |

Antes de cargar el programa, ajuste `MAC_ESP2` en `main/main7.c` a la MAC de la interfaz STA de su ESP2. La dirección conservada corresponde al montaje recibido. Los dos nodos deben usar el mismo canal y mantener idéntica la definición de `mensaje_t`.

No conecte la salida del generador directamente al ADC: el código presupone el circuito de medición y acondicionamiento descrito en la tesis. Los factores de conversión y filtrado se conservan; su presencia no sustituye la calibración del montaje.

## Compilar y cargar

Esta carpeta conserva su nombre descriptivo con espacios. Antes de compilar, prepare una copia con ruta corta desde el proyecto RStudio de la raíz:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP1")
```

El comando muestra el destino. Entre en él desde una terminal con **ESP-IDF 5.4.1** y ejecute:

```sh
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Sustituya COM5 por el puerto de su placa. Para ESP3, prepare primero la configuración local y sus certificados según este README. La copia de compilación no sustituye el fuente que se versiona: realice las modificaciones en esta carpeta y vuelva a prepararla.

Consulte [Configuración y compilación](../../docs/CONFIGURACION_Y_COMPILACION.md).
