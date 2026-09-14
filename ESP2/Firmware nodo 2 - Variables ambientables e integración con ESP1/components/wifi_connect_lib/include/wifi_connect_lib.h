
//Librerias ANSI C
#include <stdint.h>

//Librerías ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"


//Credenciales de red
#if __has_include("config_local.h")
#include "config_local.h"
#else
#define WIFI_MY_SSID ""
#define WIFI_MY_PASS ""
#endif

//Número máximo de reintentos en la conexión al AP
#define WIFI_MAXIMUM_RETRY  5

//Bits para una conexión Wi-Fi exitosa y fallida con un AP
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

//Prototipos de funciones
bool WiFiStationConnect (void);


