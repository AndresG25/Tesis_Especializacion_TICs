# Versión final ajustada de la tesis

Abra primero el PDF que está en `tesis/output/`. Esta entrega incluye el proyecto
completo y el PDF ya renderizado. La revisión detallada de los nueve comentarios
está en `control_final/REVISION_COMENTARIOS_Y_CAMBIOS.md`.

## Qué cambió

- Atribución completa de las cuatro figuras añadidas, con el modelo indicado:
  GPT-6 Astra Media de OpenAI, 2026.
- «Figura» en todos los pies y referencias del libro, sin «nro.».
- Paginación de la síntesis de DynamoDB y del cierre del capítulo 5.

Las imágenes se conservan. Los comentarios 1–6, 8 y 9 están atendidos. El 7 sigue
pendiente porque la observación recibida solo dice «Sección 3.2».

## Abrir y renderizar en RStudio

1. Extraiga todo el ZIP en una carpeta nueva.
2. Abra `tesis/tesis.Rproj`.
3. En la consola de RStudio ejecute:

```r
system("quarto render --to pdf")
```

El directorio de trabajo debe ser `tesis`, donde está `_quarto.yml`.
La salida se crea en `tesis/output/`.

El PDF incluido tiene 138 páginas. La paginación puede cambiar al renderizar
con otra versión de Quarto o LaTeX; las fuentes contienen las correcciones.
Para consultar el resultado entregado no es necesario renderizar otra vez.

Los otros LEEME e informes se conservaron para trazabilidad y describen entregas
anteriores. El estado final y la ruta de revisión están en `control_final/`.
