# Revisión de las correcciones de diagramación

**Resultado: los cuatro ajustes solicitados están aplicados y verificados.**
El PDF completo tiene 138 páginas. Esta revisión corresponde a la diagramación;
no modifica ni reevalúa el contenido académico previamente aprobado.

## Ruta de revisión

La columna «PDF» es el número que se introduce en el visor. «Impresa» es la
numeración de la tesis, que aparece en el encabezado o pie de página.

| Elemento | Impresa | PDF | Qué comprobar |
|---|---:|---:|---|
| Figura 2.2: cuatro capas | 11 | 24 | Letra ampliada y todos los textos originales. |
| Figura 2.3: matriz OSI–TCP/IP | 15 | 28 | Página horizontal; cinco columnas, siete filas OSI y notas completas. |
| Figura 2.4: comparación de arquitecturas | 19 | 32 | Figura y explicación dentro de la sección 2.4. |
| Inicio de la sección 2.5 | 20 | 33 | Comienza después de la figura de comparación. |
| Figura 4.1: arquitectura AWS | 59 | 72 | Letra ampliada; dos rutas y todos los servicios y textos conservados. |
| Explicación de la figura AWS | 60 | 73 | Párrafo completo después de la figura. |
| Evidencias ESP1 | 76–78 | 89–91 | Figuras 5.1–5.3 dentro de la prueba ESP1. |
| Evidencias ESP2 | 79–81 | 92–94 | Figuras 5.4–5.6 dentro de la prueba ESP2. |
| Evidencias y síntesis ESP3 | 82–84 | 95–97 | Figuras 5.7–5.9; la síntesis aparece después de las tres evidencias. |
| IoT Core y DynamoDB | 85–86 | 98–99 | Evidencias y síntesis antes del componente siguiente. |
| S3, Glue y Athena | 86–88 | 99–101 | Figuras 5.13–5.16 antes de la síntesis y de Lambda. |
| Lambda | 88–89 | 101–102 | Figuras 5.17–5.18 y síntesis en su prueba. |
| CloudFront | 90 | 103 | Pie completo y número de figura 5.20; citas conservadas debajo. |
| Dashboard y cierre del capítulo | 91 | 104 | Figura 5.21, síntesis de la prueba y síntesis del capítulo en ese orden. |

## Cambios realizados

1. Se fijó la posición de las 21 evidencias del capítulo 5 y se añadieron barreras
   al inicio de las pruebas. Las fotografías verticales de ESP2 y ESP3 se
   ajustaron de tamaño para que quepan junto a sus títulos y textos. Los archivos
   de las capturas y fotografías no se editaron.
2. Se controló la colocación de las nuevas figuras. La comparación de
   arquitecturas ya no cruza el inicio de la sección 2.5.
3. Se redistribuyeron las cuatro figuras a partir de sus SVG editables. La matriz
   OSI utiliza una página horizontal. Los textos principales quedan aproximadamente
   entre 9 y 11 puntos, según la figura; las notas más pequeñas, alrededor de
   8,3–8,7 puntos. Se preservaron todos los textos y relaciones de las figuras.
4. Se separaron las citas que estaban en la misma línea de la imagen de
   CloudFront. Se utilizó el pie que ya estaba escrito en su atributo `fig-cap`.
   CloudFront pasa a ser la figura 5.20 y el dashboard pasa a ser la figura 5.21.

## Preservación comprobada

Se compararon los 81 archivos del ZIP recibido. Se modificaron únicamente tres
QMD (capítulos 2, 4 y 5) y los ocho archivos PNG/SVG de las cuatro figuras.
**Los otros 70 archivos permanecen idénticos por bytes.**

Los cambios de los QMD son reversibles: al quitar exactamente las intervenciones
registradas se recuperan los bytes originales, incluidos los saltos de línea.
No hubo reescritura de párrafos. Se conservan las seis síntesis existentes,
las citas, ecuaciones, tablas, anexos, bibliografía, configuración y referencias
originales. La comparación del texto de los SVG confirma que no se perdió ni se
agregó ninguna palabra a las cuatro figuras.

Se renderizó el libro completo, se actualizaron los índices y referencias y se
inspeccionaron visualmente las páginas intervenidas y sus transiciones. No se
encontraron marcadores `??`, `@fig-…`, `@tbl-…` ni `@eq-…` sin resolver.

La nueva paginación es consecuencia de la diagramación. Al renderizar en otra
versión de Quarto o LaTeX pueden variar algunos saltos de línea o página.
El PDF incluido es la referencia de las páginas indicadas en esta guía.

El comentario 7 permanece pendiente y no se modificó.
