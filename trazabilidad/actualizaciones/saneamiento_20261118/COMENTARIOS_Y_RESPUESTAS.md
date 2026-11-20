# Comentarios y respuestas — revisión del 18 de noviembre de 2026

Base: ZIP `Tesis_Especializacion_TICs.zip`, commit `64c0305`. Se conserva la organización del proyecto y la secuencia de capítulos, secciones y anexos. No se modificaron los 41 archivos originales de ESP1, ESP2 y ESP3. Las evaluaciones firmadas y los CSV originales se conservan en `evidencias/`.

Esta revisión atiende documentalmente las dos evaluaciones y verifica los comentarios anteriores disponibles. No constituye una nueva campaña experimental. **La incorporación del software cloud/frontend requiere los fuentes originales; la observación antigua “Sección 3.2” requiere su enunciado.**

| ID | Observación | Estado | Ubicación | Respuesta |
|---|---|---|---|---|
| Y1 | Yurley: alcance de validación | Atendido | Resumen, Abstract, capítulos 5, 6 y 7 | Se distingue integración funcional de certificación de disponibilidad, seguridad y rendimiento. |
| Y2 | Yurley: prueba de 24 horas | Atendido con evidencia histórica | 6.3.3; tabla 6.7 | Inventario de S3: 7.546 objetos en la ventana de 24 h y 9.860 en 31 h 21 min 31 s. No se afirma tasa de pérdida ni latencia de transmisión. |
| Y3 | Yurley: contraste Cost Explorer | Atendido con límites | 6.3.3; tabla 6.9 | 0,1066403269 USD para la cuenta durante el 28 y 29 de mayo. No se atribuye todo a la turbina ni se valida el costo mensual. Se aclara la diferencia de relojes y el cierre de S3 en UTC el 30 de mayo. |
| Y4 | Yurley: 262 B frente a 1 KB | Atendido | 3.7.5; 6.2 y 6.3 | 262 B como objeto JSON almacenado representativo; media de 261,58 B en 24 h. El buffer de 512 B es capacidad reservada, no tamaño medido. Se corrige el cálculo a 56,6 MB decimales/mes en el escenario nominal. |
| Y5 | Yurley: comparación con antecedente de 2024 | Atendido documentalmente | 2.4; tabla 2.2 | Contraste de costos, latencia, confiabilidad, mantenimiento y crecimiento. Se indican diferencias implementadas y mediciones comparativas inexistentes; no se inventa un ensayo entre arquitecturas. |
| Y6 | Yurley: estado del arte y brecha | Atendido | Introducción; 2.4; tabla 2.3; 3.8.2 | Se añade alternativa publicada CR1000/Raspberry Pi/Azure/Power BI y se delimita el aporte de integración. Se reconoce documentación MQTT/TLS existente; la dificultad de tres meses corresponde a la experiencia del proyecto. |
| Y7 | Yurley: escalabilidad | Atendido | Resumen, 6.4, 6.5 y conclusiones | La extensión a múltiples turbinas queda presentada como proyección pendiente de ensayo, no como resultado experimental. |
| Y8 | Yurley: bibliografía y correspondencia | Atendido | bibliografia.bib; citas del capítulo 2 y capítulo 6 | Se consolidan entradas repetidas y alias TCP/ESP-MQTT, se unifica la entidad Amazon Web Services y se sustituyen letras de año manuales por referencias automáticas. Se normalizan DOI/URL y se corrigen autorías verificadas. |
| A1 | Arley: React/Vibe y hgora | Atendido | Introducción; capítulo 6 | React/Vite y hora, además de erratas puntuales identificadas. No se renombraron carpetas ni identificadores del firmware. |
| A2 | Arley: párrafo repetido de 3.4 | Atendido | 3.4 | Se conserva una explicación de modularidad. También se diferencia almacenamiento microSD de un reenvío automático no implementado. |
| A3 | Arley: nomenclatura del generador | Atendido | Figuras 2.1 y 5.21 | Se corrigen pies y rótulos internos a Savonius de eje vertical. En la captura se declara la corrección editorial; píxeles fuera del área del rótulo idénticos al original. Se retira el número interno Figura 1 de la ilustración para evitar doble numeración. |
| A4 | Arley: fuentes originales cloud/frontend | PENDIENTE DE ARCHIVOS ORIGINALES | Anexo D; tabla D.1; docs/ALCANCE_Y_TRAZABILIDAD.md | No están GetLatest, GetTimeseries, ActualizarUltimoDatoDynamo, React/Vite ni la configuración desplegable en el ZIP recibido. Se detalla lo requerido y se limita la afirmación de reproducción total. No se fabricó código equivalente. |
| D1 | Director 1: ilustración de cuatro capas | Ya incorporado; conservado | 2.2; figura 2.2 | Se conserva la figura aprobada y su explicación. |
| D2 | Director 2: modelo OSI | Ya incorporado; conservado | 2.3; figura 2.3 | Se conserva la matriz OSI–TCP/IP aprobada y la tabla. |
| D3 | Director 3: atribución OASIS | Verificado y normalizado | 2.4 | La definición de MQTT se atribuye a OASIS mediante cita automática. |
| D4 | Director 4: ilustración comparativa | Conservado y reforzado | 2.4; figura 2.4 y tablas 2.2–2.3 | Se mantiene la ilustración y se agregan límites de la comparación. |
| D5 | Director 5: sistemas de potencia | Ya incorporado; conservado | 2.7 y trabajo futuro del capítulo 7 | Se conserva como proyección conceptual; no se presenta como control de estabilidad implementado. |
| D6 | Director 6: timestamp en cursiva | Ya incorporado; conservado | 3.1 | Se mantiene la cursiva en la explicación solicitada; los nombres de campo del contrato se conservan como código. |
| D7 | Director 7: “Sección 3.2” | PENDIENTE DE ENUNCIADO | No determinado | La observación original solo contiene ese título. No es posible definir una corrección sin el comentario completo. |
| D8 | Director 8: orientación del capítulo 3 | Ya incorporado; conservado | Inicio del capítulo 3 | Se mantiene la guía de lectura por bloques del capítulo. |
| D9 | Director 9: síntesis del capítulo 3 | Ya incorporado; conservado | 3.14 | Se conservan los párrafos de síntesis y transición. |
| P1 | Revisión anterior: secuencia prueba/evidencia/síntesis | Verificado | Capítulo 5 | Se conservan posiciones H y barreras entre componentes. Las capturas no cruzan al siguiente componente. |
| P2 | Revisión anterior: figura comparativa antes de 2.5 | Verificado | 2.4 | La figura y las tablas comparativas preceden al inicio de 2.5. |
| P3 | Revisión anterior: legibilidad de nuevas figuras | Conservado | Figuras de cuatro capas, OSI, comparación y AWS | Se mantienen las versiones rediseñadas que venían aprobadas y los diagramas UML existentes. |
| P4 | Revisión anterior: pie CloudFront | Verificado | Figura 5.20 | Se conserva pie y número; se evita atribuir a la captura una medición de baja latencia. |
| P5 | Revisión anterior: ruta S3 y tabla de escalamiento | Verificado | Capítulo 4; tabla 6.10 | Se conservan el ajuste de ruta y los anchos de tabla. El tópico permanece debajo. La tabla se renumera automáticamente al añadir evidencia. |
| P6 | Numeración de figuras y atribución IA | Verificado | Todo el PDF; figuras aprobadas del director | No aparece Figura nro. ni referencias sin resolver. Se mantienen las atribuciones GPT-6 Astra Media de las cuatro figuras correspondientes y la procedencia declarada de las ilustraciones anteriores. |
| P7 | Anexos con repositorio y contacto | Ya incorporado; conservado | Anexos A–D | Se mantienen enlaces al repositorio privado, descripciones de ESP1/ESP2/ESP3 y contacto andres25@utp.edu.co. Se explicita el límite del Anexo D. |
| P8 | Diagramas UML y párrafos | Ya incorporado; conservado | Capítulos 3 y 4 | Fuentes, figuras y explicación UML conservados sin rediseño. |
| P9 | Brecha en resumen, introducción y experiencia ESP–AWS | Atendido y delimitado | Resumen, Abstract, Introducción y 3.8.2 | Se conserva la experiencia MicroPython → C/ESP-IDF y el apoyo de DeepSeek. No se atribuye el resultado a supuesta documentación china ni se generaliza ausencia mundial de documentación. |
| P10 | Comillas en concepto NIST | Ya incorporado; cita normalizada | Primer párrafo del capítulo 2 | Se conserva «redes de cosas» entre comillas y la atribución al NIST. El resto sigue siendo paráfrasis. |

