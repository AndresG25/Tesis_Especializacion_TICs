# RStudio, Git y GitHub: primera carga y trabajo diario

## 1. Clonar desde la ventana del pantallazo

En **New Project Wizard**, seleccione **Version Control**, después **Git**. Complete:

| Campo | Valor |
| --- | --- |
| Repository URL | `https://github.com/AndresG25/Tesis_Especializacion_TICs.git` |
| Project directory name | `Tesis_Especializacion_TICs` |
| Create project as subdirectory of | Una carpeta padre, por ejemplo `C:/Proyectos`. |

Pulse **Create Project** y autentíquese si GitHub lo solicita. La cuenta debe tener acceso al repositorio privado. Clone en una carpeta nueva; no use como destino una carpeta que ya contiene la extracción del ZIP. Si ya tiene un clon de este repositorio, ábralo y actualícelo con **Pull** antes de incorporar esta entrega.

Este flujo corresponde a la [clonación desde RStudio documentada por Posit](https://docs.posit.co/ide/user/ide/guide/tools/version-control.html#creating-a-new-project-based-on-a-remote-git-repository).

## 2. Copiar la entrega dentro del clon

Extraiga el ZIP aparte. Abra la carpeta extraída `Tesis_Especializacion_TICs` y copie **su contenido** dentro de la carpeta clonada. Al terminar, `Tesis_Especializacion_TICs.Rproj`, `README.md`, `ESP1`, `ESP2`, `ESP3` y `tesis` deben quedar al mismo nivel.

Conserve la carpeta `.git` que creó RStudio. El ZIP no incluye otra carpeta `.git`. No cree repositorios independientes dentro de la tesis o de cada ESP.

Si GitHub ya tenía los fuentes anteriores directamente bajo `ESP1/main`, `ESP2/main` o `ESP3/main`, revise la migración después de copiar: ahora están dentro de las carpetas descriptivas. Los archivos antiguos pueden quedar duplicados al copiar; retire únicamente las copias antiguas ya sustituidas, tras comprobar sus diferencias. Conserve cualquier modificación propia posterior a la entrega anterior. Git mostrará las altas, bajas y posibles renombrados antes del commit.

La estructura correcta tiene una sola raíz. Evite anidar repetidamente carpetas como `tesis_comentarios_director/tesis_comentarios_director/tesis`.

## 3. Abrir el proyecto y comprobar Git

Abra `Tesis_Especializacion_TICs.Rproj` desde la raíz del clon. Puede cerrar el proyecto que RStudio creó al clonar y abrir este archivo; reutiliza el mismo repositorio Git.

En la consola de RStudio:

```r
source("configurar_git.R", encoding = "UTF-8")
```

Si no aparece la pestaña Git, vuelva a abrir el proyecto. Compruebe **Tools → Global Options → Git/SVN** y la ruta de Git. Estas opciones se describen en la [guía de control de versiones de Posit](https://docs.posit.co/ide/user/ide/guide/tools/version-control.html).

El script no descarga, no hace commits y no hace push. Si se ejecuta en una extracción sin Git, inicia una rama local `main`; para un remoto que ya tiene contenido, utilice la clonación para conservar su historia.

## 4. Guardar la primera versión integrada

1. En **Git**, seleccione un archivo y abra **Diff** para revisarlo.
2. Marque **Staged** en los archivos que quiere guardar: tesis, nodos, scripts y documentación.
3. Pulse **Commit** y escriba un mensaje, por ejemplo: `Integra tesis y firmware renombrado en un único proyecto RStudio`.
4. Confirme el commit y después utilice **Push**.

Si es la primera subida de una rama `main` sin seguimiento remoto, ejecute una vez en la pestaña **Terminal**:

```sh
git push -u origin main
```

Si el remoto tiene cambios que no están en su equipo, recupérelos e intégrelos; no utilice `--force`. Consulte también la [guía oficial de GitHub](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github).

Las credenciales y los resultados generados quedan excluidos mediante `.gitignore`. Haga la subida con Git desde RStudio; no arrastre las carpetas originales con certificados o compilaciones al formulario web.

## 5. Trabajo diario

- Utilice **Pull** si hay cambios remotos y su trabajo local está guardado.
- Edite los QMD en `tesis/` o los fuentes en las carpetas descriptivas de ESP1, ESP2 y ESP3.
- Ejecute `source("renderizar_tesis.R", encoding = "UTF-8")` y revise el PDF.
- Revise **Diff**, haga un commit con un mensaje que describa el cambio y utilice **Push**.
- Consulte **History** para revisar versiones anteriores.

RStudio administra Git y permite editar C. La compilación del firmware se realiza con ESP-IDF en su terminal correspondiente.

## Qué se guarda en Git

| Se versiona | Se conserva localmente y se excluye de Git |
| --- | --- |
| QMD, bibliografía, estilos, imágenes y figuras editables. | `tesis/output/` y auxiliares de LaTeX. |
| Fuentes y componentes de ESP1, ESP2 y ESP3. | `build/`, `sdkconfig` generado y `compilacion_local/`. |
| Configuración de referencia, plantillas, README y scripts R. | `config_local.h`, contraseñas, llaves y certificados de dispositivo. |
| Informes, inventarios y referencias originales indicadas. | `trazabilidad/ejecuciones/`, sesiones y cachés de RStudio. |

Los PDF de figuras en `tesis/fig/` sí se versionan porque son recursos. El PDF final se incluye en el ZIP y se puede regenerar desde el clon.
