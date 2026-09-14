# Integración de la tesis y el firmware en RStudio/Git

## Resultado

Se integraron la tesis y los tres nodos bajo una única raíz `Tesis_Especializacion_TICs/`. Se conserva la organización adjunta:

| Nodo | Nombre interno exacto |
| --- | --- |
| ESP1 | Firmware nodo ESP1 - Variables Generador |
| ESP2 | Firmware nodo 2 - Variables ambientables e integración con ESP1 |
| ESP3 | Firmware nodo 3 e integración con AWS_IoT_Core |

El archivo `Tesis_Especializacion_TICs.Rproj` abre toda la raíz. Se archivaron como texto los puntos de entrada R anteriores para evitar proyectos operativos anidados.

## Cambios y preservación

Los anexos A–C se adaptaron a los nuevos nombres y enlaces; el Anexo A identifica además el proyecto de la tesis dentro del mismo repositorio. Los capítulos, las figuras, la bibliografía, el formato y el Anexo D conservan sus fuentes. El PDF regenerado mantiene 138 páginas; únicamente cambia el texto de las páginas PDF 123–125, correspondientes a las páginas impresas 110–112.

Los fuentes de los nuevos ZIP coinciden con los recibidos previamente. Se mantienen las tres intervenciones de configuración ya aplicadas: separación de credenciales de ESP3, configuración opcional de la biblioteca Wi-Fi de ESP2 y comprobación de archivos locales en CMake de ESP3. No se modifican los algoritmos de adquisición, integración o transmisión.

Se actualizaron los README y el mapa de rutas. Los scripts R permiten configurar Git localmente, renderizar desde la raíz, registrar el commit y el estado de trabajo, comprobar la entrega y preparar copias de compilación con rutas cortas. Las copias de compilación conservan los bytes de sus fuentes.

## Verificación

Se probaron los scripts en una sesión real de R sobre una copia separada. La configuración Git pudo ejecutarse dos veces sin reinicializar la historia; se verificaron la rama local y el remoto esperado, sin conexión remota. Se prepararon y compararon las copias de los tres nodos y se renderizó el documento desde R.

La prueba de Git verificó la inclusión de fuentes, figuras y plantillas, y la exclusión de credenciales, compilaciones y registros locales. Los fuentes añadidos al índice conservaron sus bytes. La copia de prueba no realizó commits ni envió contenido a GitHub; su carpeta `.git` no se incluye en el ZIP.

El último render terminó con código 0 y conservó iguales las huellas de los fuentes antes y después. Los anexos se revisaron visualmente y sus enlaces se comprobaron en el PDF. La configuración privada se excluyó del paquete, incluidas las llaves de dispositivo. Los nombres de tópico y cliente ya citados en el texto de la tesis se conservan como parte de su documentación.

Entorno de prueba: R 4.2.2 Patched, Quarto 1.8.25 y TeX Live 2023/Debian en Linux. No se ejecutó la interfaz de RStudio, Windows ni una compilación de ESP-IDF sobre hardware.

Los resultados están en `verificacion_integracion.json`, `resultado_render_integrado.csv` y `render_integrado.log`. Las diferencias de los anexos están en `diferencias_integracion_anexos.diff`.

## Continuar desde el equipo del autor

El flujo vigente está en `docs/INICIAR_EN_RSTUDIO.md`: clonar desde Version Control, copiar el contenido de esta entrega al clon, abrir el `.Rproj` de la raíz, revisar diferencias y hacer Commit/Push. El ZIP reúne el paquete para GitHub y el proyecto para regenerar la tesis localmente.
