# Revisión final de comentarios y ajustes

Se revisaron los nueve comentarios reproducidos por el autor y las dos indicaciones
adicionales sobre atribución de imágenes y rótulos de figuras.

**Resultado: comentarios 1–6, 8 y 9 atendidos.** El comentario 7 solo contiene
«Sección 3.2» y no permite identificar una acción; se conserva pendiente.
No se introdujo una corrección supuesta para ese apartado.

## Ruta para verificar los nueve comentarios

Las páginas corresponden al PDF de 138 páginas incluido en esta entrega.
«PDF» es la página del visor; «impresa» es la numeración de la tesis.

| Comentario | Estado y comprobación | Impresa | PDF |
|---|---|---:|---:|
| 1. Ilustrar las capas de la sección 2.2 | Atendido: Figura 2.2 con cuatro capas y explicación. | 11 | 24 |
| 2. Ilustrar el modelo OSI | Atendido: Figura 2.3, matriz OSI–TCP/IP horizontal, conservando la tabla. | 15–16 | 28–29 |
| 3. Identificar el autor y documento de MQTT | Atendido: el párrafo nombra OASIS (2019) y la especificación MQTT Version 5.0. | 17–18 | 30–31 |
| 4. Comparar arquitectura anterior y propuesta | Atendido: Figura 2.4 y explicación antes de la sección 2.5. | 19 | 32 |
| 5. Proyección a estabilidad de sistemas de potencia | Atendido como proyección en 2.7 y trabajo futuro; no se presenta como resultado validado. | 24 y 108 | 37 y 121 |
| 6. Timestamp en cursiva | Atendido en la aparición señalada de la sección 3.1; verificado en la tipografía del PDF. | 30 | 43 |
| 7. «Sección 3.2» | Pendiente: no especifica qué corregir. | — | — |
| 8. Orientar sobre los subcapítulos del capítulo 3 | Atendido: dos párrafos describen las secciones 3.1–3.14. | 29–30 | 42–43 |
| 9. Resumen del capítulo 3 | Atendido: tres párrafos de síntesis, incluido el enlace hacia el capítulo de AWS. | 56–57 | 69–70 |

## Ajustes incorporados en esta entrega

### Atribuciones de las cuatro figuras añadidas

Las Figuras 2.2, 2.3, 2.4 y 4.1 utilizan la fórmula del pantallazo de referencia:

> Fuente: elaboración propia con apoyo de ChatGPT, modelo GPT-6 Astra Media de OpenAI, 2026.

Se conservan, cuando corresponden, las aclaraciones del pie sobre los modelos de
referencia, la documentación de protocolos y el antecedente de Gómez Flórez (2024).
Las figuras anteriores 2.1 y 3.1 mantienen su atribución original a GPT-5.5 Thinking;
esta entrega actualiza los créditos de las cuatro figuras añadidas.

### Rótulos y referencias a figuras

Las **27 figuras** del libro utilizan «Figura 2.2», «Figura 5.20», etc. Se eliminó
«nro.» tanto de los pies como de las referencias cruzadas del texto. Se regeneraron
los listados y referencias. La denominación de las tablas se conserva como estaba.

### Paginación de los dos bloques señalados

- La síntesis de DynamoDB se mantiene completa en una página (PDF 99, impresa 86).
- El título y los dos párrafos del cierre del capítulo 5 se mantienen juntos
  (PDF 104, impresa 91). No queda una página con una sola línea de ese cierre.
- Se redujo únicamente el ancho de presentación de la última captura del dashboard
  del 90 % al 82 % para reservar espacio al cierre. El archivo de la captura es idéntico.
- Se mantienen los controles de orden de las ocho pruebas y sus evidencias.

## Preservación

Sobre el ZIP entregado en la ronda anterior se modificaron únicamente cinco
archivos de configuración o fuentes: los capítulos 2, 4 y 5, `_quarto.yml` y
`tex/preamble.tex`. Las modificaciones exactas están en `intervenciones_finales.json`
y `diferencias_finales.diff`. Al revertirlas se recuperan los bytes de esos cinco
archivos anteriores, incluidos sus saltos de línea.

Los **35 archivos de imágenes PNG, JPG y SVG son idénticos**. También se mantienen
los PDF vectoriales de las figuras, los anexos, las ecuaciones, los datos,
la bibliografía y los textos académicos ajenos a las atribuciones solicitadas.
No se regeneró ni se retocó ninguna imagen.

El PDF y el archivo LaTeX generado se actualizaron mediante render completo.
Los informes de `control_cambios/` y `control_diagramacion/` son registros de
entregas anteriores; para el estado de esta versión use `control_final/`.

## Verificación realizada

- Render completo con Quarto 1.8.25 y pdfLaTeX.
- Revisión visual de las cuatro figuras, sus pies, CloudFront, DynamoDB y el cierre.
- 27 figuras numeradas y ninguna aparición de «Figura nro.» en el PDF.
- Sin marcadores `??`, `@fig-…`, `@tbl-…` o `@eq-…` pendientes de resolución.
- Ocho bloques de prueba conservan sus evidencias antes de la síntesis y del
  componente siguiente.
- Verificación por bytes de preservación y comprobación de integridad del ZIP.

Los saltos de página pueden variar con la versión local de Quarto o LaTeX.
Las reglas que mantienen juntos los dos bloques se incluyen en las fuentes.
El PDF adjunto es la referencia de las páginas de esta guía.