## Comprobaciones realizadas

- PDF generado correctamente: 152 páginas, Quarto 1.8.27 y pdfLaTeX/TeX Live 2026. Resumen y Abstract caben en una página cada uno.
- Revisión visual de tablas comparativas, evidencia de S3, costos, rótulos corregidos, anexos y páginas de pruebas. Figuras y tablas con referencias automáticas.
- Cálculo reproducido con R base 4.5.0: 7.546 objetos/24 h; 9.860 objetos en la captura; 261,58 B de tamaño medio en 24 h; 0,1066403269 USD de cuenta en dos días.
- No se encontraron `??`, `@fig-`, `@tbl-`, `Figura nro.`, `React/Vibe` ni `hgora` en el texto del PDF.
- Se conservan todos los archivos originales del ZIP. Los encabezados de los capítulos activos y anexos son idénticos. Los únicos cambios de configuración de renderizado son el estilo bibliográfico local necesario para desambiguar referencias más allá de la letra z.

## Bibliografía

La base contenía 143 entradas con 109 claves distintas. Tras consolidar repeticiones y dos alias equivalentes quedan 107 entradas con clave única. Los alias retirados se sustituyeron en los capítulos activos. Se revisaron las correspondencias de citas y fuentes y se corrigieron, entre otras, la autoría de Akyüz/Demircan según el PDF publicado y la identificación de Emílio Roberto Reginaldo Tubino en la referencia de MQTT.

