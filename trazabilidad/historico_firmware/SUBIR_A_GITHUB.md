# Subir la entrega al repositorio privado

Repositorio indicado por el autor: [AndresG25/Tesis_Especializacion_TICs](https://github.com/AndresG25/Tesis_Especializacion_TICs).

## Opción con Git

Estas instrucciones corresponden al repositorio vacío mostrado en el pantallazo. Extraiga el ZIP de entrega y abra una terminal **dentro de `Tesis_Especializacion_TICs`**, donde se encuentran `README.md` y las tres carpetas ESP. Suba los archivos extraídos, no el ZIP.

Git debe estar instalado y la autenticación con GitHub debe estar configurada para la cuenta con acceso al repositorio. Consulte la [guía oficial para añadir un proyecto local a GitHub](https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github).

```sh
git init -b main
git add .
git status --short
git diff --cached --stat
```

Revise la selección. Deben aparecer las carpetas ESP, los README, la documentación y los archivos de configuración de ejemplo. No deben aparecer `build/`, `config_local.h`, `private_key.key` ni `device_cert.crt`.

Después de revisar:

```sh
git commit -m "Entrega del firmware ESP1 ESP2 ESP3 y documentacion de la tesis"
git remote add origin https://github.com/AndresG25/Tesis_Especializacion_TICs.git
git remote -v
git push -u origin main
```

Si Git solicita su identidad de autor, configúrela en este repositorio y repita el commit:

```sh
git config user.name "Carlos Andrés Gómez Flórez"
git config user.email "andres25@utp.edu.co"
```

Si ya inicializó esta carpeta, omita `git init`. Si el remoto `origin` ya existe, compruebe su URL antes de continuar; no lo agregue de nuevo. Si GitHub ya contiene cambios, descárguelos e intégrelos antes de subir: estas instrucciones no requieren `--force`.

## Opción desde la web

En el repositorio vacío, utilice **uploading an existing file**. En un repositorio con contenido, la opción habitual es **Add file → Upload files**. Seleccione el contenido extraído de la carpeta de entrega, incluidas las carpetas ESP y los archivos de la raíz.

Compruebe que `.gitignore` y `.gitattributes` estén incluidos: algunos exploradores ocultan los archivos cuyo nombre empieza por punto. El README debe quedar directamente en la raíz del repositorio, sin un nivel de carpeta adicional.

La web no aplica su `.gitignore` local al seleccionar archivos. Use el paquete limpio entregado; no arrastre sus carpetas originales con credenciales o resultados de compilación.

## Verificación de la entrega

Abra el README en GitHub y compruebe que sus enlaces permiten navegar a `ESP1`, `ESP2`, `ESP3` y a la documentación. Confirme que la etiqueta de visibilidad continúe indicando **Private**.

Para que el director y los evaluadores puedan consultar el material, debe concederles acceso al repositorio desde su configuración de colaboradores. Compartir el enlace no concede acceso por sí solo. Pueden solicitarlo a [andres25@utp.edu.co](mailto:andres25@utp.edu.co), indicando su usuario de GitHub.

Guarde el identificador del commit entregado para relacionarlo con la versión final de la tesis:

```sh
git rev-parse HEAD
```

Este paquete se prepara para que el autor efectúe la subida. No contiene un historial Git ni presupone que los archivos ya estén publicados.
