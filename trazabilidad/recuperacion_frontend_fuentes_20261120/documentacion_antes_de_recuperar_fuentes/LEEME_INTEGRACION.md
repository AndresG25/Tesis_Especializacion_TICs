# Complemento cloud de la tesis

Este paquete reúne las tres funciones Lambda recuperadas y una copia exacta del dashboard publicado. Incluye documentación y evidencia de procedencia. Los códigos y los archivos publicados se conservan sin modificación.

## Incorporación al proyecto local

1. Extraiga el ZIP en una carpeta temporal.
2. Abra en RStudio el proyecto Tesis_Especializacion_TICs que ya usa con Git. Confirme que en su raíz están ESP1/, ESP2/, ESP3/ y tesis/.
3. Copie las carpetas cloud/, docs/ y trazabilidad/ de este complemento a esa raíz. Combine las carpetas existentes. Si incorporó el complemento Lambda anterior, los fuentes Lambda son idénticos y la documentación cloud/frontend se actualiza con este rescate. Revise cualquier diferencia con cambios locales propios antes de reemplazar archivos.
4. Conserve LEEME_INTEGRACION.md y SHA256SUMS.txt junto al ZIP como inventario de la entrega.
5. Para localizar el proyecto editable del dashboard, ejecute en la consola de RStudio:

```r
source("docs/buscar_frontend.R", encoding = "UTF-8")
```

El buscador lee carpetas y archivos package.json dentro de su perfil de usuario. Omite dependencias instaladas, cachés y carpetas de compilación. Imprime candidatos con su ruta y la presencia de React, Vite y src/. No modifica archivos ni se conecta a AWS. Puede tardar varios minutos; Escape permite detenerlo. Si el proyecto se guardó en otra unidad, indique su carpeta, por ejemplo:

```r
buscar_frontend_tesis(raices = "D:/Proyectos")
```

Comparta la lista de candidatos antes de preparar el ZIP del proyecto editable. La carpeta buscada probablemente se llama turbina-dashboard; el título del HTML recuperado tiene ese nombre, pero no confirma el nombre de la carpeta original.

## Estado y control de versiones

El complemento agrega material documental; no reemplaza la tesis ni altera capítulos, figuras, _quarto.yml, firmware o scripts de renderizado. La observación sobre reproducibilidad sigue abierta hasta localizar los fuentes React/Vite y completar la configuración requerida.

Revise los cambios en la pestaña Git de RStudio. Si corresponden a esta incorporación, seleccione los archivos de cloud/, docs/ y trazabilidad/ y cree un commit, por ejemplo: «Documenta funciones Lambda y respalda dashboard publicado». Después haga Push. No se ha actualizado GitHub desde esta entrega.

Para este complemento no es necesario renderizar la tesis: los archivos .qmd no cambian. La actualización del Anexo D se preparará con el estado comprobado del software.

Detalles: docs/RECUPERACION_FRONTEND_20261120.md y cloud/frontend/ESTADO_RECUPERACION.md.
