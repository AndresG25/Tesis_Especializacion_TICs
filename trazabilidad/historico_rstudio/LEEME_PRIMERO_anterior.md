> **Para abrir esta entrega en RStudio y conservar el registro de cada render, consulte primero [ABRIR_EN_RSTUDIO.md](ABRIR_EN_RSTUDIO.md).**

# Proyecto Quarto — anexos vinculados al repositorio

Esta entrega adapta los anexos de la tesis para consultar el código de los nodos ESP en el repositorio privado:

[AndresG25/Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs)

**Autor:** Carlos Andrés Gómez Flórez  
**Contacto:** [andres25@utp.edu.co](mailto:andres25@utp.edu.co)

## Abrir y renderizar en RStudio

1. Extraiga este ZIP en una carpeta nueva.
2. Abra `tesis/tesis.Rproj`.
3. En la consola de RStudio, ejecute:

```r
getwd()
stopifnot(file.exists("_quarto.yml"))
system("quarto render --to pdf")
```

Requiere Quarto y una distribución LaTeX con las dependencias del proyecto. El PDF se genera en `tesis/output/`; se incluye un PDF ya renderizado y revisado en esa carpeta. Una instalación distinta de Quarto o LaTeX puede producir variaciones de paginación.

## Cambios de esta entrega

- El Anexo A introduce el repositorio, su condición de acceso privado y el correo del autor. Describe la carpeta ESP1 y sus códigos.
- Los anexos B y C describen las carpetas ESP2 y ESP3 y enlazan sus fuentes en GitHub.
- El Anexo D aclara que Lambda y frontend no se incluyeron entre los códigos adjuntos y remite al alcance documentado del repositorio.
- No se modificaron los capítulos, las figuras, las tablas, la bibliografía ni la configuración de formato de la versión final ajustada anterior.

Los enlaces del PDF son activos. Los vínculos a carpetas requieren que el autor suba el paquete preparado a la rama `main` del repositorio y conceda acceso a sus evaluadores.

## Archivos de la entrega

- `tesis/`: proyecto completo con anexos actualizados.
- `tesis/output/`: PDF actualizado.
- `control_cambios/ACTUALIZACION_ANEXOS_REPOSITORIO.md`: descripción y ruta de revisión de esta intervención.
- `control_cambios/verificacion_anexos_repositorio.json`: comprobación de preservación y enlaces.
- `referencia_original/`: referencias originales conservadas de entregas anteriores.

Los informes anteriores dentro de `control_cambios/` documentan intervenciones pasadas. Sus estados de validación corresponden a la fecha de cada informe; para esta entrega utilice los dos archivos indicados arriba.

El código para GitHub se entrega en un ZIP separado llamado `Repositorio_Tesis_Especializacion_TICs.zip`. Este ZIP del proyecto Quarto no incluye las tres carpetas de firmware ni credenciales de dispositivo.

El comentario 7 del director continúa sin poder evaluarse porque el texto disponible solo indica «Sección 3.2».
