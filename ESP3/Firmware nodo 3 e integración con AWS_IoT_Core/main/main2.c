//// Este código busca recibir los datos del proceso por UART y enviarlos a AWS IoT Core

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "time.h"

// Configuración UART
#define UART_PORT_NUM      UART_NUM_1
#define UART_BAUD_RATE     9600
#define UART_TX_PIN        15
#define UART_RX_PIN        22
#define UART_BUF_SIZE      512
#define UART_READ_TOUT     10 // ticks

// Configuración local: copiar config_local.example.h a config_local.h.
#include "config_local.h"
#define MAX_WIFI_RETRY    20
#define AWS_IOT_PORT      8883

// Certificados embebidos
extern const uint8_t aws_root_ca_pem_start[] asm("_binary_AmazonRootCA1_pem_start");
extern const uint8_t aws_root_ca_pem_end[]   asm("_binary_AmazonRootCA1_pem_end");
extern const uint8_t device_cert_start[]     asm("_binary_device_cert_crt_start");
extern const uint8_t device_cert_end[]       asm("_binary_device_cert_crt_end");
extern const uint8_t private_key_start[]     asm("_binary_private_key_key_start");
extern const uint8_t private_key_end[]       asm("_binary_private_key_key_end");

static const char *TAG = "AWS_IOT_BRIDGE";

// Variables globales
static esp_mqtt_client_handle_t mqtt_client = NULL;
static bool mqtt_connected = false;
static SemaphoreHandle_t mqtt_mutex = NULL;
static bool wifi_connected = false;

// =============================================
// [Funciones WiFi - Sin cambios]
// =============================================

void wifi_event_handler(void* arg, esp_event_base_t event_base, 
                        int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            wifi_connected = false;
            ESP_LOGW(TAG, "WiFi desconectado. Reconectando...");
            esp_wifi_connect();
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        wifi_connected = true;
        ESP_LOGI(TAG, "WiFi conectado! IP: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

void wifi_init_sta(void) {
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Registrar manejadores de eventos
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Conectando a WiFi...");
}

// =============================================
// [Funciones MQTT - Adaptadas para AWS IoT]
// =============================================

void mqtt_event_handler(void *handler_args, esp_event_base_t base, 
                        int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    
    if (xSemaphoreTake(mqtt_mutex, portMAX_DELAY)) {
        switch (event->event_id) {
            case MQTT_EVENT_BEFORE_CONNECT:
                ESP_LOGI(TAG, "Iniciando conexión MQTT a AWS IoT...");
                break;
                
            case MQTT_EVENT_CONNECTED:
                mqtt_connected = true;
                ESP_LOGI(TAG, "✅ MQTT conectado a AWS IoT Core!");
                ESP_LOGI(TAG, "Endpoint: %s", AWS_IOT_ENDPOINT);
                ESP_LOGI(TAG, "Cliente ID: %s", AWS_IOT_CLIENT_ID);
                ESP_LOGI(TAG, "Tópico: %s", AWS_IOT_TOPIC);
                break;
                
            case MQTT_EVENT_DISCONNECTED:
                mqtt_connected = false;
                ESP_LOGW(TAG, "⚠️ MQTT desconectado. Intentando reconectar...");
                vTaskDelay(pdMS_TO_TICKS(5000));
                esp_mqtt_client_reconnect(mqtt_client);
                break;
                
            case MQTT_EVENT_SUBSCRIBED:
                ESP_LOGI(TAG, "Suscrito al tópico (ID: %d)", event->msg_id);
                break;
                
            case MQTT_EVENT_UNSUBSCRIBED:
                ESP_LOGI(TAG, "Desuscrito del tópico (ID: %d)", event->msg_id);
                break;
                
            case MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG, "📤 Publicación confirmada (ID: %d)", event->msg_id);
                break;

            case MQTT_EVENT_DATA:
                ESP_LOGI(TAG, "📥 Mensaje recibido:");
                ESP_LOGI(TAG, "Tópico: %.*s", event->topic_len, event->topic);
                ESP_LOGI(TAG, "Datos: %.*s", event->data_len, event->data);
                break;
                
            case MQTT_EVENT_ERROR: {
                ESP_LOGE(TAG, "❌ Error MQTT");
                esp_mqtt_error_codes_t *error = event->error_handle;
                
                if (error->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                    ESP_LOGE(TAG, "Error de transporte: %s", esp_err_to_name(error->esp_tls_last_esp_err));
                    ESP_LOGE(TAG, "TLS stack error: 0x%x", error->esp_tls_stack_err);
                    
                    if (error->esp_tls_last_esp_err == ESP_ERR_MBEDTLS_SSL_HANDSHAKE_FAILED) {
                        ESP_LOGE(TAG, "Error en handshake TLS - Verifica tus certificados!");
                    }
                }
                break;
            }
                
            default:
                ESP_LOGD(TAG, "Evento MQTT no manejado: %d", event->event_id);
                break;
        }
        xSemaphoreGive(mqtt_mutex);
    }
}

void mqtt_app_start(void) {
    ESP_LOGI(TAG, "Iniciando MQTT con AWS IoT Core...");

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .hostname = AWS_IOT_ENDPOINT,
                .port = AWS_IOT_PORT,
                .transport = MQTT_TRANSPORT_OVER_SSL,
            },
            .verification = {
                .crt_bundle_attach = esp_crt_bundle_attach,
            },
        },
        .credentials = {
            .client_id = AWS_IOT_CLIENT_ID,
            .authentication = {
                .certificate = (const char *)device_cert_start,
                .key = (const char *)private_key_start,
            },
        },
        .buffer = {
            .size = 2048,
        },
        .network = {
            .reconnect_timeout_ms = 10000,
        },
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

