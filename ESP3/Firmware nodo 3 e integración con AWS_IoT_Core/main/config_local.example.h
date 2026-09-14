#pragma once

// Copiar como config_local.h y completar SOLO en el equipo de trabajo.
// El archivo config_local.h queda excluido de Git.
#define WIFI_SSID "REEMPLAZAR_SSID"
#define WIFI_PASS "REEMPLAZAR_CONTRASENA"

// Nombre de host del endpoint de datos AWS IoT; sin mqtts:// ni rutas.
#define AWS_IOT_ENDPOINT "REEMPLAZAR_ENDPOINT_ATS.iot.REEMPLAZAR_REGION.amazonaws.com"
// Deben coincidir con la política y las reglas del despliegue utilizado.
#define AWS_IOT_TOPIC "REEMPLAZAR_TOPICO"
#define AWS_IOT_CLIENT_ID "REEMPLAZAR_CLIENT_ID"
