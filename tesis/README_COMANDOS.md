# Plantilla Quarto para tesis de especialización TICs

## Carpeta destino recomendada

Copiar todo el contenido de esta carpeta en:

```cmd
C:\Users\andre\TESIS-ESPECIALIZACION-TICS
```

## Comandos de trabajo

Abrir RStudio y luego:

```cmd
cd /d C:\Users\andre\TESIS-ESPECIALIZACION-TICS
quarto render --to pdf
```

Para compilar todos los formatos:

```cmd
quarto render
```

Para vista previa HTML:

```cmd
quarto preview
```

## Figuras

Copiar las figuras reales del proyecto Overleaf en la carpeta `fig/`.

Nombres sugeridos:

```text
fig/logo_utp.png
fig/arquitectura_general.png
fig/arquitectura_esp32.png
fig/dashboard.png
```

## Migración desde Overleaf

El archivo original queda preservado en:

```text
legacy/main_overleaf_original.tex
```

La bibliografía original queda en:

```text
bibliografia.bib
```

## Recomendación

No editar `legacy/main_overleaf_original.tex`. Usarlo solo como fuente de consulta. La tesis nueva debe escribirse en los archivos `.qmd`.
