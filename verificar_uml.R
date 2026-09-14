# Ejecutar desde la raíz del proyecto RStudio.
# source("verificar_uml.R", encoding = "UTF-8")
local({
  ruta <- "docs/uml/inventario_archivos.csv"
  if (!file.exists("tesis/_quarto.yml") || !file.exists(ruta)) {
    stop("Abra el proyecto raíz e instale primero la actualización UML.", call. = FALSE)
  }
  tabla <- read.csv(ruta, fileEncoding = "UTF-8", stringsAsFactors = FALSE,
                    colClasses = "character", check.names = FALSE, na.strings = NULL)
  huella_lf <- function(ruta) {
    bytes <- readBin(ruta, "raw", n = file.info(ruta)$size)
    if (length(bytes) >= 3L && identical(bytes[1:3], as.raw(c(239, 187, 191)))) bytes <- bytes[-(1:3)]
    texto <- gsub("\r\n", "\n", rawToChar(bytes), fixed = TRUE)
    temp <- tempfile("uml_huella_")
    on.exit(unlink(temp))
    writeBin(charToRaw(texto), temp)
    unname(tools::md5sum(temp))
  }
  huellas <- unname(tools::md5sum(tabla$archivo))
  tabla$estado <- ifelse(!file.exists(tabla$archivo), "FALTANTE",
                         ifelse(is.na(huellas), "NO_LEIBLE",
                                ifelse(huellas == tabla$md5, "IDENTICO", "MODIFICADO")))
  for (i in which(tabla$estado == "MODIFICADO" & nzchar(tabla$md5_lf))) {
    if (identical(huella_lf(tabla$archivo[i]), tabla$md5_lf[i])) tabla$estado[i] <- "IDENTICO"
  }
  print(table(tabla$estado))
  problemas <- tabla[tabla$estado != "IDENTICO", c("archivo", "estado"), drop = FALSE]
  if (nrow(problemas)) {
    print(problemas, row.names = FALSE)
    stop("Hay diferencias frente a la actualización UML entregada. Revise los archivos indicados.", call. = FALSE)
  }
  cat("Actualización UML verificada. Los archivos coinciden con la entrega.\n")
  cat("El PDF y las ejecuciones locales se revisan con renderizar_tesis.R.\n")
})
