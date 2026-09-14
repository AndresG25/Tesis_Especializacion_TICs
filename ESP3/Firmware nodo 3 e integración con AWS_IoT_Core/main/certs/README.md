# Certificados del nodo ESP3

Se conserva `AmazonRootCA1.pem`, que es un certificado público de autoridad raíz.

Antes de configurar y compilar ESP3, coloque en esta misma carpeta:

| Nombre exacto | Contenido esperado |
| --- | --- |
| `device_cert.crt` | Certificado X.509 del dispositivo en formato PEM. |
| `private_key.key` | Llave privada correspondiente, en formato PEM y compatible con ESP-TLS. |

Los dos archivos deben proceder de su configuración autorizada de AWS IoT. No se proporcionan archivos ficticios con estos nombres. `main/CMakeLists.txt` los incorpora al firmware con `EMBED_FILES`; los nombres y los símbolos usados por el código se conservan.

Estos archivos privados y los binarios de compilación quedan fuera de Git. Añadir una regla a `.gitignore` no elimina un secreto que ya se haya incorporado al historial: no suba los ZIP originales ni fuerce su inclusión con `git add -f`.
