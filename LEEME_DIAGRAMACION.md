# Entrega con correcciones de diagramación

El PDF de esta entrega ya está renderizado y tiene **138 páginas**. Está en
`tesis/output/Desarrollo-de-una-arquitectura-escalable-de-Big-Data-en-AWS-alimentada-mediante-un-sistema-de-IoT.pdf`.

Esta entrega aplica únicamente los cuatro ajustes de diagramación solicitados.
Los documentos anteriores de revisión se conservan tal como llegaron; las notas
que allí indican que faltaba renderizar corresponden a la entrega anterior.
Para la revisión actual, consulte `control_diagramacion/GUIA_REVISION.md`.

## Abrir el proyecto

1. Extraiga todo el ZIP en una carpeta nueva.
2. Abra `tesis/tesis.Rproj` en RStudio.
3. Para volver a generar el PDF, ejecute en la consola:

```r
system("quarto render --to pdf")
```

El comando debe ejecutarse desde la carpeta `tesis`, donde está `_quarto.yml`.
El proyecto conserva su configuración, motor de PDF, bibliografía y preámbulo.
Para consultar el resultado entregado no es necesario volver a renderizar.

## Archivos de esta corrección

- `tesis/`: proyecto completo y PDF actualizado.
- `tesis/fig/revision_director/`: las cuatro figuras en PNG, SVG editable y PDF vectorial.
- `control_diagramacion/`: guía de revisión, diferencias de fuentes y verificaciones.
- `control_cambios/` y `referencia_original/`: material anterior íntegro.

El comentario 7 no se intervino: la observación original sigue incompleta.
