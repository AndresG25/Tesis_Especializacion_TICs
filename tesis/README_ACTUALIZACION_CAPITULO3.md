# Actualización capítulo 3 - Arquitectura de hardware IoT Edge

Copiar estos archivos en:

`C:\Users\andre\TESIS-ESPECIALIZACION-TICS`

## Archivos incluidos

- `03-arquitectura-hardware-iot-edge.qmd`: reemplaza el capítulo 3 actual.
- `fig/arquitectura_iot_edge_esp32.png`: figura nueva para el capítulo 3.
- `bibliografia_append_capitulo3.bib`: entradas nuevas para pegar al final de `bibliografia.bib` si ya tienes la bibliografía actualizada del capítulo 2.
- `bibliografia_capitulo2_mas_capitulo3.bib`: alternativa para reemplazar directamente `bibliografia.bib` si usaste la actualización anterior del capítulo 2.

## Recomendación práctica

1. Reemplaza `03-arquitectura-hardware-iot-edge.qmd`.
2. Copia la imagen dentro de la carpeta `fig`.
3. Agrega el contenido de `bibliografia_append_capitulo3.bib` al final de tu `bibliografia.bib`, o reemplaza `bibliografia.bib` por `bibliografia_capitulo2_mas_capitulo3.bib`.
4. Renderiza:

```cmd
cd /d C:\Users\andre\TESIS-ESPECIALIZACION-TICS
quarto render --to pdf
```

## Nota técnica

El capítulo se construyó con base en los archivos activos revisados:

- `clase-UART/main/main7.c`
- `Proyecto_Integrador/main/main7.c`
- `AWS_IoT_Core/main/main2.c`

Se documentan explícitamente limitaciones y mejoras: cifrado ESP-NOW, calibración, gestión de secretos, sincronización SNTP real y validación robusta del JSON.