Al unificar el autor institucional AWS, quedaron más de 26 referencias del mismo autor/año. El estilo `plainnat` original producía un carácter inválido después de z e impedía compilar. `tesis/tex/plainnat-extendido.bst` conserva su presentación y extiende la desambiguación a aa, ab, etc.; `_quarto.yml` apunta a ese archivo. No se asignan letras manualmente.

La normalización documental no equivale a verificar cada afirmación de cada fuente ni a comprobar la vigencia tarifaria actual. Los valores mensuales se conservan como escenario histórico de referencia, no como cotización de noviembre de 2026. Los metadatos consultados para resolver discrepancias se registran en `revision_bibliografica.json`.

## Límites de la evidencia

El inventario contiene claves S3, tamaños y última modificación; no contiene todos los valores sensados ni un contador de mensajes emitidos. La regularidad de registros almacenados no demuestra ausencia de pérdidas. Los nombres de archivo no explicitan su zona horaria, por lo que no se calcula latencia con `LastModified`. El reporte de costos agrega toda la cuenta y no identifica costos exclusivos de la turbina ni consultas individuales de Athena. El plan de consultas se conserva expresamente como protocolo, sin presentarlo como bitácora recuperada.

## Archivos de control

- `inventario_saneamiento.csv`: huellas de los archivos entregados que deben coincidir antes de nuevas ediciones.
- `comprobacion_preservacion.json`: comparación con el ZIP original y comprobación de figuras.
- `cambios_documentales.diff` y `antes/`: diferencias y copia previa de los archivos intervenidos.
- `telemetria_resumen.csv`, `costos_resumen.csv` y `ejecucion_calculo_R.txt`: resultados reproducidos.
- `compilacion_Quarto.txt` y `ruta_revision_pdf.csv`: generación y guía de lectura del PDF.
- `docs/ACTUALIZACION_20261118.md`: pasos para abrir, renderizar, revisar y versionar.
