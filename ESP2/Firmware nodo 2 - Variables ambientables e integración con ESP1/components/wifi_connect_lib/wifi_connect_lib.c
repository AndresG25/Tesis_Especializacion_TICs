#include "wifi_connect_lib.h"

static const char *TAG = "WiFiDriver";
static int retry_num_connect = 0;

// Elimina el EventGroup (no es necesario si no bloqueas la ejecución)
static void esp_event_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_num_connect < WIFI_MAXIMUM_RETRY) {
                    esp_wifi_connect();
                    retry_num_connect++;
                    ESP_LOGW(TAG, "Reintento %d...", retry_num_connect);
                } else {
                    ESP_LOGE(TAG, "Conexión fallida después de %d intentos", WIFI_MAXIMUM_RETRY);
                }
                break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGI(TAG, "Conectado a %s con IP: " IPSTR, WIFI_MY_SSID, IP2STR(&event->ip_info.ip));
        retry_num_connect = 0;
    }
}

bool WiFiStationConnect(void) {
    // Configuración básica de WiFi (sin inicializar NVS/netif/event loop)
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    // Registra solo los manejadores de eventos necesarios
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &esp_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &esp_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_MY_SSID,
            .password = WIFI_MY_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi iniciado en modo STA. Conectando...");
    return true; // Retorna inmediatamente sin bloquear
}

