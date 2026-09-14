# Tesis completa para trabajar en RStudio

Esta carpeta reúne el proyecto editable con los anexos vinculados al repositorio, todos sus recursos y los registros de las revisiones anteriores. El contenido de la tesis y el PDF de referencia se conservan exactamente como en la entrega `Proyecto_Tesis_Anexos_Repositorio.zip`.

## Abrir y generar el PDF

1. Extraiga **todo** el ZIP en una carpeta nueva. En Windows puede usar una ruta corta, por ejemplo `C:/Tesis_RStudio_Trazabilidad/`.
2. Abra `tesis/tesis.Rproj` con RStudio. El directorio de trabajo debe ser la carpeta `tesis`, que contiene `_quarto.yml`.
3. En la consola de RStudio ejecute:

```r
source("renderizar_tesis.R", encoding = "UTF-8")
```

El comando genera el PDF en `tesis/output/`. Además, crea un registro independiente en `trazabilidad/ejecuciones/`, identificado por fecha, hora y proceso. Espere a que termine antes de editar los fuentes. Cierre el PDF si su visor de Windows impide reemplazarlo.

## Qué queda guardado en cada ejecución

| Archivo o carpeta | Para qué sirve |
| --- | --- |
| `fuentes/` | Copia de los documentos, configuración, imágenes, bibliografía y demás recursos presentes antes de renderizar. |
| `fuentes_antes.csv` y `fuentes_despues.csv` | Inventario y huellas MD5 para comprobar si los archivos cambiaron durante la ejecución. |
| `versiones.txt` y `sesion_R.txt` | Versiones disponibles de R, Quarto, Pandoc y pdfLaTeX; información de la sesión de R. |
| `ejecucion.txt` | Momento de inicio, carpeta utilizada y comando ejecutado. |
| `render_stdout.log` y `render_stderr.log` | Salida del proceso y mensajes de compilación, incluidos errores o advertencias. |
| `resultado.csv` | Estado final, código de salida y huella del PDF generado. |
| `tesis_previa.pdf` | Copia del PDF anterior, si existía antes de iniciar. |
| `tesis_generada.pdf` y `tesis_generada.tex` | Copias del PDF y del TeX de esa ejecución cuando se generan correctamente. |

Si la compilación falla, el script informa el error y conserva los registros. El PDF que pudiera seguir en `output/` no se presenta como una nueva generación exitosa. Cada ejecución ocupa espacio adicional porque conserva sus fuentes y resultados.

## Comprobar la entrega antes de editar

Puede ejecutar primero:

```r
source("verificar_entrega.R", encoding = "UTF-8")
```

La comprobación compara los archivos del paquete con el inventario inicial y muestra cuáles son idénticos, cuáles cambiaron y cuáles faltan. No restaura ni modifica archivos. Después de editar o renderizar es normal que el PDF, el TeX o sus fuentes aparezcan como modificados.

El inventario incluye MD5 y SHA-256. El script usa MD5 para comparar bytes con las funciones que ya incluye R; no requiere instalar paquetes adicionales.

## Requisitos del equipo

Se necesitan **R, RStudio, Quarto y LaTeX**. R inicia y documenta la ejecución; Quarto y LaTeX componen la tesis. Conserve la instalación con la que ya había renderizado el proyecto.

Si RStudio no encuentra Quarto, compruebe:

```r
Sys.which("quarto")
```

Si el resultado está vacío, instale Quarto o reinicie RStudio después de instalarlo. También puede indicar la ruta completa de su ejecutable antes de llamar al script:

```r
# Ejemplo: ajuste esta ruta a su instalación real.
Sys.setenv(QUARTO_PATH = "C:/Program Files/Quarto/bin/quarto.cmd")
source("renderizar_tesis.R", encoding = "UTF-8")
```

Si no tiene LaTeX, la [documentación oficial de PDF de Quarto](https://quarto.org/docs/output-formats/pdf-basics.html) explica cómo instalar TinyTeX. Desde una instalación de Quarto accesible en R puede ejecutar:

```r
system("quarto install tinytex")
```

No es necesario reinstalarlo si su entorno ya genera el documento. Los scripts entregados no instalan software automáticamente.

## Ubicar los cambios anteriores

Abra [trazabilidad/LEEME_TRAZABILIDAD.md](trazabilidad/LEEME_TRAZABILIDAD.md). Allí se identifica qué informe corresponde a cada etapa y cómo consultar las diferencias de los anexos.

La versión de referencia incluida tiene 138 páginas. Las diferencias de versión de Quarto, LaTeX o sus paquetes pueden cambiar los saltos de línea y la paginación al ejecutar localmente. La carpeta de ejecución registra el entorno utilizado para poder comparar esas diferencias.

## Comando directo, sin registro automático

El comando habitual continúa disponible desde la carpeta `tesis`:

```r
system("quarto render --to pdf")
```

Para conservar la trazabilidad de cada generación, utilice `renderizar_tesis.R`. La organización del proyecto sigue el funcionamiento de los [proyectos de Quarto](https://quarto.org/docs/projects/quarto-projects.html).
