# Trazabilidad de la actualización UML

- `integracion_aprobada.json`: los tres párrafos aprobados, sus ubicaciones y los bloques Quarto añadidos.
- `cambios_capitulos.diff`: diferencias completas en los únicos dos capítulos intervenidos.
- `verificacion_integracion.json`: comprobaciones de preservación de los fuentes, numeración, referencias y render.
- `ruta_revision.csv`: páginas de los elementos nuevos en el PDF de referencia de 144 páginas.
- `registro_render.txt`: salida del render de referencia. Incluye avisos bibliográficos preexistentes; no se modificó la bibliografía.
- `prueba_instalacion.json`: pruebas del instalador en copias aisladas del proyecto.

Los números de figura se resuelven automáticamente con Quarto. La paginación local puede variar según el entorno. Los informes de la entrega anterior conservan su identidad y sus huellas; el nuevo inventario está en `docs/uml/inventario_archivos.csv`.

El instalador registra y respalda cada aplicación local en `trazabilidad/ejecuciones/actualizacion_uml_.../`. El renderizador habitual conserva su propia copia de fuentes y PDF. Git registra posteriormente la versión que usted confirme mediante Commit y Push.
