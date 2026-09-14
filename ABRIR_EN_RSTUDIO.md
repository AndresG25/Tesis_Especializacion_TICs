# Abrir y regenerar la tesis en RStudio

1. Para trabajar con GitHub, clone el repositorio y copie el contenido del ZIP dentro del clon, según [Iniciar en RStudio](docs/INICIAR_EN_RSTUDIO.md).
2. Abra **Tesis_Especializacion_TICs.Rproj en la raíz**. ESP1, ESP2, ESP3 y tesis estarán dentro del mismo proyecto.
3. En la consola de RStudio ejecute:

```r
source("renderizar_tesis.R", encoding = "UTF-8")
```

El PDF se genera en `tesis/output/`. Cada ejecución crea una carpeta en `trazabilidad/ejecuciones/` con copias de los fuentes activos, PDF previo, PDF generado, TeX, versiones, estado de Git y mensajes del proceso. Espere a que termine antes de editar. Cierre el PDF si su visor de Windows impide reemplazarlo.

Para comprobar los archivos del ZIP antes de editarlos:

```r
source("verificar_entrega.R", encoding = "UTF-8")
```

Las modificaciones posteriores se muestran como diferencias; el script no restaura ni elimina archivos. El inventario inicial se conserva aunque usted siga trabajando y creando commits.

## Requisitos

Utilice su instalación de R, RStudio, Quarto y LaTeX. Los scripts usan funciones incluidas con R; no requieren paquetes adicionales ni instalan software automáticamente. El trabajo de composición lo realizan Quarto y LaTeX.

Si Quarto no aparece, compruebe `Sys.which("quarto")`. Puede indicar su instalación real antes de ejecutar el script:

```r
Sys.setenv(QUARTO_PATH = "C:/Program Files/Quarto/bin/quarto.cmd")
source("renderizar_tesis.R", encoding = "UTF-8")
```

Ajuste esa ruta a su equipo. Si necesita instalar LaTeX, la [documentación oficial de Quarto](https://quarto.org/docs/output-formats/pdf-basics.html) explica TinyTeX. No lo reinstale si su entorno ya renderiza la tesis.

El comando directo desde la raíz, sin el registro automático de esta entrega, es:

```r
system("quarto render tesis --to pdf")
```

Los informes previos conservan las rutas y comandos de su propia etapa. Para esta organización utilice el `.Rproj` y los scripts de la raíz. La paginación puede variar entre versiones de LaTeX; los registros permiten identificar el entorno usado.
