# Versión integral V2 de la tesis Quarto

Este paquete contiene una versión actualizada y más completa del proyecto Quarto.

## Archivos principales

- `_quarto.yml`: configuración del libro.
- `tex/frontmatter.tex`: portada, nota de aceptación, resumen, abstract e índices.
- `tex/preamble.tex`: ajustes LaTeX del documento.
- `index.qmd`: capítulo 1.
- `02-marco-referencia.qmd`: capítulo 2.
- `03-arquitectura-hardware-iot-edge.qmd`: capítulo 3 corregido y ampliado.
- `04-arquitectura-software-aws.qmd`: capítulo 4 limpio con infografía final.
- `05-pruebas-validacion.qmd`: capítulo 5 desarrollado.
- `06-analisis-costos-resultados.qmd`: capítulo 6 desarrollado.
- `07-conclusiones-trabajos-futuros.qmd`: capítulo 7 desarrollado.
- `references.qmd`: sección de referencias.
- `bibliografia.bib`: bibliografía limpia y unificada.
- `anexos/`: placeholders para anexos de código.
- `fig/`: figuras principales del documento.

## Instrucciones

Copiar el contenido de esta carpeta en:

`C:\Users\andre\TESIS-ESPECIALIZACION-TICS`

Luego ejecutar:

```cmd
cd /d C:\Users\andre\TESIS-ESPECIALIZACION-TICS
quarto render --to pdf
```

Antes de copiar, se recomienda respaldar la carpeta actual.
