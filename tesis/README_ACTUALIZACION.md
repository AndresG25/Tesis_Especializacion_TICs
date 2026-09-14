# Actualización capítulo 2 y bibliografía

Copiar estos archivos en:

C:\Users\andre\TESIS-ESPECIALIZACION-TICS

## Archivos

- `02-marco-referencia.qmd`: reemplaza el capítulo 2 actual.
- `bibliografia.bib`: reemplaza el archivo actual; conserva la bibliografía existente y agrega las referencias nuevas del capítulo 2.
- `bibliografia_append_capitulo2.bib`: alternativa por si prefieres pegar solo las nuevas entradas al final del `bibliografia.bib` local.
- `fig/arquitectura_referencia_redisenada.png`: imagen de arquitectura para incluir en el capítulo 2.

## Render

```cmd
cd /d C:\Users\andre\TESIS-ESPECIALIZACION-TICS
quarto render --to pdf
```

## Nota

La bibliografía no aparecía porque el documento tenía archivo `.bib`, pero casi no tenía citas activas del tipo `[@clave]` en los capítulos renderizados.