// =============================================
// [Funciones UART - Sin cambios]
// =============================================

void uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, UART_BUF_SIZE * 2, UART_BUF_SIZE * 2, 10, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_set_rx_timeout(UART_PORT_NUM, UART_READ_TOUT));
}

bool verify_and_format_json(const char *json_str, char *output, size_t output_size) {
    // Verificar que sea un JSON válido (contenga { y })
    if (strchr(json_str, '{') == NULL || strchr(json_str, '}') == NULL) {
        ESP_LOGE(TAG, "JSON inválido: no se encontraron llaves");
        return false;
    }

    // Extraer solo el contenido entre llaves
    const char *start = strchr(json_str, '{');
    const char *end = strrchr(json_str, '}');
    
    if (start == NULL || end == NULL || end <= start) {
        ESP_LOGE(TAG, "JSON inválido: estructura de llaves incorrecta");
        return false;
    }

    size_t json_len = end - start + 1;
    if (json_len >= output_size) {
        ESP_LOGE(TAG, "JSON demasiado grande para el buffer");
        return false;
    }

    strncpy(output, start, json_len);
    output[json_len] = '\0';
    return true;
}

void pretty_print_json(const char *json_str) {
    ESP_LOGI(TAG, "════════════════ DATOS RECIBIDOS ════════════════");
    ESP_LOGI(TAG, "Payload: %s", json_str);
    ESP_LOGI(TAG, "═════════════════════════════════════════════════");
}

