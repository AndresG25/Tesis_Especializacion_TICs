# Informe de refuerzo bibliográfico académico

Fecha de generación: 2026-06-06

## Objetivo de esta nueva intervención

Se construyó una versión reforzada del proyecto Quarto para que el documento no dependiera excesivamente de documentación de proveedor. La intervención se concentró en aumentar el peso de literatura académica, artículos de revista y trabajos previos relacionados con IoT, cloud computing, Big Data, seguridad IoT, monitoreo de turbinas eólicas, turbinas de eje vertical, predicción eólica, instrumentación y manufactura aditiva.

## Criterio aplicado

- No se reescribió el contenido humanizado de los capítulos.
- Se añadieron citas al final de párrafos ya existentes.
- Se conservaron las citas técnicas oficiales de AWS y Espressif cuando son necesarias para justificar implementación concreta.
- Se incorporaron artículos de revistas indexadas y literatura científica tomada de los insumos del anteproyecto, la tesis de maestría y fuentes académicas complementarias.
- Se redujo la bibliografía final a referencias efectivamente citadas dentro del documento para mantener una lista manejable.

## Resultado bibliográfico

- Referencias citadas reales en el proyecto: **70**.
- Artículos de revista citados: **35**.
- Otras fuentes citadas: **35**.
- Documentación AWS citada: **14**.
- Documentación Espressif citada: **8**.
- Porcentaje de artículos: **50.0%**.

Con este ajuste, la tesis queda en **70 referencias citadas** y **35 artículos de revista**, cumpliendo el rango solicitado de 50 a 70 referencias y la condición de que al menos la mitad correspondan a artículos académicos.

## Principales bloques fortalecidos

1. **Energía eólica de pequeña escala y VAWT**: se reforzó con literatura sobre turbinas Savonius, H-Darrieus, revisiones de VAWT, microgeneración eólica y desempeño experimental.
2. **IoT y arquitecturas cloud**: se incorporaron revisiones académicas sobre IoT, integración IoT-cloud, Industrial IoT y Big IoT Data Analytics.
3. **Monitoreo, predicción y mantenimiento**: se fortaleció con referencias sobre monitoreo de condición, predicción de potencia eólica y uso de IoT/ML en sistemas eólicos.
4. **Seguridad IoT**: se reforzó con literatura sobre amenazas, firmware, Zero Trust y machine learning para ciberseguridad en IoT.
5. **Big Data y nube**: se agregaron referencias para justificar que el prototipo no es Big Data por volumen inicial, sino que se orienta a una arquitectura escalable para crecimiento futuro.

## Validación interna

- Claves bibliográficas usadas y no encontradas: `[]`.
- Archivo bibliográfico final: `bibliografia.bib`.
- Se volvió a aplicar saneamiento LaTeX a caracteres `&` para evitar errores de compilación con `pdflatex` y `plainnat`.

## Recomendación para renderizar

Desde la carpeta raíz del proyecto:

```bash
quarto render --to pdf
```

Si existen residuos de compilaciones anteriores, limpiar primero:

```bat
del index.aux index.bbl index.blg index.log index.out index.tex index.pdf
```
