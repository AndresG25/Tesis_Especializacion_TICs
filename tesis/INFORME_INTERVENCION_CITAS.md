# Informe de intervención controlada de la tesis

Fecha de generación: 2026-06-06

## Alcance aplicado

Se trabajó sobre una copia del proyecto Quarto recibido en `tesis.zip`. Se mantuvo la estructura de capítulos y anexos definida en `_quarto.yml`.

## Archivos intervenidos

1. `02-marco-referencia.qmd`
   - Se sustituyó el capítulo por la versión revisada del archivo `Marco_referencia_revision_final_citas_verificadas.docx`.
   - Se conservó la figura de arquitectura de referencia existente del proyecto.
   - Se reconstruyó la tabla de lectura por capas en sintaxis Markdown/Quarto.
   - Se retiró la lista manual de referencias del DOCX para que el proyecto use `bibliografia.bib` y `references.qmd`.
   - Se añadieron citas Quarto/BibTeX al cierre de cada párrafo del capítulo.

2. `03-arquitectura-hardware-iot-edge.qmd`
   - No se reescribió el contenido humanizado.
   - Se añadieron citas Quarto/BibTeX a los párrafos que no tenían al menos dos referencias formales.
   - Se marcaron palabras clave técnicas en negrita mediante sintaxis Markdown.

3. `04-arquitectura-software-aws.qmd`
   - No se reescribió el contenido humanizado.
   - Se verificó y reforzó el soporte bibliográfico por párrafo.
   - Se marcaron palabras clave técnicas en negrita mediante sintaxis Markdown.

4. `bibliografia.bib`
   - Se consolidaron referencias del proyecto actual, anteproyecto, anexos bibliográficos existentes y la referencia BibTeX de la tesis de maestría.
   - Se agregó una clave segura sin tildes para la tesis de maestría: `gomez_prototipado_2024`.
   - Se evitaron claves duplicadas cuando ya existía una entrada equivalente.

## Criterio de intervención

- No se hizo reescritura estilística de los capítulos 3 y 4.
- La intervención consistió en insertar citas, conservar la estructura seccionada del proyecto y aplicar énfasis tipográfico a términos clave.
- La actualización del capítulo 2 se hizo con base en el documento revisado que fue entregado como insumo adicional.

## Estadísticas de proceso

- Capítulo 2 reconstruido desde DOCX: 38 párrafos con citas asignadas manualmente.
- Capítulo 3: 96 bloques de texto reforzados con citas formales.
- Capítulo 4: 3 bloques de texto reforzados con citas formales.
- Énfasis aplicado en capítulos: cap. 2 = 39 bloques, cap. 3 = 64 bloques, cap. 4 = 35 bloques.
- Total de entradas bibliográficas consolidadas en `bibliografia.bib`: 89 referencias.

## Validación interna de citas

- Capítulo 2: 38 párrafos textuales revisados; todos quedaron con al menos dos claves bibliográficas Quarto/BibTeX.
- Capítulo 3: 98 párrafos textuales revisados; todos quedaron con al menos dos claves bibliográficas Quarto/BibTeX.
- Capítulo 4: 32 párrafos textuales revisados; todos quedaron con al menos dos claves bibliográficas Quarto/BibTeX.
- Claves bibliográficas faltantes detectadas: 0.
- Se ejecutó parseo sintáctico con Pandoc sobre los capítulos 2, 3 y 4.

## Advertencia de validación

En este entorno no está instalado el ejecutable `quarto`, por lo que no fue posible ejecutar `quarto render`. Sí se realizaron validaciones estructurales internas: existencia de archivos, revisión de claves BibTeX usadas en los capítulos, prueba de parseo con Pandoc para los capítulos 2, 3 y 4, y preservación de la estructura del libro. La validación interna no detectó claves bibliográficas faltantes; los identificadores `@fig-*` y `@tbl-*` corresponden a referencias cruzadas de Quarto, no a bibliografía.

## Comandos sugeridos para renderizar

Desde la carpeta raíz del proyecto:

```bash
quarto render
```

Para salidas específicas:

```bash
quarto render --to html
quarto render --to pdf
quarto render --to docx
```
