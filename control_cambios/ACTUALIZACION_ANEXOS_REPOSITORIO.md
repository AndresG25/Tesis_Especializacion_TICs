# Actualización de anexos — repositorio del sistema

## Solicitud atendida

Se adaptó la entrega final anterior para consultar el código en el repositorio privado [AndresG25/Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs), con enlaces activos y contacto del autor: [andres25@utp.edu.co](mailto:andres25@utp.edu.co).

Los anexos describen los archivos suministrados por el autor en `ESP1.zip`, `ESP2.zip` y `ESP3.zip`. La entrega de firmware preparada para GitHub se proporciona en un ZIP separado.

## Ruta de revisión del PDF incluido

| Elemento | Página impresa | Página del visor PDF | Ajuste |
| --- | --- | --- | --- |
| Anexo A | 110 | 123 | Introducción del repositorio, acceso privado, correo y descripción de ESP1. |
| Anexo B | 111 | 124 | Descripción de ESP2, sus componentes y el envío JSON por UART; enlace a la carpeta y correo. |
| Anexo C | 112 | 125 | Descripción de ESP3, recepción UART, MQTT/TLS y configuración local; enlace a la carpeta y correo. |
| Anexo D | 113 | 126 | Aclaración de que Lambda y frontend no estaban entre los fuentes adjuntos y enlace al alcance de la entrega. |

Estas páginas corresponden al PDF incluido, generado con el entorno de esta revisión. La paginación puede variar al renderizar con otras versiones de Quarto o LaTeX.

## Preservación

La base fue `tesis_version_final_ajustada.zip`. Solo se editaron los cuatro archivos QMD de anexos y la guía externa `LEEME_PRIMERO.md`; se regeneraron el `.tex` y el PDF. Se añadieron este informe y su comprobación JSON.

Los capítulos, figuras, tablas, bibliografía, estilos y `_quarto.yml` permanecen idénticos por bytes a la base. El texto y la representación visual de las 134 páginas que no corresponden a los anexos son iguales a los del PDF anterior generado en el mismo entorno. El nuevo PDF conserva 138 páginas.

## Verificación

- Render completo con Quarto y LaTeX terminado correctamente.
- Anexos inspeccionados como imágenes, con texto legible dentro de los márgenes.
- Destinos de los enlaces al repositorio, carpetas y correo comprobados en las anotaciones del PDF.
- Comparación de todos los archivos incluidos originalmente y de las páginas del PDF fuera de los anexos.

Los resultados detallados constan en `verificacion_anexos_repositorio.json`.

## Alcance pendiente del repositorio

El autor debe subir el contenido del ZIP de firmware a la rama `main` del repositorio y habilitar el acceso de los evaluadores. La preparación de esta entrega no publicó archivos en GitHub.

Para que el repositorio reúna también los componentes de nube y visualización, faltan los fuentes de las funciones Lambda, del frontend y las configuraciones desplegables de AWS. El Anexo D declara esa disponibilidad real; no se añadieron implementaciones supuestas.

El comentario 7 del director conserva su estado anterior: el texto disponible «Sección 3.2» no permite identificar un ajuste concreto.
