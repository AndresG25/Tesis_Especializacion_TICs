# Actualización Capítulo 4 — Arquitectura de software en AWS

Contenido del paquete:

- `04-arquitectura-software-aws.qmd`: capítulo 4 actualizado.
- `fig/cap4_arquitectura_software_aws_final.png`: infografía final adoptada para el capítulo.
- `bibliografia_append_capitulo4_aws.bib`: referencias bibliográficas complementarias del capítulo 4.
- Carpeta `fig/`: evidencias de apoyo utilizadas durante la redacción.

## Criterio adoptado

La figura principal del capítulo 4 es `fig/cap4_arquitectura_software_aws_final.png`.

La narrativa del capítulo se consolidó sobre el flujo real implementado:

`ESP3 -> AWS IoT Core -> IoT Rule -> DynamoDB / S3 -> Glue / Athena -> Lambda Function URLs -> React + CloudFront`

API Gateway se conserva únicamente como evidencia complementaria dentro de la carpeta `fig/`, pero no se presenta como componente central de la arquitectura final del capítulo.
