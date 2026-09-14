# Preparación local, sin commits ni publicaciones automáticas.
# Preferencia: clonar primero desde RStudio > New Project > Version Control > Git.
# Después de copiar la entrega a ese clon, ejecutar desde el .Rproj de la raíz:
# source("configurar_git.R", encoding = "UTF-8")

local({
  if (!file.exists("Tesis_Especializacion_TICs.Rproj") || !file.exists("tesis/_quarto.yml")) {
    stop("Abra Tesis_Especializacion_TICs.Rproj en la raíz del proyecto.", call. = FALSE)
  }
  raiz <- normalizePath(getwd(), winslash = "/", mustWork = TRUE)
  git <- unname(Sys.which("git"))
  if (!nzchar(git)) stop("Instale Git y reinicie RStudio; compruebe Tools > Global Options > Git/SVN.")
  ejecutar <- function(args, obligatorio = TRUE) {
    salida <- suppressWarnings(system2(git, args, stdout = TRUE, stderr = TRUE))
    estado <- attr(salida, "status")
    if (is.null(estado)) estado <- 0L
    if (obligatorio && estado != 0L) stop(paste(salida, collapse = "\n"), call. = FALSE)
    list(texto = salida, estado = estado)
  }
  comprobar_raiz <- ejecutar(c("rev-parse", "--show-toplevel"), obligatorio = FALSE)
  if (comprobar_raiz$estado == 0L) {
    existente <- normalizePath(comprobar_raiz$texto[1], winslash = "/", mustWork = TRUE)
    if (!identical(tolower(existente), tolower(raiz))) {
      stop("Esta carpeta está dentro de otro repositorio Git. Use una carpeta independiente o el clon correcto.")
    }
  } else {
    if (file.exists(".git") || dir.exists(".git")) {
      stop("Git no puede leer el repositorio existente. Revise el diagnóstico en la Terminal; no se modificó su configuración.")
    }
    ejecutar(c("init", "-b", "main"))
  }

  remoto <- "https://github.com/AndresG25/Tesis_Especializacion_TICs.git"
  origen <- ejecutar(c("remote", "get-url", "origin"), obligatorio = FALSE)
  permitidos <- c(remoto, sub("\\.git$", "", remoto),
                  "git@github.com:AndresG25/Tesis_Especializacion_TICs.git",
                  "git@github.com:AndresG25/Tesis_Especializacion_TICs")
  if (origen$estado == 0L) {
    if (!origen$texto[1] %in% permitidos) {
      stop("origin apunta a otro repositorio. Revíselo en la Terminal; no se reemplazó el remoto.")
    }
  } else {
    ejecutar(c("remote", "add", "origin", remoto))
  }
  # Solo completar la identidad local cuando no exista una identidad efectiva.
  if (ejecutar(c("config", "--get", "user.name"), FALSE)$estado != 0L) {
    ejecutar(c("config", "--local", "user.name", shQuote("Carlos Andrés Gómez Flórez")))
  }
  if (ejecutar(c("config", "--get", "user.email"), FALSE)$estado != 0L) {
    ejecutar(c("config", "--local", "user.email", "andres25@utp.edu.co"))
  }
  cat("Git preparado en la raíz del proyecto.\n")
  cat("Rama actual: ", paste(ejecutar(c("branch", "--show-current"))$texto, collapse = " "), "\n", sep = "")
  cat("Reabra el .Rproj si todavía no aparece la pestaña Git.\n")
  cat("Revise los cambios, haga Commit y después Push desde RStudio.\n")
  cat("Si partió de un ZIP sin clonar y GitHub ya tenía contenido, use primero el flujo de clonación descrito en docs/INICIAR_EN_RSTUDIO.md.\n")
})
