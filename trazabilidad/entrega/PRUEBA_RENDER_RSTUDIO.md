# Prueba del procedimiento desde R

Se ejecutaron `verificar_entrega.R` y `renderizar_tesis.R` en una sesión real de R, sobre una copia separada del proyecto. La carpeta de entrega no fue sobrescrita por esta prueba.

- R: 4.2.2 Patched; Quarto: 1.8.25; motor: pdfLaTeX de TeX Live 2023/Debian.
- La comprobación inicial encontró idénticos los 113 archivos inventariados en ese momento. Después se añadieron los informes de esta prueba y se regeneró el inventario final.
- El render terminó con código 0 y produjo un PDF de 138 páginas.
- El texto de las 138 páginas coincide con el PDF incluido en la entrega.
- Se guardaron las copias de fuentes, PDF previo, PDF generado, TeX, versiones y mensajes del proceso.
- Las huellas de los fuentes antes y después de renderizar coinciden.
- Una prueba separada con fallo simulado de Quarto, código 17, produjo un error en R, conservó el PDF previo y no lo presentó como recién generado.
- Esa prueba de error utilizó carpetas y una ruta de ejecutable con espacios.

Los resultados de la prueba real están en `resultado_prueba_R.csv`, `versiones_prueba_R.txt` y `sesion_prueba_R.txt`. La comprobación de preservación está en `verificacion_paquete.json`.

La ejecución se realizó en Linux desde R. No se ejecutó la interfaz de RStudio ni se hizo una prueba en Windows. Los scripts utilizan funciones estándar de R y un ejecutable de Quarto configurable para su instalación local.
