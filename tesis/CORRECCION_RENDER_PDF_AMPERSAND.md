# Corrección para render PDF

Se corrigieron problemas detectados durante `quarto render --to pdf`:

1. En `bibliografia.bib` se escaparon los caracteres `&` que LaTeX interpreta como separadores de alineación. El caso que detenía la compilación era el journal `IEEE Communications Surveys \& Tutorials`.
2. Se cambió la entrada `@standard{iso_ieee_8802_11_2018}` a `@misc{iso_ieee_8802_11_2018}` porque `plainnat.bst` no reconoce el tipo `@standard`.
3. Se simplificó y corrigió la entrada `gomez_prototipado_2024` como `@MASTERSTHESIS`, agregando el campo obligatorio `school` para evitar la advertencia de BibTeX.
4. En `04-arquitectura-software-aws.qmd` se agregó una línea en blanco después de la figura principal para ayudar a Quarto/Pandoc a reconocer correctamente el identificador `#fig-cap4-arquitectura-software-aws`.

No se modificó el contenido académico de los capítulos.
