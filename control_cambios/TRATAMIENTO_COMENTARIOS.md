# Tratamiento de comentarios del director

La numeración de los comentarios corresponde a la lista suministrada por el
usuario. Las referencias a apartados originales se mantienen en este registro
para facilitar su seguimiento. La revisión introduce una sección 5.3 para reunir
las evidencias de todos los componentes, por lo que cambia su numeración.

| N.º | Tratamiento | Ubicación / estado |
|---|---|---|
| 1 | Insertada la figura de cuatro capas y un párrafo de lectura. | 2.2. No se modifica la explicación original de las capas. |
| 2 | Insertada la matriz OSI/TCP-IP aprobada y una aclaración de lectura por tramo. Se aclara la fila funcional de seguridad de la tabla, sin reescribirla. | 2.3. Figura `fig-cap2-osi-tramos`. |
| 3 | Añadido «En la especificación MQTT Version 5.0» antes de la atribución a OASIS. | 2.4. Se conserva el resto del párrafo, incluida su primera frase. |
| 4 | Insertada la comparación funcional con la arquitectura precedente y su explicación. | Final de 2.4. Se mantiene la referencia existente a Gómez Flórez. |
| 5 | Añadida una mención acotada a supervisión y posible estudio de estabilidad; añadida una línea de trabajo futuro. | Final de 2.7 y punto 10 de trabajos futuros. No se atribuyen resultados de estabilidad al prototipo. |
| 6 | Aplicada cursiva a la aparición señalada de timestamp. | 3.1. No se hizo sustitución global de términos extranjeros. |
| 7 | Sin modificación específica. | PENDIENTE: el comentario «Sección 3.2» está incompleto. |
| 8 | Añadidos dos párrafos de orientación que describen los apartados 3.1–3.14. | Introducción del capítulo 3. |
| 9 | Conservados literalmente los dos párrafos de síntesis existentes; añadido un tercer párrafo de transición hacia AWS. | Final de 3.14. |
| 10 | Sustituida la referencia de imagen 4.1 por la nueva figura; actualizado su crédito sin asignar un modelo de IA no comprobado. | Capítulo 4. Se conserva el identificador de figura y la explicación original. Se mantienen las dos rutas conforme a la posdata del director. |
| 11 | Aplicado formato PDF con puntos de salto permitidos en la ruta S3; para HTML/DOCX se conserva el bloque literal original. | 4.6. No se cambia el bucket ni el prefijo. Revisión visual final pendiente. |
| 12 | Añadidos cierre del capítulo 1 y síntesis general del capítulo 5. Conservadas las síntesis existentes de 2, 3, 4, 6 y el cierre de 7. | No se duplican resúmenes. El capítulo 3 recibe solo el párrafo señalado en el comentario 9. |
| 13 | Nombrado el «procedimiento de validación incremental por etapas con trazabilidad de evidencias». | 5.1. Se presenta como organización de este trabajo, no como estándar ni certificación. |
| 14 | Añadidas seis síntesis de pruebas: ESP3, IoT Core, DynamoDB, S3/Glue/Athena, Lambda y dashboard. | Antiguas 5.2.3–5.2.8; pasan a 5.3.3–5.3.8. Se inserta la sección «Evidencias de validación por componente» sin cambiar títulos ni orden de las pruebas. |
| 15 | Sustituida «podría escalarse a n cantidad» por «podría escalarse a un número $n$ de turbinas». | 6.3. También se expresa n en notación matemática en el título 6.4. |
| 16 | Redistribuidos anchos de columnas; la celda del tópico se abrevia y el ejemplo literal se conserva debajo. | Tabla 6.8. No se elimina información; revisión visual final pendiente. |

## Ajuste técnico vinculado al comentario 14

Las fotografías de ESP2 y ESP3 tenían el mismo identificador
`fig-esp3-desenergizada`. Se asignó `fig-esp2-desenergizada` a la fotografía de
ESP2. El texto, el archivo de imagen y el pie de esa fotografía se conservaron.

## Cambios no aplicados

- No se reescribieron los capítulos ni las conclusiones existentes.
- No se actualizaron precios, resultados, pruebas o afirmaciones técnicas ajenas
  a los comentarios.
- No se reformatearon globalmente los extranjerismos ni los identificadores de código.
- No se incorporó una nueva bibliografía ni se cambiaron las entradas BibTeX.
- No se atribuye a esta revisión un porcentaje obtenido en detectores de IA.

## Limitación de verificación

La sintaxis y la preservación pueden comprobarse sin Quarto; la paginación y
los ajustes efectivos de margen del PDF requieren ejecutar Quarto en RStudio.
El PDF de `referencia_original/` es el oficial recibido, no una salida nueva.
