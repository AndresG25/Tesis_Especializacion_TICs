# Reemplazo directo de los archivos de la tesis

Esta entrega contiene los archivos editables que debe copiar sobre su proyecto local existente. No requiere ejecutar un instalador. Los capítulos y las figuras son los mismos de la actualización UML revisada; aquí se facilita su reemplazo manual.

## 1. Copiar en las rutas correctas

Abra su proyecto habitual `Tesis_Especializacion_TICs.Rproj`. La raíz es la carpeta donde se encuentran ese archivo, `renderizar_tesis.R`, `tesis`, `ESP1`, `ESP2` y `ESP3`.

Si hizo cambios locales después de la última entrega, conserve primero una copia de los capítulos 3 y 4 fuera del repositorio para poder compararlos. Este reemplazo manual sobrescribe ambos capítulos.

Descomprima el ZIP en Descargas. Abra `Reemplazo_Directo_UML_Tesis` y copie TODO su contenido en la raíz del proyecto existente. Windows debe combinar las carpetas y reemplazar los archivos coincidentes. Mantenga las carpetas existentes y sus demás archivos; no elimine la carpeta `tesis`.

| Archivo o carpeta del paquete | Destino dentro del proyecto | Acción |
|---|---|---|
| `tesis/03-arquitectura-hardware-iot-edge.qmd` | `tesis/03-arquitectura-hardware-iot-edge.qmd` | Reemplazar el capítulo 3. |
| `tesis/04-arquitectura-software-aws.qmd` | `tesis/04-arquitectura-software-aws.qmd` | Reemplazar el capítulo 4. |
| `tesis/fig/uml/` | `tesis/fig/uml/` | Añadir las tres figuras y sus exportaciones. |
| `docs/uml/` | `docs/uml/` | Añadir fuentes editables, instrucciones e inventario. |
| `trazabilidad/actualizaciones/uml_20260914/` | La misma ruta | Añadir el registro de la integración. |
| `verificar_uml.R` | Junto al archivo `.Rproj` | Añadir el verificador. |
| `LEEME_REEMPLAZO_UML.md` | Junto al archivo `.Rproj` | Conservar esta guía. |

No copie la carpeta contenedora `Reemplazo_Directo_UML_Tesis` completa dentro del proyecto: copie los elementos que contiene. Al terminar, el archivo debe estar en `tesis/03-arquitectura-hardware-iot-edge.qmd`, no en `tesis/tesis/` ni en otra subcarpeta adicional.

El paquete conserva la configuración del proyecto, los anexos, los otros capítulos y los códigos ESP. Las figuras se incluyen como PDF vectoriales; no necesita instalar herramientas de dibujo para renderizar.

## 2. Verificar los archivos y renderizar desde R

En la **consola de R** de RStudio:

```r
source("verificar_uml.R", encoding = "UTF-8")
source("renderizar_tesis.R", encoding = "UTF-8")
```

La primera instrucción compara los archivos con la entrega. Si informa archivos faltantes o modificados, revise la copia antes de renderizar. La segunda genera el PDF nuevo y guarda la trazabilidad de la ejecución. Espere hasta que aparezca `PDF generado:` y vuelva el indicador `>`.

El resultado queda en:

`tesis/output/Desarrollo-de-una-arquitectura-escalable-de-Big-Data-en-AWS-alimentada-mediante-un-sistema-de-IoT.pdf`

`verificar_entrega.R` conserva el inventario de la entrega original; para esta actualización utilice `verificar_uml.R`.

## 3. Revisar el PDF que generó su equipo

| Elemento que debe aparecer | Ubicación | Página PDF de referencia | Página impresa de referencia |
|---|---|---:|---:|
| Párrafo de adquisición y publicación | Cierre de 3.8.3 | 65 | 52 |
| Figura 3.2: secuencia de telemetría | Después del párrafo | 66 | 53 |
| Párrafo de despliegue | Cierre de 4.3 | 78 | 65 |
| Figura 4.2: despliegue IoT Edge–AWS | Después del párrafo | 79 | 66 |
| Párrafo de consultas | Cierre de 4.11 | 87 | 74 |
| Figura 4.3: consultas del dashboard | Después del párrafo | 88 | 75 |

Compruebe que las tres figuras aparecen también en el listado de figuras y que el texto remite a sus números correctos. Las páginas de los diagramas son horizontales. Los paneles A y B de cada secuencia forman una sola figura.

La copia de referencia tiene 144 páginas y 30 figuras. La paginación puede variar según el entorno local de Quarto/LaTeX; compruebe los números de figura y las secciones, sin forzar el número total de páginas. El render de referencia y la revisión visual ya se completaron. Esta entrega no verifica por sí sola el PDF que se genere en su computadora.

## 4. Registrar los cambios con Git y subirlos a GitHub

En la **terminal** de RStudio, situada en la raíz del proyecto:

```bash
git status --short
git diff --stat
git diff -- tesis/03-arquitectura-hardware-iot-edge.qmd tesis/04-arquitectura-software-aws.qmd
```

En un proyecto que estaba limpio antes de copiar, deben figurar como modificados los dos capítulos y como nuevos los recursos UML, la guía y los archivos de verificación/trazabilidad. Los documentos nuevos todavía no aparecen en `git diff --stat` hasta prepararlos para el commit. Si la salida del diff ocupa toda la terminal, pulse `q` para salir del visor.

En la pestaña **Git**:

1. Marque **Staged** para los dos capítulos y TODOS los recursos de esta actualización: `tesis/fig/uml`, `docs/uml`, `trazabilidad/actualizaciones/uml_20260914`, `verificar_uml.R` y `LEEME_REEMPLAZO_UML.md`.
2. Pulse **Commit** y use: `Incorpora diagramas UML y párrafos explicativos en capítulos 3 y 4`.
3. Espere la confirmación del commit, cierre esa ventana y pulse **Push**.
4. Espere la confirmación de la subida.

Los PDF de las figuras sí se incluyen en Git. El PDF completo renderizado en `tesis/output/` y las copias de ejecución están excluidos por la configuración existente y permanecen disponibles localmente.

## 5. Comprobar que Git y GitHub quedaron sincronizados

Después de un Push satisfactorio, ejecute en la **terminal**:

```bash
git status
git log -1 --oneline
git rev-parse HEAD
git ls-remote origin refs/heads/main
```

El estado debe indicar `On branch main`, `Your branch is up to date with 'origin/main'` y `nothing to commit, working tree clean`. La huella de `git rev-parse HEAD` debe coincidir con la primera columna de `git ls-remote`: esta última consulta directamente la rama `main` del repositorio remoto. Puede solicitarle autenticación de GitHub.

Abra también [Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs) y compruebe que aparecen el nuevo commit, los dos capítulos y `tesis/fig/uml/`. La verificación remota corresponde a su equipo autenticado; el paquete no hace Commit ni Push automáticamente.
