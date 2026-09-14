# Sistema IoT para monitoreo eólico con ESP32-C6 y AWS

Código de apoyo de la tesis **«Desarrollo de una arquitectura escalable de Big Data en AWS alimentada mediante un sistema de IoT»**.

**Autor:** Carlos Andrés Gómez Flórez  
**Contacto:** [andres25@utp.edu.co](mailto:andres25@utp.edu.co)  
**Repositorio:** [AndresG25/Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs)

## Alcance de esta entrega

Se incluyen tres proyectos ESP-IDF independientes: adquisición de campo, integración local y pasarela hacia AWS IoT Core. La entrega contiene sus fuentes, componentes locales, configuración base y documentación de montaje y uso.

Las funciones Lambda, las reglas y recursos de AWS y el dashboard React/Vite **no están incluidos en los archivos recibidos para esta entrega**. Su relación con el firmware y los elementos por incorporar se describen en [Alcance y trazabilidad](docs/ALCANCE_Y_TRAZABILIDAD.md). La disponibilidad de estos módulos no debe inferirse del título de la tesis.

El repositorio se mantiene privado durante la revisión académica. Para solicitar acceso o consultar el material, escriba al correo del autor e indique su usuario de GitHub.

## Carpetas y relación con los anexos

| Carpeta | Anexo de la tesis | Fuente principal | Responsabilidad |
| --- | --- | --- | --- |
| [ESP1](ESP1/README.md) | A | [main/main7.c](ESP1/main/main7.c) | GPS, voltaje y RPM; envío binario mediante ESP-NOW a ESP2. |
| [ESP2](ESP2/README.md) | B | [main/main7.c](ESP2/main/main7.c) | Integra ESP-NOW y sensores locales, consulta el RTC, usa microSD y OLED, y transmite JSON por UART. |
| [ESP3](ESP3/README.md) | C | [main/main2.c](ESP3/main/main2.c) | Recibe UART y publica los mensajes en AWS IoT Core mediante Wi-Fi y MQTT sobre TLS. |
| [docs](docs/) | Apoyo de A–C | Guías y ejemplo de telemetría | Configuración, interfaces, subida a GitHub y límites de la entrega. |

## Flujo de información

| Origen → destino | Interfaz | Contenido y comportamiento del código |
| --- | --- | --- |
| Sensores → ESP1 | UART, ADC y GPIO | Posición GPS, señal de voltaje y pulsos para RPM. |
| ESP1 → ESP2 | ESP-NOW, canal 1 | Estructura `mensaje_t`; envío aproximadamente cada 2 segundos. |
| Sensores locales → ESP2 | I²C y GPIO | Temperatura, humedad, presión y pulsos de velocidad del viento; fecha y hora del RTC. |
| ESP2 → ESP3 | UART a 9600 bit/s, 8N1 | JSON tras acumular 10 muestras válidas con la microSD abierta. |
| ESP3 → AWS IoT Core | MQTT/TLS, puerto 8883 | Publicación con QoS 1, sin mensaje retenido, en el tópico configurado localmente. |

El formato exacto de los mensajes y las conexiones entre placas están en [Interfaces y telemetría](docs/INTERFACES_Y_TELEMETRIA.md).

## Entorno y puesta en marcha

Los archivos `sdkconfig` y los metadatos de compilación de los proyectos adjuntos identifican estas versiones:

| Proyecto | Objetivo | ESP-IDF de referencia | Flash configurada |
| --- | --- | --- | --- |
| ESP1 | `esp32c6` | 5.4.1 | 2 MB |
| ESP2 | `esp32c6` | 5.4.1 | 2 MB |
| ESP3 | `esp32c6` | 5.4.3 | 2 MB |

1. Consulte el README de cada nodo y [Configuración y compilación](docs/CONFIGURACION_Y_COMPILACION.md).
2. En ESP1, ajuste `MAC_ESP2` a la dirección de su placa receptora y confirme las conexiones.
3. En ESP3, prepare `main/config_local.h` y sus certificados de dispositivo **antes** de ejecutar la configuración de ESP-IDF.
4. Desde la terminal de ESP-IDF de la versión correspondiente, entre en la carpeta del nodo y ejecute:

```sh
idf.py --version
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

`COM5` es un ejemplo; utilice el puerto de su placa. En Linux puede tener un nombre como `/dev/ttyACM0`. Los tres nodos se compilan y cargan por separado. Para salir del monitor, pulse `Ctrl+]`.

`sdkconfig.defaults` conserva la configuración completa recibida de cada proyecto como base para un directorio limpio. Los componentes locales se incluyen en `components/`; las tablas de particiones propias de ESP2 y ESP3 también se conservan.

## Antes de utilizar el prototipo

La preparación de este repositorio conserva los algoritmos de los tres nodos. Las modificaciones del código se limitan a separar las credenciales de Wi-Fi/AWS en archivos locales y a comprobar que los archivos privados de ESP3 existan antes de compilar.

El código de ESP2 vuelve a crear `/sd/data.csv` al iniciar y tras cada bloque de 10 muestras. Además, su configuración permite formatear la microSD si falla el montaje. Use una tarjeta dedicada al prototipo; esta implementación no constituye un archivo histórico permanente. La generación de JSON depende de la lectura del BME680 y de que el archivo de la microSD esté abierto.

ESP3 no implementa una cola persistente para retransmitir los mensajes recibidos cuando MQTT está desconectado. Su función `sync_time()` tampoco consulta un servidor de hora; la hora utilizada por TLS debe verificarse durante la puesta en marcha. Estas condiciones se documentan con mayor detalle en los README de los nodos.

## Publicación y revisión

Siga [Subir a GitHub](docs/SUBIR_A_GITHUB.md) para cargar el contenido de esta carpeta en el repositorio ya creado. No suba los ZIP originales: contienen compilaciones y configuración privada.

Se verificaron la estructura del paquete, los enlaces locales, las exclusiones de Git y la preservación de los fuentes fuera de los cambios de configuración registrados. **Esta preparación no incluye una nueva compilación con ESP-IDF ni una prueba física del sistema.** Consulte [Alcance y trazabilidad](docs/ALCANCE_Y_TRAZABILIDAD.md).

## Cita y componentes de terceros

El archivo [CITATION.cff](CITATION.cff) identifica al autor y al repositorio. Al citar una revisión concreta, añada su identificador de commit después de publicarla. Los avisos de los componentes recibidos se conservan; consulte [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) y [LICENSING.md](LICENSING.md).
