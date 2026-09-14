# Ejecutar desde RStudio con Tesis_Especializacion_TICs.Rproj abierto:
# source("renderizar_tesis.R", encoding = "UTF-8")
# Requiere R, Quarto y LaTeX. Utiliza solo paquetes incluidos con R.

local({
  proyecto <- normalizePath(getwd(), winslash = "/", mustWork = TRUE)
  if (!file.exists("tesis/_quarto.yml") || !file.exists("Tesis_Especializacion_TICs.Rproj")) {
    stop("Abra Tesis_Especializacion_TICs.Rproj en la raíz y vuelva a ejecutar este archivo.", call. = FALSE)
  }

  quarto <- Sys.getenv("QUARTO_PATH", unset = "")
  if (!nzchar(quarto)) quarto <- unname(Sys.which("quarto"))
  if (!nzchar(quarto) || !file.exists(quarto)) {
    stop(paste("No se encuentra Quarto. Reinicie RStudio tras instalarlo, o defina",
               "Sys.setenv(QUARTO_PATH = 'ruta completa a quarto o quarto.cmd')."),
         call. = FALSE)
  }

  # Copia la versión activa de tesis y firmware, sin credenciales ni resultados.
  listar_fuentes <- function() {
    archivos <- list.files(".", recursive = TRUE, all.files = TRUE,
                           no.. = TRUE, include.dirs = FALSE)
    excluir <- grepl("(^|/)(output|build|managed_components|compilacion_local|trazabilidad|referencia_original|control_cambios|control_diagramacion|control_final|\\.quarto|_freeze|\\.git|\\.Rproj.user|\\.vscode)(/|$)", archivos) |
      grepl("\\.(aux|log|toc|out|bbl|blg|nlo|nls|ilg|fls|fdb_latexmk|synctex\\.gz|bin|elf|o|a|zip|key|p12|pfx|crt)$", archivos, ignore.case = TRUE) |
      grepl("(^|/)(config_local\\.h|sdkconfig|sdkconfig\\.old|\\.env.*|\\.RData|\\.Rhistory)$", archivos, ignore.case = TRUE) |
      grepl("^tesis/[^/]+\\.tex$", archivos) |
      (grepl("\\.pem$", archivos, ignore.case = TRUE) &
         !grepl("/AmazonRootCA1\\.pem$", archivos))
    sort(archivos[!excluir])
  }
  inventario <- function(archivos) {
    data.frame(archivo = archivos,
               bytes = unname(file.info(archivos)$size),
               md5 = unname(tools::md5sum(archivos)),
               stringsAsFactors = FALSE)
  }
  guardar_csv <- function(datos, ruta) {
    write.csv(datos, ruta, row.names = FALSE, fileEncoding = "UTF-8")
  }

  inicio <- Sys.time()
  identificador <- paste0(format(inicio, "%Y%m%d_%H%M%S"), "_", Sys.getpid())
  registro <- file.path(proyecto, "trazabilidad", "ejecuciones", identificador)
  if (dir.exists(registro)) stop("Ya existe el registro de esta ejecución; espere un segundo.")
  if (!dir.create(registro, recursive = TRUE)) stop("No se pudo crear el registro de ejecución.")
  writeLines(capture.output(sessionInfo()), file.path(registro, "sesion_R.txt"))
  writeLines(c(paste("Inicio:", format(inicio, "%Y-%m-%d %H:%M:%S %z")),
               paste("Proyecto:", proyecto), paste("Quarto:", quarto),
               "Comando: quarto render tesis --to pdf"),
             file.path(registro, "ejecucion.txt"), useBytes = TRUE)

  version <- function(comando, argumentos) {
    if (!nzchar(comando)) return("No disponible en PATH; Quarto puede usar su propia instalación.")
    tryCatch(suppressWarnings(system2(comando, argumentos, stdout = TRUE, stderr = TRUE)),
             error = function(e) conditionMessage(e))
  }
  writeLines(c("R:", R.version.string,
               "", "Quarto:", version(quarto, "--version"),
               "", "Pandoc:", version(quarto, c("pandoc", "--version")),
               "", "pdfLaTeX:", version(unname(Sys.which("pdflatex")), "--version")),
             file.path(registro, "versiones.txt"), useBytes = TRUE)

  # El commit identifica la base; las huellas registran también cambios sin commit.
  git <- unname(Sys.which("git"))
  git_info <- function(args) {
    if (!nzchar(git)) return("Git no está disponible en PATH.")
    salida <- version(git, args)
    if (!is.null(attr(salida, "status")) && attr(salida, "status") != 0L) {
      return("No disponible: repositorio sin inicializar o sin primer commit.")
    }
    salida
  }
  writeLines(c("Commit HEAD:", git_info(c("rev-parse", "HEAD")),
               "", "Rama:", git_info(c("branch", "--show-current")),
               "", "Estado antes del render:", git_info(c("status", "--short"))),
             file.path(registro, "estado_git.txt"), useBytes = TRUE)

  antes <- inventario(listar_fuentes())
  if (anyNA(antes$md5)) stop("No se pudieron leer todos los fuentes; revise los permisos.")
  guardar_csv(antes, file.path(registro, "fuentes_antes.csv"))
  copia <- file.path(registro, "fuentes")
  for (archivo in antes$archivo) {
    destino <- file.path(copia, archivo)
    dir.create(dirname(destino), recursive = TRUE, showWarnings = FALSE)
    if (!file.copy(archivo, destino, overwrite = FALSE, copy.date = TRUE)) {
      stop(paste("No se pudo conservar el fuente:", archivo), call. = FALSE)
    }
  }
  copia_md5 <- unname(tools::md5sum(file.path(copia, antes$archivo)))
  if (!identical(copia_md5, antes$md5)) stop("La copia de fuentes no pasó la verificación.")

  nombre_pdf <- paste0("Desarrollo-de-una-arquitectura-escalable-de-Big-Data-en-AWS-",
                       "alimentada-mediante-un-sistema-de-IoT.pdf")
  salida_pdf <- file.path("tesis", "output", nombre_pdf)
  salida_tex <- file.path("tesis", sub("\\.pdf$", ".tex", nombre_pdf))
  if (file.exists(salida_pdf)) {
    if (!file.copy(salida_pdf, file.path(registro, "tesis_previa.pdf"), overwrite = FALSE)) {
      stop("No se pudo respaldar el PDF previo. Cierre el visor e inténtelo de nuevo.")
    }
  }

  cat("Renderizando la tesis. El proceso puede tardar varios minutos.\n")
  cat("Registro: ", registro, "\n", sep = "")
  error_ejecucion <- ""
  codigo <- tryCatch(
    system2(quarto, c("render", "tesis", "--to", "pdf"),
            stdout = file.path(registro, "render_stdout.log"),
            stderr = file.path(registro, "render_stderr.log")),
    error = function(e) { error_ejecucion <<- conditionMessage(e); 999L }
  )
  despues <- inventario(listar_fuentes())
  guardar_csv(despues, file.path(registro, "fuentes_despues.csv"))
  fuentes_iguales <- identical(antes, despues)
  pdf_nuevo <- file.exists(salida_pdf) && !is.na(file.info(salida_pdf)$mtime) &&
    as.numeric(file.info(salida_pdf)$mtime) >= floor(as.numeric(inicio))
  correcto <- identical(as.integer(codigo), 0L) && pdf_nuevo

  if (correcto) {
    if (!file.copy(salida_pdf, file.path(registro, "tesis_generada.pdf"))) {
      correcto <- FALSE
      error_ejecucion <- "El PDF se generó, pero no se pudo conservar su copia en el registro."
    }
    if (file.exists(salida_tex)) {
      if (!file.copy(salida_tex, file.path(registro, "tesis_generada.tex"))) {
        correcto <- FALSE
        error_ejecucion <- "No se pudo conservar la copia del archivo TeX generado."
      }
    }
  }
  resumen <- data.frame(
    inicio = format(inicio, "%Y-%m-%d %H:%M:%S %z"),
    fin = format(Sys.time(), "%Y-%m-%d %H:%M:%S %z"),
    codigo_salida = as.integer(codigo), render_y_copia_correctos = correcto,
    fuentes_sin_cambios_durante_render = fuentes_iguales,
    pdf_md5 = if (correcto) unname(tools::md5sum(salida_pdf)) else NA_character_,
    detalle_error = error_ejecucion, stringsAsFactors = FALSE)
  guardar_csv(resumen, file.path(registro, "resultado.csv"))

  if (!correcto) {
    stop(paste("No se completó la generación y el registro de un PDF nuevo.",
               "Revise resultado.csv, render_stdout.log y render_stderr.log en:", registro),
         call. = FALSE)
  }
  if (!fuentes_iguales) {
    warning(paste("Los fuentes cambiaron durante el render. La copia inicial no garantiza",
                  "representar todos los archivos usados. Repita sin editar durante el proceso."))
  }
  cat("PDF generado: ", normalizePath(salida_pdf, winslash = "/"), "\n", sep = "")
  cat("Copia y trazabilidad: ", registro, "\n", sep = "")
})
