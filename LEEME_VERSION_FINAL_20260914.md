# Actualización final de la tesis — 14 de septiembre de 2026

Este paquete se copia sobre el proyecto RStudio que ya está vinculado al repositorio privado [Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs). Contiene los archivos de reemplazo y la documentación nueva. El resto del proyecto permanece en su carpeta actual.

## Qué cambia

| Archivo existente | Cambio |
|---|---|
| `tesis/tex/frontmatter.tex` | Primer párrafo del resumen y del abstract centrado en el problema. |
| `tesis/index.qmd` | Antecedente, dificultad de integración, brecha y aporte; justificación delimitada al caso documentado. |
| `tesis/02-marco-referencia.qmd` | Comillas en «redes de cosas», al comienzo del capítulo. |
| `tesis/03-arquitectura-hardware-iot-edge.qmd` | Experiencia de tres meses, intentos con MicroPython, continuación en C/ESP-IDF y reconocimiento a DeepSeek, dentro de 3.8.2. |
| `tesis/anexos/anexo-c-codigo-esp3.qmd` | Enlace a la guía de conexión ESP3–AWS IoT Core. |
| `ESP3/README.md` | Acceso a la misma guía desde la carpeta del nodo. |

Los tres PDF de figuras en `tesis/fig/uml/` se incluyen como dependencias y coinciden byte por byte con la entrega UML anterior. Se mantienen su numeración y sus párrafos. No se modificaron el firmware, la bibliografía, la configuración de Quarto ni los demás capítulos y anexos.

La introducción activa es **`tesis/index.qmd`**, según `tesis/_quarto.yml`. El archivo previo `01-introduccion.qmd` no participa en esta compilación y no debe reemplazarse con el nuevo texto.

Se añaden la [guía de conexión](docs/GUIA_CONEXION_ESP3_AWS.md), sus dos plantillas, el verificador y el registro de cambios en `trazabilidad/actualizaciones/final_20260914/`. La guía se contrastó con el código; esta edición no incluyó una nueva prueba física ni acceso a la cuenta AWS.

## 1. Copiar en la carpeta correcta

1. Cierre los archivos que tenga abiertos en el editor. En la terminal de RStudio ejecute `git status` para identificar cualquier cambio local pendiente y conservarlo antes del reemplazo.
2. Extraiga el ZIP fuera del repositorio.
3. Abra la carpeta extraída `Version_Final_Tesis` y copie **su contenido** a la raíz actual, donde están `Tesis_Especializacion_TICs.Rproj`, `renderizar_tesis.R`, `tesis`, `ESP1`, `ESP2` y `ESP3`.
4. Combine las carpetas y acepte el reemplazo de los archivos indicados. No coloque `Version_Final_Tesis` dentro de `tesis`: las rutas deben quedar como en la tabla anterior. Copie el contenido sobre las carpetas existentes; no elimine esas carpetas.

Continúe abriendo el mismo archivo `.Rproj`. La actualización utiliza el repositorio ya configurado.

## 2. Verificar y renderizar en RStudio

En la **consola de R**, desde la raíz del proyecto, ejecute:

```r
source("verificar_version_final.R", encoding = "UTF-8")
source("renderizar_tesis.R", encoding = "UTF-8")
```

El primer comando comprueba las huellas de los archivos, admitiendo diferencias de fin de línea entre Windows y Linux. Si informa un archivo faltante o modificado, revise la copia antes de renderizar. Después de futuras ediciones legítimas puede informar diferencias respecto de esta entrega; el inventario es una referencia histórica de esta versión.

El segundo comando conserva el flujo existente: genera el PDF en `tesis/output/` y crea una ejecución local en `trazabilidad/ejecuciones/`. Abra el PDF cuya ruta imprima la consola.

Los verificadores de entregas anteriores comparan contra sus versiones históricas y pueden señalar como modificado el capítulo 3. Para este reemplazo use `verificar_version_final.R`.

## 3. Revisar el PDF

Compruebe:

- Resumen y abstract: comienzan por la necesidad de integrar adquisición, comunicación segura, almacenamiento y consulta.
- Introducción y 1.1: se identifican el antecedente, la dificultad de integración y el aporte documentado.
- Inicio del capítulo 2: aparece «redes de cosas» entre comillas.
- Sección 3.8.2: se conserva el relato del autor, con MicroPython, C/ESP-IDF, los tres meses aproximados y DeepSeek.
- Figuras 3.2, 4.2 y 4.3: permanecen los tres UML con sus referencias.
- Anexo C: el enlace conduce a `docs/GUIA_CONEXION_ESP3_AWS.md` en GitHub y se conserva el correo del autor. El archivo nuevo será accesible en GitHub después del Push, para usuarios autorizados en el repositorio privado.

La [ruta de revisión](trazabilidad/actualizaciones/final_20260914/ruta_revision.csv) indica las páginas de la compilación de referencia. Su instalación de Quarto y LaTeX puede producir una paginación distinta: use los títulos y números de sección para localizar cada cambio.

## 4. Guardar la nueva versión en Git y GitHub

En la **terminal** de RStudio:

```bash
git status --short
git diff --stat
```

Revise las diferencias en la pestaña **Git**. Aparecerán los seis archivos existentes de la tabla y los archivos nuevos de documentación y trazabilidad; los PDF UML idénticos normalmente no aparecerán como cambios. Marque en **Staged** los archivos de esta actualización.

Mensaje sugerido para **Commit**:

```text
Precisa la brecha y documenta la conexión ESP3 con AWS IoT Core
```

Después haga **Push**. Compruebe en GitHub el mensaje del commit y la guía de conexión. El PDF completo generado se conserva localmente según el `.gitignore` existente; los fuentes, las figuras y los registros de esta actualización permiten regenerarlo.

En la terminal, compruebe:

```bash
git status
git rev-parse HEAD
git ls-remote origin refs/heads/main
```

Debe aparecer el árbol de trabajo limpio y la rama al día. El hash de `HEAD` debe coincidir con el hash remoto de `refs/heads/main`. Esto confirma la publicación de ese commit; la comprobación visual del PDF corresponde al paso anterior.

## Registro de esta entrega

La carpeta `trazabilidad/actualizaciones/final_20260914/` contiene el texto antes/después, el diff, las huellas de los archivos, la ruta de revisión, el resultado de la compilación y el alcance de las verificaciones. No se añadieron resultados experimentales ni se atribuyó el apoyo de DeepSeek a un origen de sus datos de entrenamiento.
