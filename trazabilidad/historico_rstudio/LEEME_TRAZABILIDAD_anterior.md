# Trazabilidad de la tesis

## Versión de trabajo

La carpeta `tesis/` contiene la versión con los anexos A, B y C orientados al repositorio privado y el Anexo D que declara el alcance disponible de Lambda y frontend. Esta preparación para RStudio no altera el contenido académico, los anexos, las figuras, la bibliografía ni el formato de esa versión.

El historial se conserva mediante referencias originales, diferencias de texto, registros de intervención e inventarios. No se inventa un historial Git ni se atribuyen fechas de publicación a cambios que no se han publicado.

## Etapas conservadas

| Etapa | Evidencia dentro del paquete |
| --- | --- |
| Proyecto y PDF originales | `referencia_original/tesis_original_sin_modificar.zip` y `referencia_original/PDF_OFICIAL_ENVIADO.pdf`. |
| Atención de comentarios del director | `control_cambios/TRATAMIENTO_COMENTARIOS.md`, `intervenciones.json` y `diferencias_fuentes.diff`. |
| Ajustes de colocación y tamaño de figuras | `control_diagramacion/GUIA_REVISION.md`, registros JSON y `diferencias_diagramacion.diff`. |
| Ajustes finales de rótulos, atribuciones y paginación | `control_final/REVISION_COMENTARIOS_Y_CAMBIOS.md`, registros JSON y `diferencias_finales.diff`. |
| Anexos vinculados al repositorio | `control_cambios/ACTUALIZACION_ANEXOS_REPOSITORIO.md`, `verificacion_anexos_repositorio.json` y `trazabilidad/entrega/diferencias_anexos_repositorio.diff`. |
| Preparación para RStudio | `ABRIR_EN_RSTUDIO.md`, los dos scripts R y `trazabilidad/entrega/verificacion_paquete.json`. |
| Próximos renders locales | `trazabilidad/ejecuciones/`, creada por `renderizar_tesis.R`. |

Las rutas de esta tabla son relativas a la raíz del ZIP. Los informes anteriores conservan el estado y las páginas de su propia etapa; para los anexos actuales use el informe de actualización del repositorio. `LEEME_DIAGRAMACION.md`, `LEEME_VERSION_FINAL.md` y los README antiguos son documentos históricos; el punto de entrada de esta entrega es `ABRIR_EN_RSTUDIO.md`.

## Cómo revisar qué se modificó

Los archivos `.diff` muestran las líneas anteriores con `-` y las nuevas con `+`. Pueden abrirse como texto en RStudio. Los JSON describen las intervenciones y las verificaciones; los archivos de referencia permiten contrastar los cambios con los originales.

La diferencia de los anexos se calculó entre `tesis_version_final_ajustada.zip` y la versión con el repositorio. No contiene códigos completos de ESP ni credenciales. La entrega para RStudio conserva exactamente los archivos académicos de la versión con el repositorio.

## Integridad del paquete

`entrega/inventario_entrega.csv` contiene la ruta, el tamaño, MD5 y SHA-256 de cada archivo de la entrega, salvo el propio inventario para evitar una referencia circular. El script `tesis/verificar_entrega.R` realiza la comparación local usando MD5 y no modifica los archivos.

`entrega/verificacion_paquete.json` registra la comparación con el ZIP anterior. `entrega/render_quarto_validado.log` conserva la salida del render del PDF de referencia. La validación de los scripts R se registra por separado en `entrega/PRUEBA_RENDER_RSTUDIO.md`.

## Alcance de reproducibilidad

Se incluyen los fuentes y los recursos necesarios para generar la tesis. Las versiones del entorno se documentan, pero no se distribuyen instaladores de R, RStudio, Quarto ni LaTeX. La igualdad exacta por bytes del PDF entre equipos no se promete: sus metadatos, fuentes instaladas y versiones del motor pueden variar.

Las capturas se conservan como evidencias originales. Las cuatro figuras añadidas también incluyen sus SVG editables y archivos PDF/PNG. Los scripts R no recalculan mediciones del prototipo ni reconstruyen capturas a partir de datos que no se suministraron.

El código ESP se entrega por separado para GitHub. El comentario 7 del director continúa pendiente de aclaración porque el texto recibido únicamente indica «Sección 3.2».
