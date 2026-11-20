> Nota de actualización: este informe conserva el estado del rescate Lambda anterior a la recepción de los fuentes React/Vite. Estos ya fueron recuperados; el estado conjunto vigente está en RECUPERACION_FRONTEND_20261120.md.

# Recuperación de Lambda — control documental

## Resultado

- Tres fuentes Python recuperados y conservados sin modificación.
- Runtime Python 3.12 y handler lambda_function.lambda_handler en los tres casos.
- Veintidós huellas SHA-256 del respaldo coinciden con sus archivos.
- Los tres paquetes de código coinciden con los hashes CodeSha256 exportados de AWS.
- No hay capas externas declaradas, alias ni versiones publicadas en este respaldo.
- Sintaxis Python comprobada mediante análisis estático. No se ejecutaron funciones contra AWS.

## Relación con la observación del evaluador

La recuperación atiende la parte de código Lambda de la observación A4. Todavía no debe marcarse como completamente cerrada: falta el frontend original y completar la configuración y los pasos de reproducción. El Anexo D se actualizará para distinguir los componentes efectivamente incorporados de los pendientes. Esta entrega no reemplaza capítulos de la tesis.

## Configuración que necesita confirmación

Los valores de entorno se omitieron deliberadamente en la exportación. Los ejemplos mantienen sus nombres y marcan los valores por completar. En GetLatest, la clave exportada DDB_TABLE no es leída por el código, que utiliza DDB_TABLE_NAME, TABLE_NAME o turbina_latest como respaldo. La discrepancia quedó documentada; no se intervino el sistema.

Las dos Function URLs exportadas corresponden a GetLatest y GetTimeseries, con AuthType NONE. Los encabezados CORS se generan en los fuentes Python y las configuraciones CORS de las URL están vacías. ActualizarUltimoDatoDynamo no tiene Function URL en el respaldo. Esto describe la configuración recuperada; no es un cambio propuesto ni una evaluación integral de seguridad.

## Integración local

1. Conservar el ZIP de recuperación original como evidencia.
2. El complemento contiene carpetas cloud/, docs/ y trazabilidad/ para agregarlas a la raíz del proyecto Tesis_Especializacion_TICs, al nivel de ESP1, ESP2, ESP3 y tesis.
3. Si ya existe alguno de estos archivos, revisar las diferencias antes de reemplazarlo. No se incluye ninguna modificación de _quarto.yml, capítulos, figuras o scripts de renderizado.
4. Completar el rescate del frontend antes de preparar el cierre conjunto del Anexo D y la respuesta a los evaluadores.
5. Revisar los cambios en Git antes de commit y push; la recuperación por sí sola no actualiza GitHub.

Las huellas de los fuentes y las fechas originales se encuentran en trazabilidad/recuperacion_lambda_20261120/verificacion_fuentes.json. El inventario de todo el complemento está en SHA256SUMS.txt.
