# Actualización documental — 18 de noviembre de 2026

## Qué contiene esta entrega

Proyecto completo con la misma organización: `.Rproj` en la raíz, documento bajo `tesis/`, nodos `ESP1/`, `ESP2/`, `ESP3/`, documentación y trazabilidad. Se conservan capítulos, secciones, anexos, diagramas UML y referencias originales. El firmware no se modificó. La base recibida corresponde al commit `64c0305` (Integración cierre de brechas tecnológicas).

El PDF incluido se generó con Quarto 1.8.27 y pdfLaTeX de TeX Live 2026. La paginación puede variar ligeramente con otra instalación; las figuras, tablas y referencias se numeran automáticamente.

## Abrir y comprobar en RStudio

1. Descomprima en una carpeta nueva para revisar la entrega. Abra `Tesis_Especializacion_TICs.Rproj` desde su raíz, no desde `tesis/`.
2. Ejecute en la consola de R:

```r
source("verificar_saneamiento.R", encoding = "UTF-8")
source("recalcular_evidencia.R", encoding = "UTF-8")
source("renderizar_tesis.R", encoding = "UTF-8")
```

La primera instrucción compara los archivos de esta revisión con su inventario. La segunda reproduce con R base las tablas de evidencia y guarda resultados en `trazabilidad/ejecuciones/recalculo_evidencia_20261118/`. La tercera ejecuta el procedimiento original de generación del PDF y registro del entorno. Requiere Quarto y una instalación LaTeX operativa, como la que ya utilizaba el proyecto.

El PDF queda en `tesis/output/Desarrollo-de-una-arquitectura-escalable-de-Big-Data-en-AWS-alimentada-mediante-un-sistema-de-IoT.pdf`.

## Incorporar a su repositorio actual

Si su carpeta local ya está conectada a GitHub, copie dentro de ella el contenido de la entrega **sin reemplazar su carpeta `.git` ni los archivos privados locales de configuración**. El ZIP conserva el historial recibido como respaldo; no debe sustituir un historial local más reciente. No cree otro repositorio dentro de `tesis/`.

En la Terminal de RStudio, después de verificar el PDF:

```bash
git status --short
git diff --stat
```

Revise las diferencias y los archivos nuevos desde la pestaña Git. Seleccione los archivos de la revisión, haga Commit con el mensaje `Atiende evaluaciones e integra evidencia de mayo` y luego Push. No se hicieron commits ni publicaciones desde esta entrega. El PDF de `tesis/output/` y los registros de ejecuciones se mantienen excluidos por las reglas originales de Git; conserve el PDF para el envío académico.

Los verificadores `verificar_entrega.R` y `verificar_version_final.R` corresponden a entregas anteriores y se mantienen sin cambios. Es normal que sus huellas históricas no coincidan con los capítulos corregidos; utilice el nuevo verificador para esta versión.

## Revisar los cambios

Abra `trazabilidad/actualizaciones/saneamiento_20261118/COMENTARIOS_Y_RESPUESTAS.md`. Allí se relacionan observaciones, respuesta y ubicación. En esa misma carpeta están las fuentes antes de la edición, el diff, el inventario y los CSV originales. Las capturas conservan sus datos; el rótulo «axial» se corrigió editorialmente y el pie lo declara.

## Pendientes reales

Los fuentes originales de `GetLatest`, `GetTimeseries`, `ActualizarUltimoDatoDynamo`, el frontend React/Vite y la configuración desplegable no venían en el ZIP. El Anexo D delimita este pendiente. No se reconstruyeron códigos ni se presentaron como pruebas nuevas los registros históricos. La antigua observación «7. Sección 3.2» sigue sin un enunciado que permita determinar un cambio.
