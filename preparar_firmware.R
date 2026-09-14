# Preparar una copia local con rutas compatibles con ESP-IDF.
# source("preparar_firmware.R", encoding = "UTF-8")
# preparar_firmware("ESP1")

preparar_firmware <- function(nodo, destino = NULL) {
  if (!file.exists("Tesis_Especializacion_TICs.Rproj")) {
    stop("Ejecute desde el proyecto RStudio de la raíz.", call. = FALSE)
  }
  mapa <- read.csv("docs/RUTAS_PROYECTOS.csv", stringsAsFactors = FALSE,
                   fileEncoding = "UTF-8")
  rutas <- setNames(mapa$ruta, mapa$nodo)
  if (length(nodo) != 1L || !nodo %in% names(rutas)) stop("Use ESP1, ESP2 o ESP3.")
  raiz <- normalizePath(getwd(), winslash = "/", mustWork = TRUE)
  origen <- unname(rutas[nodo])
  if (is.null(destino)) {
    destino <- file.path(raiz, "compilacion_local",
                          paste0(format(Sys.time(), "%Y%m%d_%H%M%S"), "_", Sys.getpid()), nodo)
  }
  destino <- gsub("\\\\", "/", path.expand(destino))
  if (!grepl("^(/|[A-Za-z]:/)", destino)) destino <- file.path(raiz, destino)
  if (grepl("[[:space:]()]", destino) || grepl("[^ -~]", destino)) {
    stop("La ruta de compilación debe carecer de espacios, paréntesis y tildes. Ejemplo: preparar_firmware('ESP1', 'C:/esp_tesis/ESP1').")
  }
  if (file.exists(destino) || dir.exists(destino)) {
    stop("El destino ya existe. Elija una carpeta nueva para conservar su configuración anterior.")
  }
  archivos <- list.files(origen, recursive = TRUE, all.files = TRUE,
                         no.. = TRUE, include.dirs = FALSE)
  archivos <- archivos[!grepl("(^|/)(build|managed_components|\\.git|\\.vscode)(/|$)", archivos) &
                         !grepl("(^|/)sdkconfig(\\.old)?$", archivos)]
  if (!length(archivos) || !file.exists(file.path(origen, "CMakeLists.txt"))) {
    stop("No se encontró el proyecto fuente del nodo.")
  }
  for (archivo in archivos) {
    nuevo <- file.path(destino, archivo)
    dir.create(dirname(nuevo), recursive = TRUE, showWarnings = FALSE)
    if (!file.copy(file.path(origen, archivo), nuevo, overwrite = FALSE, copy.date = TRUE)) {
      stop(paste("No se pudo copiar:", archivo))
    }
  }
  originales <- unname(tools::md5sum(file.path(origen, archivos)))
  copiados <- unname(tools::md5sum(file.path(destino, archivos)))
  if (anyNA(originales) || !identical(originales, copiados)) stop("La copia no pasó la verificación.")
  version <- mapa$esp_idf[match(nodo, mapa$nodo)]
  cat("Copia preparada: ", destino, "\n", sep = "")
  cat("Abra una terminal ESP-IDF ", version, " en esa carpeta y ejecute:\n", sep = "")
  cat("idf.py set-target esp32c6\nidf.py build\nidf.py -p COM5 flash monitor\n")
  cat("Ajuste COM5 al puerto real. ESP3 requiere su configuración local y certificados.\n")
  cat("Edite los fuentes versionados en ESP1/ESP2/ESP3 y prepare otra copia para una nueva compilación.\n")
  invisible(destino)
}
