# Trazabilidad del proyecto integrado

El punto de entrada actual es `Tesis_Especializacion_TICs.Rproj` en la raíz. La tesis y las tres carpetas ESP forman un único repositorio.

## Historia de las entregas

| Etapa | Ubicación relativa a la raíz |
| --- | --- |
| Proyecto y PDF originales | `referencia_original/`. |
| Comentarios del director | `control_cambios/`. |
| Diagramación | `control_diagramacion/`. |
| Rótulos, atribuciones y ajustes finales | `control_final/`. |
| Anexos con enlaces al repositorio | `control_cambios/ACTUALIZACION_ANEXOS_REPOSITORIO.md` y diferencias conservadas en `trazabilidad/entrega/`. |
| Preparación anterior de RStudio | `trazabilidad/historico_rstudio/` e informes anteriores en `trazabilidad/entrega/`. |
| Preparación anterior del firmware | `trazabilidad/historico_firmware/`. |
| Integración actual | `trazabilidad/entrega/INTEGRACION_RSTUDIO_GIT.md`, `verificacion_integracion.json` y `diferencias_integracion_anexos.diff`. |

Los informes históricos conservan sus rutas y paginaciones de aquel momento. Los antiguos puntos de entrada R se archivaron como texto para dejar un solo `.Rproj` operativo. Los inventarios anteriores se conservan como evidencias de su etapa; la comprobación actual usa `inventario_integrado.csv`.

## Control de versiones y registros de render

Git registra los cambios que usted confirma mediante Commit; Push los envía al remoto. El script de render no hace commits. Cada ejecución registra el commit HEAD disponible, la rama y el estado del árbol de trabajo antes del render. Si todavía no hay un primer commit, lo indica expresamente.

Además conserva una copia de los fuentes activos de la tesis y el firmware, sus huellas antes y después, las versiones del entorno, el PDF previo, el nuevo PDF, el TeX y los mensajes de compilación. Las credenciales, compilaciones, referencias históricas y anteriores carpetas de trazabilidad no se duplican dentro de esas copias activas.

`trazabilidad/ejecuciones/` se mantiene local y está excluida de Git. Los informes de entrega y los inventarios sí se versionan. No modifique archivos mientras renderiza: el registro compara las huellas para detectar cambios durante el proceso.

## Integridad inicial

`trazabilidad/entrega/inventario_integrado.csv` contiene rutas, tamaños, MD5 y SHA-256 de los archivos del ZIP, salvo el propio inventario. `verificar_entrega.R` compara los bytes con MD5 usando funciones de R base. El archivo `.gitattributes` conserva los finales de línea de los fuentes y mantiene disponibles sus diferencias de texto.

Después de editar o renderizar puede haber archivos modificados respecto de esta entrega inicial. Es normal; Git conserva las revisiones que vaya confirmando. El verificador no borra ni restaura su trabajo.

El proyecto incluye los recursos para regenerar el PDF, pero no instaladores de R, Quarto, LaTeX ni ESP-IDF. Las capturas son evidencias conservadas; no se reconstruyen a partir de mediciones que no se adjuntaron.
