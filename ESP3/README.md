# ESP3

El proyecto está en la carpeta **Firmware nodo 3 e integración con AWS_IoT_Core**, con el nombre suministrado por el autor.

- [Abrir el proyecto](Firmware%20nodo%203%20e%20integraci%C3%B3n%20con%20AWS_IoT_Core/).
- [Leer las instrucciones del nodo](Firmware%20nodo%203%20e%20integraci%C3%B3n%20con%20AWS_IoT_Core/README.md).
- [Abrir el fuente principal](Firmware%20nodo%203%20e%20integraci%C3%B3n%20con%20AWS_IoT_Core/main/main2.c).

Para compilar una copia local con ruta compatible con ESP-IDF, abra `Tesis_Especializacion_TICs.Rproj` en la raíz y ejecute:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP3")
```

La copia de compilación se genera en `compilacion_local/`, que queda fuera de Git. Edite siempre el fuente de la carpeta descriptiva y prepare otra copia cuando cambie el código.

## Guía de conexión con AWS IoT Core

Consulte la [guía de reproducción ESP3--AWS IoT Core](../docs/GUIA_CONEXION_ESP3_AWS.md), vinculada con el Anexo C. Reúne la configuración del entorno, los recursos de AWS, la compilación, las comprobaciones y los límites identificados en la lectura del código.
