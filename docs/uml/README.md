# Integración de diagramas UML

Versión: `uml-20260914`. Aprobación del usuario: incorporación de los tres diagramas y de los tres párrafos explicativos acordados en la conversación.

| Figura | Identificador Quarto | Sección | Archivo vectorial |
|---|---|---|---|
| 3.2 | `fig-uml-secuencia-telemetria` | Cierre de 3.8.3 | `tesis/fig/uml/02_secuencia_telemetria.pdf` |
| 4.2 | `fig-uml-despliegue` | Cierre de 4.3 | `tesis/fig/uml/01_despliegue.pdf` |
| 4.3 | `fig-uml-secuencia-consultas` | Cierre de 4.11 | `tesis/fig/uml/03_secuencia_consultas.pdf` |

La numeración la resuelve Quarto a partir de los identificadores. Las imágenes no tienen números incrustados. Los paneles A y B de las secuencias pertenecen a una sola figura cada uno. Las figuras previas conservan sus identificadores.

## Alcance de los cambios

Solo se añaden tres bloques a los capítulos 3 y 4. Los párrafos aprobados se conservan literalmente, sustituyendo el número escrito por una referencia automática. Se incluyen figuras vectoriales, fuentes editables y documentación de la actualización.

La disposición de las figuras se adapta a páginas horizontales dentro de los márgenes existentes. No se cambia la geometría general, la bibliografía, el firmware, los anexos, ni los scripts de renderizado o de configuración de Git.

La secuencia de telemetría se contrastó con los archivos ESP suministrados. La secuencia de consultas se presenta como **modelo**, tal como indica el párrafo aprobado: falta cotejar las llamadas internas con el código de `GetTimeseries`, que no fue suministrado. No se añadió ninguna afirmación de que ese cotejo ya se realizó.

## Renderizar y comprobar

Desde la consola de R, con `Tesis_Especializacion_TICs.Rproj` abierto:

```r
source("verificar_uml.R", encoding = "UTF-8")
source("renderizar_tesis.R", encoding = "UTF-8")
```

Se usa el renderizador habitual, que registra el estado de Git y conserva los fuentes y el PDF de cada ejecución. No necesita Python, Java o PlantUML para renderizar la tesis: los PDF vectoriales ya están incluidos.

`verificar_entrega.R` conserva la referencia de la entrega original. Después de esta actualización es normal que señale los capítulos 3 y 4 como modificados; para esta versión use `verificar_uml.R`. El inventario nuevo excluye su propio CSV y los resultados de render.

## Editar y regenerar los diagramas (opcional)

- `generar_despliegue.py` fija la disposición horizontal del despliegue y reutiliza las primitivas de dibujo de `generar_despliegue_propuesta.py`.
- `fuentes/02a_adquisicion.puml` y `02b_publicacion.puml` componen la Figura 3.2.
- `fuentes/03a_ultimo_dato.puml` y `03b_historico.puml` componen la Figura 4.3.
- `paneles/` contiene los SVG de las cuatro secuencias parciales.
- `fuentes/01_despliegue.puml`, `02_secuencia_telemetria.puml` y `03_secuencia_consultas.puml` conservan los modelos de la propuesta anterior. Su diagramación automática no es la de la versión horizontal integrada.

Las dependencias de edición opcionales son Python, CairoSVG y PlantUML 1.2026.8 con Java. CairoSVG necesita Cairo instalado en el sistema. Las fuentes usan DejaVu Sans.

Desde una terminal:

```bash
python docs/uml/generar_figuras.py --jar "C:/herramientas/plantuml-1.2026.8.jar"
```

Para recomponer a partir de los paneles SVG ya incluidos:

```bash
python docs/uml/generar_figuras.py --solo-exportar
```

La regeneración de imágenes se verificó en Linux. No se probó la instalación de esas herramientas de edición en Windows.

## Trazabilidad

Los párrafos, ubicaciones, diferencias y comprobaciones de esta integración se encuentran en `trazabilidad/actualizaciones/uml_20260914/`.

La actualización se copia sobre el proyecto existente siguiendo [LEEME_REEMPLAZO_UML.md](../../LEEME_REEMPLAZO_UML.md). Mantenga su archivo `.Rproj` y su historial Git. Para registrar la versión: revisar los cambios, marcar Staged, hacer Commit y después Push desde RStudio.
