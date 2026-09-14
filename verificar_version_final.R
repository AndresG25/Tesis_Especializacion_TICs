# Ejecutar desde el proyecto RStudio de la raíz, después de copiar la actualización.
# source("verificar_version_final.R", encoding = "UTF-8")
local({
  ruta <- "trazabilidad/actualizaciones/final_20260914/inventario_final.csv"
  if (!file.exists("Tesis_Especializacion_TICs.Rproj") ||
      !file.exists("tesis/_quarto.yml") || !file.exists(ruta)) {
    stop("Abra el proyecto raíz y copie allí el contenido de la actualización final.", call. = FALSE)
  }
  tabla <- read.csv(ruta, fileEncoding = "UTF-8", stringsAsFactors = FALSE,
                    colClasses = "character", check.names = FALSE, na.strings = NULL)
  huella_lf <- function(ruta) {
    bytes <- readBin(ruta, "raw", n = file.info(ruta)$size)
    if (length(bytes) >= 3L && identical(bytes[1:3], as.raw(c(239, 187, 191)))) {
      bytes <- bytes[-(1:3)]
    }
    texto <- gsub("\r\n", "\n", rawToChar(bytes), fixed = TRUE)
    temporal <- tempfile("tesis_final_huella_")
    on.exit(unlink(temporal))
    writeBin(charToRaw(texto), temporal)
    unname(tools::md5sum(temporal))
  }
  existe <- file.exists(tabla$archivo)
  huellas <- rep(NA_character_, nrow(tabla))
  huellas[existe] <- unname(tools::md5sum(tabla$archivo[existe]))
  tabla$estado <- ifelse(!existe, "FALTANTE",
                         ifelse(is.na(huellas), "NO_LEIBLE",
                                ifelse(huellas == tabla$md5, "IDENTICO", "MODIFICADO")))
  for (i in which(tabla$estado == "MODIFICADO" & nzchar(tabla$md5_lf))) {
    if (identical(huella_lf(tabla$archivo[i]), tabla$md5_lf[i])) tabla$estado[i] <- "IDENTICO"
  }
  print(table(tabla$estado))
  problemas <- tabla[tabla$estado != "IDENTICO", c("archivo", "estado"), drop = FALSE]
  if (nrow(problemas)) {
    print(problemas, row.names = FALSE)
    stop("Hay archivos faltantes o distintos de la entrega final. Revise los indicados.", call. = FALSE)
  }
  cat("Versión final verificada: los archivos coinciden con la entrega.\n")
  cat("Ahora ejecute source('renderizar_tesis.R', encoding = 'UTF-8').\n")
  cat("La verificación confirma la copia; revise después el PDF y haga Commit y Push.\n")
})
