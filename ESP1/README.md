# ESP1

El proyecto está en la carpeta **Firmware nodo ESP1 - Variables Generador**, con el nombre suministrado por el autor.

- [Abrir el proyecto](Firmware%20nodo%20ESP1%20-%20Variables%20Generador/).
- [Leer las instrucciones del nodo](Firmware%20nodo%20ESP1%20-%20Variables%20Generador/README.md).
- [Abrir el fuente principal](Firmware%20nodo%20ESP1%20-%20Variables%20Generador/main/main7.c).

Para compilar una copia local con ruta compatible con ESP-IDF, abra `Tesis_Especializacion_TICs.Rproj` en la raíz y ejecute:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP1")
```

La copia de compilación se genera en `compilacion_local/`, que queda fuera de Git. Edite siempre el fuente de la carpeta descriptiva y prepare otra copia cuando cambie el código.
