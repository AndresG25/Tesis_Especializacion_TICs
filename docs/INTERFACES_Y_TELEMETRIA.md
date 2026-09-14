# Interfaces y telemetría

Esta descripción procede de los fuentes adjuntos; no modifica sus protocolos ni sus cálculos.

## ESP1 → ESP2: estructura ESP-NOW

Ambos nodos definen `mensaje_t` con el mismo orden de campos:

| Campo | Tipo en C | Significado |
| --- | --- | --- |
| `latitud` | `float` | Latitud calculada a partir del GPS. |
| `longitud` | `float` | Longitud calculada a partir del GPS. |
| `voltaje_ac` | `float` | Resultado del cálculo de voltaje de ESP1. |
| `rpm` | `float` | Velocidad de giro calculada. |
| `gps_fix` | `bool` | Estado de fijación GPS. |

ESP1 transmite los bytes de la estructura con `sizeof(mensaje_t)`. ESP2 comprueba que el tamaño recibido coincida. No es una serialización portable entre arquitecturas: mantenga la definición y el entorno compatibles en ambos ESP32-C6. El canal configurado es 1 y el par de ESP1 tiene `.encrypt = false`.

## ESP2 → ESP3: conexión UART

| Señal | Conexión |
| --- | --- |
| Telemetría | ESP2 TX GPIO15 → ESP3 RX GPIO22. |
| Retorno, si se utiliza | ESP3 TX GPIO15 → ESP2 RX GPIO9. |
| Referencia | GND de ESP2 ↔ GND de ESP3. |
| Formato | UART1, 9600 bit/s, 8 bits de datos, sin paridad, 1 bit de parada, sin control de flujo. |

No conecte TX con TX. Use niveles lógicos compatibles con las placas ESP32-C6.

## JSON emitido por ESP2

Se proporciona [telemetria_ejemplo.json](telemetria_ejemplo.json) con **valores ficticios**, exclusivamente para ilustrar el formato. No es una medición ni evidencia de las pruebas de la tesis.

| Clave | Tipo JSON | Contenido del código |
| --- | --- | --- |
| `timestamp` | Texto | Fecha y hora del DS3231: `DD/MM/AA HH:MM:SS`. |
| `wind_speed` | Número | Promedio del bloque para velocidad del viento. |
| `voltage` | Número | Promedio de voltaje; puede utilizar `-1` como indicador según la lógica del acumulador. |
| `temperature` | Número | Promedio de temperatura con el ajuste aplicado en el prototipo. |
| `humidity` | Número | Promedio de humedad con el ajuste aplicado. |
| `pressure` | Número | Promedio de presión con el ajuste aplicado. |
| `altitude` | Número | Promedio de la altitud estimada a partir de presión. |
| `lat`, `lon` | Número | Promedio de muestras GPS consideradas válidas; cero si no hay muestras válidas en el bloque. |
| `rpm` | Número | Promedio de RPM; puede utilizar `-1` según la lógica del acumulador. |
| `esp_now_connected` | Booleano | Estado calculado en ESP2 al formar el mensaje. |
| `memory_usage` | Número | Porcentaje estimado de memoria utilizada en ESP2. |

El mensaje termina con un salto de línea. El `timestamp` no es ISO 8601, no incluye zona horaria y utiliza año de dos dígitos. Su interpretación debe coincidir con la transformación aplicada en AWS. Los promedios se forman con 10 muestras; las condiciones de adquisición pueden alargar el intervalo real de emisión.

ESP3 busca las llaves de apertura y cierre para reconstruir el mensaje dentro de sus buffers. No agrega `clientId`, no transforma el `timestamp` y no ejecuta una validación completa del esquema. Si las reglas de AWS agregan identificadores o normalizan fechas, esas operaciones pertenecen al despliegue externo.

## ESP3 → AWS IoT Core

La conexión utiliza el host configurado en `AWS_IOT_ENDPOINT`, transporte TLS y puerto 8883. El cliente se identifica con `AWS_IOT_CLIENT_ID`, usa certificado y llave privada y publica en `AWS_IOT_TOPIC`, con QoS 1 y sin retención. La política del certificado debe permitir las operaciones correspondientes al despliegue utilizado.

QoS 1 no sustituye una cola local persistente: en esta implementación los mensajes UART recibidos mientras MQTT está desconectado no se almacenan para reintento posterior.