void uart_receive_task(void *arg) {
    uint8_t *rx_buffer = malloc(UART_BUF_SIZE);
    char json_buffer[UART_BUF_SIZE];
    size_t json_partial_len = 0;
    bool json_in_progress = false;
    
    if (!rx_buffer) {
        ESP_LOGE(TAG, "Error al asignar buffer UART!");
        vTaskDelete(NULL);
    }

    while (1) {
        int len = uart_read_bytes(UART_PORT_NUM, rx_buffer, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        
        if (len > 0) {
            rx_buffer[len] = '\0';
            ESP_LOGD(TAG, "Datos UART crudos (%d bytes): %.*s", len, len, rx_buffer);
            
            // Procesar buffer para detectar JSONs completos o parciales
            for (int i = 0; i < len; i++) {
                if (rx_buffer[i] == '{') {
                    json_in_progress = true;
                    json_partial_len = 0;
                    json_buffer[json_partial_len++] = rx_buffer[i];
                } 
                else if (json_in_progress) {
                    if (json_partial_len < sizeof(json_buffer) - 1) {
                        json_buffer[json_partial_len++] = rx_buffer[i];
                    }
                    
                    if (rx_buffer[i] == '}') {
                        json_buffer[json_partial_len] = '\0';
                        
                        // Verificar si es un JSON válido
                        if (verify_and_format_json(json_buffer, json_buffer, sizeof(json_buffer))) {
                            pretty_print_json(json_buffer);
                            
                            if (xSemaphoreTake(mqtt_mutex, pdMS_TO_TICKS(1000))) {
                                if (mqtt_connected) {
                                    ESP_LOGI(TAG, "Publicando en AWS IoT...");
                                    int msg_id = esp_mqtt_client_publish(
                                        mqtt_client, 
                                        AWS_IOT_TOPIC, 
                                        json_buffer, 
                                        strlen(json_buffer),
                                        1,  // QoS 1
                                        0   // No retener
                                    );
                                    
                                    if (msg_id == -1) {
                                        ESP_LOGE(TAG, "❌ Fallo al publicar en AWS IoT!");
                                    } else {
                                        ESP_LOGI(TAG, "🚀 Enviado a AWS IoT (ID: %d)", msg_id);
                                    }
                                } else {
                                    ESP_LOGW(TAG, "MQTT no conectado, descartando datos");
                                }
                                xSemaphoreGive(mqtt_mutex);
                            }
                        }
                        json_in_progress = false;
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(rx_buffer);
}

// =============================================
// [Sincronización de tiempo]
// =============================================

void sync_time() {
    // Configuración manual de la hora (timestamp Unix actual)
    struct timeval tv = {
        .tv_sec = time(NULL),  // Usar hora actual si es posible
        .tv_usec = 0
    };
    
    // Añadir zona horaria si es necesario
    setenv("TZ", "UTC-5", 1);  // Ajusta según tu zona horaria
    tzset();
    
    if (settimeofday(&tv, NULL) == 0) {
        // Mostrar hora actual
        time_t now = time(NULL);
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);
        ESP_LOGI(TAG, "Hora del sistema: %02d:%02d:%02d", 
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    } else {
        ESP_LOGE(TAG, "Error al configurar hora del sistema");
    }
}

// =============================================
// [Función principal]
// =============================================

void app_main() {
    // Inicialización básica
    ESP_LOGI(TAG, "=== Inicializando sistema ===");
    ESP_ERROR_CHECK(nvs_flash_init());
    
    // Inicializar interfaz de red y event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Configurar niveles de log
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("AWS_IOT_BRIDGE", ESP_LOG_VERBOSE);
    esp_log_level_set("wifi", ESP_LOG_WARN);
    esp_log_level_set("esp_netif_handlers", ESP_LOG_WARN);
    
    ESP_LOGI(TAG, "Memoria libre al inicio: %u bytes", (unsigned int)esp_get_free_heap_size());

    // Inicializar WiFi
    wifi_init_sta();
    
    // Esperar conexión WiFi con timeout
    int retry = 0;
    while (!wifi_connected && retry < MAX_WIFI_RETRY) {
        ESP_LOGI(TAG, "Esperando conexión WiFi... (%d/%d)", retry + 1, MAX_WIFI_RETRY);
        vTaskDelay(pdMS_TO_TICKS(500));
        retry++;
    }
    
    if (!wifi_connected) {
        ESP_LOGE(TAG, "Fallo en conexión WiFi después de %d intentos", retry);
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
    }

    // Sincronizar hora (opcional para AWS IoT)
    sync_time();

    // Inicializar UART
    uart_init();
    ESP_LOGI(TAG, "UART inicializado en GPIO%d(RX), GPIO%d(TX)", UART_RX_PIN, UART_TX_PIN);

    // Verificación de certificados antes de iniciar MQTT
    ESP_LOGI(TAG, "Verificando certificados...");
    ESP_LOGI(TAG, "Tamaño Root CA: %d bytes", (int)(aws_root_ca_pem_end - aws_root_ca_pem_start));
    ESP_LOGI(TAG, "Tamaño Device Cert: %d bytes", (int)(device_cert_end - device_cert_start));
    ESP_LOGI(TAG, "Tamaño Private Key: %d bytes", (int)(private_key_end - private_key_start));

    // Inicializar MQTT
    mqtt_mutex = xSemaphoreCreateMutex();
    if (mqtt_mutex == NULL) {
        ESP_LOGE(TAG, "Error creando mutex MQTT");
        esp_restart();
    }
    
    mqtt_app_start();
    ESP_LOGI(TAG, "MQTT para AWS IoT iniciado");

    // Crear tarea UART
    if (xTaskCreate(uart_receive_task, "uart_rx_task", 4096, NULL, 5, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Error creando tarea UART");
        esp_restart();
    }

    ESP_LOGI(TAG, "=== Sistema listo ===");
    ESP_LOGI(TAG, "Esperando datos UART para enviar a AWS IoT...");
}