# Recuperación del frontend — informe actualizado

## Resultado

El ZIP del portátil contiene el proyecto editable React/Vite y una compilación dist/. Los cinco archivos de esa compilación son idénticos por tamaño y SHA-256 a los cinco archivos del respaldo descargado de S3. Los valores de entorno del frontend coinciden con las URL de las Lambda recuperadas.

Esto permite vincular el respaldo del portátil con el sitio publicado. La igualdad se refiere a los archivos dist ya existentes: no equivale a haber recompilado los fuentes durante esta revisión.

## Verificaciones y límites

| Comprobación | Resultado |
|---|---|
| Integridad CRC del ZIP recibido | Correcta. |
| Dist del portátil frente al respaldo S3 | Cinco archivos idénticos de cinco. |
| Sintaxis JS/JSX | Cinco módulos analizados con Espree 11.2.0; correctos. |
| Imports relativos de esos módulos | Archivos presentes. |
| Dependencias instaladas presentes frente al lock | Sin diferencias de versión detectadas. |
| Configuración frontend | Tres variables recuperadas; .env y .env.production idénticos. |
| Preservación de aplicación y configuración | Quince archivos copiados sin modificar; dist preservado en publicado/. |
| ESLint del original | Tres errores de reglas, conservados y documentados. |
| Recompilación | Pendiente de ejecutar localmente. Los módulos nativos del respaldo son de Windows. |
| Prueba contra AWS | No realizada. |

El import React no utilizado aparece en App.jsx:1. Las dos observaciones react-hooks/set-state-in-effect corresponden a Historico.jsx:121 y :160. No son errores de sintaxis; tampoco se declara aprobada la revisión ESLint. Se preservó el original para no modificar el sistema que respalda la tesis.

## Archivos y trazabilidad

El proyecto está en cloud/frontend/turbina-dashboard/. Los fuentes, public/, index.html, package.json, package-lock.json y configuraciones Vite/ESLint conservan sus bytes originales. El README se adaptó al proyecto; el texto original se preserva en trazabilidad. La regla Git para incluir .env.example se documenta conservando también el .gitignore original.

En trazabilidad/recuperacion_frontend_fuentes_20261120/ están manifest_origen.json, comparacion_dist_s3.csv, inspeccion_sintaxis.json, eslint_original.json y configuracion_publicada.json. Las versiones previas de la documentación se preservan en documentacion_antes_de_recuperar_fuentes/. Sus menciones a fuentes pendientes describen el estado anterior a esta recuperación.

Se excluyeron las dependencias instaladas de node_modules para la entrega al repositorio. El ZIP original de 31.899.276 bytes debe conservarse como respaldo local del autor; su SHA-256 está registrado en manifest_origen.json.

## Relación con la observación A4

La recuperación de los tres fuentes Lambda y del proyecto React/Vite está atendida en el material recibido. Para cerrar la incorporación al repositorio, falta que el autor integre el complemento y confirme commit/push. La reproducibilidad integral aún requiere ejecutar la compilación local y completar la configuración, recursos y permisos AWS que faltan.

El valor desplegado de MAX_QUERY_MINUTES no se recuperó con la exportación Lambda; por tanto, la presencia de 24 horas y 7 días en la interfaz no prueba que esos rangos estén habilitados. Tampoco se alteró la discrepancia documentada de DDB_TABLE frente a las variables leídas por GetLatest.

No se cambian los capítulos de la tesis en esta entrega. El Anexo D deberá describir las carpetas y los códigos realmente incorporados, sin presentar los pendientes de reproducción como ya verificados.

El ZIP también conservaba tres commits originales del 15 de mayo de 2026. Los quince archivos cotejados coinciden con el último commit: `7a3c2020f49fe2c9ebf218faec7b890af6b7aa5a`. El historial se preservó en `trazabilidad/recuperacion_frontend_fuentes_20261120/historial_frontend_original.bundle`, acompañado de su inventario. Se conserva como respaldo; no se incorpora un repositorio Git anidado al proyecto de la tesis.
