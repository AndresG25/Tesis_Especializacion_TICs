# Compara los archivos recibidos con su inventario, sin modificarlos.
# source("verificar_entrega.R", encoding = "UTF-8")

local({
  if (!file.exists("tesis/_quarto.yml") || !file.exists("Tesis_Especializacion_TICs.Rproj")) {
    stop("Abra Tesis_Especializacion_TICs.Rproj en la raíz antes de ejecutar la verificación.", call. = FALSE)
  }
  raiz <- normalizePath(".", winslash = "/", mustWork = TRUE)
  inventario <- file.path(raiz, "trazabilidad", "entrega", "inventario_integrado.csv")
  if (!file.exists(inventario)) stop("Falta trazabilidad/entrega/inventario_integrado.csv.")
  original <- read.csv(inventario, stringsAsFactors = FALSE, fileEncoding = "UTF-8")
  rutas <- file.path(raiz, original$archivo)
  existe <- file.exists(rutas)
  actual <- rep(NA_character_, nrow(original))
  actual[existe] <- unname(tools::md5sum(rutas[existe]))
  estado <- ifelse(!existe, "FALTANTE",
                   ifelse(is.na(actual), "NO_LEIBLE",
                          ifelse(actual == original$md5, "IDENTICO", "MODIFICADO")))
  resumen <- data.frame(archivo = original$archivo, estado = estado,
                        md5_entrega = original$md5, md5_actual = actual,
                        stringsAsFactors = FALSE)
  cat("Archivos comparados:", nrow(resumen), "\n")
  print(table(resumen$estado))
  diferencias <- resumen[resumen$estado != "IDENTICO", c("archivo", "estado"), drop = FALSE]
  if (nrow(diferencias)) {
    print(diferencias, row.names = FALSE)
    cat("Los archivos modificados no se restauran automáticamente.\n")
  } else {
    cat("Todos los archivos inventariados coinciden con la entrega.\n")
  }
  cat("Los archivos nuevos que usted añada no forman parte de esta comparación inicial.\n")
  invisible(resumen)
})
