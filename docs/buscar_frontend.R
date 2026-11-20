# Busca posibles proyectos React/Vite en el computador.
# Solo lee nombres de carpetas y package.json; no modifica archivos ni instala nada.
# Ejecutar desde la consola de RStudio:
# source("docs/buscar_frontend.R", encoding = "UTF-8")

buscar_frontend_tesis <- function(
    raices = Sys.getenv("USERPROFILE", unset = path.expand("~")),
    max_carpetas = 100000L) {
  if (!length(raices) || anyNA(raices) || any(!nzchar(raices))) {
    stop("Indique al menos una carpeta en raices.")
  }
  if (length(max_carpetas) != 1L || is.na(max_carpetas) || max_carpetas < 1) {
    stop("max_carpetas debe ser un numero positivo.")
  }
  omitidas <- tolower(c(
    "node_modules", ".git", ".svn", ".hg", ".Rproj.user", ".quarto",
    ".cache", ".npm", "AppData", "renv", ".venv", "venv",
    "__pycache__", "build", "dist", "$RECYCLE.BIN", "System Volume Information"
  ))
  pendientes <- unique(raices[dir.exists(raices)])
  if (!length(pendientes)) stop("Ninguna de las carpetas indicadas existe.")
  visitadas <- new.env(hash = TRUE, parent = emptyenv())
  hallazgos <- list()
  revisadas <- 0L
  cat("Buscando proyectos React/Vite. Puede tardar varios minutos.\n")
  cat("Carpetas iniciales:\n", paste(pendientes, collapse = "\n"), "\n", sep = "")

  while (length(pendientes) && revisadas < max_carpetas) {
    carpeta <- pendientes[[length(pendientes)]]
    pendientes <- pendientes[-length(pendientes)]
    canonica <- normalizePath(carpeta, winslash = "/", mustWork = FALSE)
    clave <- if (.Platform$OS.type == "windows") tolower(canonica) else canonica
    if (exists(clave, envir = visitadas, inherits = FALSE)) next
    assign(clave, TRUE, envir = visitadas)
    revisadas <- revisadas + 1L

    paquete <- file.path(carpeta, "package.json")
    if (file.exists(paquete)) {
      contenido <- tryCatch(
        paste(readLines(paquete, warn = FALSE, encoding = "UTF-8"), collapse = "\n"),
        error = function(e) ""
      )
      react <- grepl('"react"\\s*:', contenido, perl = TRUE)
      vite <- grepl('"vite"\\s*:', contenido, perl = TRUE)
      turbina <- grepl("turbina", paste(carpeta, contenido), ignore.case = TRUE)
      if (react || vite || turbina) {
        hallazgos[[length(hallazgos) + 1L]] <- data.frame(
          carpeta = canonica, react = react, vite = vite,
          src = dir.exists(file.path(carpeta, "src")),
          stringsAsFactors = FALSE
        )
      }
    }

    entradas <- tryCatch(
      list.files(carpeta, all.files = TRUE, full.names = TRUE,
                 recursive = FALSE, no.. = TRUE, include.dirs = TRUE),
      error = function(e) character()
    )
    if (length(entradas)) {
      info <- suppressWarnings(file.info(entradas))
      subcarpetas <- entradas[!is.na(info$isdir) & info$isdir]
      subcarpetas <- subcarpetas[!tolower(basename(subcarpetas)) %in% omitidas]
      if (length(subcarpetas)) {
        enlaces <- suppressWarnings(Sys.readlink(subcarpetas))
        subcarpetas <- subcarpetas[is.na(enlaces) | enlaces == ""]
        pendientes <- c(pendientes, subcarpetas)
      }
    }
    if (revisadas %% 1000L == 0L) {
      cat("Carpetas revisadas:", revisadas, "| Proyectos encontrados:", length(hallazgos), "\n")
      flush.console()
    }
  }

  resultado <- if (length(hallazgos)) do.call(rbind, hallazgos) else data.frame(
    carpeta = character(), react = logical(), vite = logical(), src = logical()
  )
  cat("\nCarpetas revisadas:", revisadas, "\n")
  if (length(pendientes)) cat("Se alcanzo el limite de carpetas; la busqueda no fue completa.\n")
  if (nrow(resultado)) {
    resultado <- resultado[order(!resultado$src, !resultado$react, !resultado$vite, resultado$carpeta), ]
    rownames(resultado) <- NULL
    print(resultado, row.names = FALSE)
    cat("\nComparta esta lista para identificar el dashboard correcto.\n")
  } else {
    cat("No se encontraron candidatos en las carpetas accesibles revisadas.\n")
    cat('Si el proyecto estaba en otra unidad, pruebe: buscar_frontend_tesis(raices = "D:/")\n')
  }
  cat("Las carpetas sin permiso de lectura o fuera de las raices no se revisaron.\n")
  cat("No se modificaron archivos ni se consulto AWS.\n")
  invisible(resultado)
}

resultados_frontend_tesis <- buscar_frontend_tesis()
