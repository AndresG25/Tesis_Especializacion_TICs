# Configuración y compilación de los nodos

Las rutas vigentes están en [RUTAS_PROYECTOS.csv](RUTAS_PROYECTOS.csv). ESP1 y ESP2 conservan ESP-IDF 5.4.1 como referencia, y ESP3 conserva 5.4.3. Los tres objetivos son ESP32-C6 con flash configurada de 2 MB. Los archivos `sdkconfig.defaults` conservan las configuraciones recibidas.

## Ubicación y preparación

| Nodo | Proyecto dentro de su carpeta |
| --- | --- |
| ESP1 | Firmware nodo ESP1 - Variables Generador |
| ESP2 | Firmware nodo 2 - Variables ambientables e integración con ESP1 |
| ESP3 | Firmware nodo 3 e integración con AWS_IoT_Core |

Los nombres se conservan literalmente. Espressif indica que [el sistema de compilación ESP-IDF no admite espacios en las rutas de los proyectos](https://docs.espressif.com/projects/esp-idf/en/v5.5.4/esp32c5/get-started/linux-macos-setup.html). Por eso se proporciona una copia de compilación con ruta corta.

Abra el proyecto RStudio de la raíz y ejecute:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP1")
```

Seleccione ESP2 o ESP3 según corresponda. Se crea una carpeta nueva dentro de `compilacion_local/` y se muestra su ubicación. Si la ruta del repositorio contiene espacios o tildes, indique un destino sin ellos:

```r
preparar_firmware("ESP1", "C:/esp_tesis/ESP1")
```

El destino debe ser nuevo. El script conserva los nombres de los archivos, copia el código y comprueba sus huellas. No borra destinos existentes y no ejecuta la compilación. Si ya completó credenciales en los archivos locales excluidos de Git, estas también se copian al destino para poder compilar. Mantenga ese destino local y fuera de la selección de archivos para GitHub.

## Configurar antes de preparar la copia

| Nodo | Acción |
| --- | --- |
| ESP1 | Ajustar `MAC_ESP2` en `main/main7.c` a la MAC de la placa receptora y revisar conexiones. |
| ESP2 | Revisar sensores, RTC y microSD. El flujo actual de `main7.c` no requiere conexión a un punto de acceso. |
| ESP3 | Copiar `main/config_local.example.h` como `main/config_local.h`, completar Wi-Fi/AWS y añadir el certificado de dispositivo y su llave privada en `main/certs/`. |

Estas rutas son relativas a la carpeta descriptiva del nodo. El endpoint de ESP3 es un nombre de host, sin `mqtts://`. El tópico y el identificador del cliente deben coincidir con su despliegue de AWS. Los README de cada nodo explican pines, funciones y condiciones de operación.

## Compilar y cargar

Entre en la copia creada desde una terminal con la versión correspondiente de ESP-IDF activada:

```sh
idf.py --version
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Sustituya COM5 por el puerto de su placa. Cada nodo se compila y carga por separado. Para salir del monitor, pulse Ctrl+]. Consulte la [guía oficial de ESP-IDF 5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32c6/get-started/index.html) para el entorno de esa versión.

Edite siempre los fuentes dentro de ESP1, ESP2 y ESP3, que son los que Git registra. Cuando cambie el código, prepare una copia nueva. Si modifica directamente la copia de compilación, esos cambios no pasan automáticamente al repositorio.

## Condiciones del prototipo conservadas

ESP2 recrea el CSV de la microSD tras cada bloque de 10 muestras y permite formatear la tarjeta si falla el montaje. La generación de JSON depende de la lectura válida del BME680 y del archivo de microSD abierto. Utilice una tarjeta dedicada y revise la fecha y hora del RTC.

ESP3 no conserva una cola persistente de mensajes durante la desconexión MQTT. Su función `sync_time()` no consulta un servidor SNTP. Estas condiciones ya pertenecían al firmware recibido y no se modificaron en esta integración.

La preparación para Git/RStudio y las comprobaciones de copia no sustituyen una compilación con ESP-IDF ni una prueba física del sistema.
