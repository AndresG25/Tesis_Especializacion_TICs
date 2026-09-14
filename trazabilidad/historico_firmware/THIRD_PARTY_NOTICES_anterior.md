# Componentes de terceros

Los controladores y bibliotecas incluidos conservan sus cabeceras originales. Sus avisos no se sustituyen por los metadatos de autoría del proyecto de tesis.

| Componente | Atribución presente en los fuentes | Aviso conservado o incorporado |
| --- | --- | --- |
| `ESP1/components/minmea` | Kosma Moczek, 2014; [proyecto minmea](https://github.com/kosma/minmea). | La cabecera declara WTFPL versión 2. Se incorpora `COPYING` obtenido del proyecto original. |
| `ESP2/components/bme680` | Ruslan V. Uss, 2018; actualizaciones atribuidas a Ivan Vargas A., INTECX S.A.C. | La copia recibida declara MIT en sus cabeceras. Se añade el texto estándar MIT con la atribución de esa copia en `LICENSE`. |
| `ESP2/components/ssd1306` | [nopnop2002/esp-idf-ssd1306](https://github.com/nopnop2002/esp-idf-ssd1306); actualizaciones atribuidas a Ivan Vargas A., INTECX S.A.C. | Se incorpora el archivo MIT `LICENSE` del proyecto indicado en la cabecera. |
| `ESP2/components/wifi_connect_lib` | No incluye un aviso explícito de autoría o licencia en los archivos adjuntos. | Se conserva como parte de la entrega recibida; esta preparación no le asigna una nueva licencia. |
| ESP-IDF | Espressif y colaboradores. | Dependencia externa; no se redistribuye el SDK dentro del paquete. Consulte los avisos de la versión instalada. |

Las copias locales pueden contener adaptaciones respecto de sus proyectos de origen. No se declara que coincidan con una revisión concreta de los repositorios upstream. En particular, la licencia de BME680 se documenta según la cabecera recibida, sin trasladar automáticamente la licencia de otras versiones.

El certificado `ESP3/main/certs/AmazonRootCA1.pem` es material público de autoridad certificadora conservado de la entrega. No es una llave privada ni una identidad de dispositivo.
