# Configuración y compilación

## Entorno

Cada carpeta `ESP1`, `ESP2` y `ESP3` es un proyecto ESP-IDF completo e independiente. No existe un único comando de compilación en la raíz que programe las tres placas.

Active **ESP-IDF 5.4.1 para ESP1 y ESP2**, y **ESP-IDF 5.4.3 para ESP3**, con sus herramientas para ESP32-C6. Estas son las versiones identificadas en la entrega original. Consulte la [guía oficial de instalación de ESP-IDF 5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32c6/get-started/index.html) y seleccione la versión correspondiente a cada proyecto. No se ha verificado la migración a versiones posteriores.

En Windows puede abrir la terminal de ESP-IDF de la instalación elegida o utilizar la extensión de Espressif en VS Code con esa misma versión. En Linux/macOS active el entorno instalado antes de ejecutar `idf.py`. Compruebe siempre:

```sh
idf.py --version
```

Los `sdkconfig.defaults` de esta entrega son copias completas de los `sdkconfig` recibidos. Conserve estos archivos como referencia versionada; ESP-IDF creará un `sdkconfig` local durante la configuración. Las placas están configuradas con flash de 2 MB; revise este dato si utiliza un hardware diferente.

## Configurar cada nodo

| Nodo | Acción previa |
| --- | --- |
| ESP1 | Ajustar `MAC_ESP2` en `main/main7.c` y revisar GPS, ADC y sensor Hall. |
| ESP2 | Revisar direcciones I²C, RTC, anemómetro y microSD. El enlace actual no requiere credenciales de un punto de acceso. |
| ESP3 | Crear `main/config_local.h`, completar Wi-Fi/AWS y añadir el certificado de dispositivo y su llave privada. |

Para preparar ESP3, desde la raíz del repositorio en Git Bash o una terminal que admita `cp`:

```sh
cp ESP3/main/config_local.example.h ESP3/main/config_local.h
```

También puede hacer la copia con el explorador de archivos. Edite la copia, no la plantilla; use los valores de su entorno original. Coloque los dos archivos de identidad con los nombres exactos indicados en [ESP3/main/certs/README.md](../ESP3/main/certs/README.md). El endpoint debe contener solo el nombre de host y el tópico debe corresponder al utilizado por las reglas de AWS.

El repositorio conserva el certificado público raíz y las dependencias de ESP-IDF. No contiene la configuración completa de IAM, las reglas IoT ni scripts para crear automáticamente los recursos en AWS.

## Configurar, compilar y cargar

Ejemplo para ESP1, partiendo de la raíz del repositorio:

```sh
cd ESP1
idf.py set-target esp32c6
idf.py build
idf.py -p COM5 flash monitor
```

Repita dentro de ESP2 y ESP3, con su versión de ESP-IDF, configuración local y puerto. `COM5` es un ejemplo de Windows; seleccione el puerto real. Salga del monitor con `Ctrl+]`.

Para cambiar opciones, utilice `idf.py menuconfig`. Si necesita compartir una configuración revisada, genere sus valores por defecto con `idf.py save-defconfig` y revise el cambio antes de actualizar `sdkconfig.defaults`. No suba `build/`, `sdkconfig.old` ni archivos de configuración privada.

## Comprobación funcional después de cargar

1. En ESP2, compruebe el inicio de OLED, BME680, RTC y microSD. La fecha del RTC debe corresponder a la prueba.
2. En ESP1, compruebe los mensajes de adquisición y envío ESP-NOW. En ESP2, confirme la recepción y el estado de conexión.
3. Confirme que ESP2 genera un JSON después de 10 muestras válidas y que ESP3 lo recibe por UART. Esta emisión depende del BME680 y del archivo de microSD.
4. En ESP3, confirme Wi-Fi, conexión MQTT/TLS y publicaciones. Revise la hora del dispositivo si hay errores de validación del certificado.
5. En el despliegue autorizado de AWS, compruebe que el tópico recibe el mensaje y que las reglas lo procesan con el esquema esperado.

Esta secuencia es una guía para la próxima prueba. No constituye un registro de pruebas ejecutadas durante la preparación del repositorio.

## Datos locales y Git

Los archivos `config_local.h`, las llaves privadas, los certificados de dispositivo y los resultados de compilación están excluidos por `.gitignore`. Esa exclusión se aplica al uso de Git; arrastrar archivos privados directamente a la web de GitHub no queda protegido por esas reglas. Suba únicamente el contenido del paquete limpio de entrega y revise la selección antes de confirmar.
