# ESP2

El proyecto está en la carpeta **Firmware nodo 2 - Variables ambientables e integración con ESP1**, con el nombre suministrado por el autor.

- [Abrir el proyecto](Firmware%20nodo%202%20-%20Variables%20ambientables%20e%20integraci%C3%B3n%20con%20ESP1/).
- [Leer las instrucciones del nodo](Firmware%20nodo%202%20-%20Variables%20ambientables%20e%20integraci%C3%B3n%20con%20ESP1/README.md).
- [Abrir el fuente principal](Firmware%20nodo%202%20-%20Variables%20ambientables%20e%20integraci%C3%B3n%20con%20ESP1/main/main7.c).

Para compilar una copia local con ruta compatible con ESP-IDF, abra `Tesis_Especializacion_TICs.Rproj` en la raíz y ejecute:

```r
source("preparar_firmware.R", encoding = "UTF-8")
preparar_firmware("ESP2")
```

La copia de compilación se genera en `compilacion_local/`, que queda fuera de Git. Edite siempre el fuente de la carpeta descriptiva y prepare otra copia cuando cambie el código.
