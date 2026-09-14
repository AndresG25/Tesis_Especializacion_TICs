#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "minmea.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

// Configuración de hardware
#define UART_GPS        UART_NUM_1
#define UART_RX_PIN     20
#define UART_TX_PIN     21
#define BUF_SIZE        1024
#define VOLTAGE_ADC     ADC_CHANNEL_0
#define MAC_ESP2        {0x40, 0x4C, 0xCA, 0x42, 0x13, 0x00}
#define SENSOR_GPIO     GPIO_NUM_22
#define LED_GPIO        GPIO_NUM_2
#define INTERVALO_SEGUNDOS 1
#define PULSOS_POR_REVOLUCION 2  // Ajustar según tu sensor
#define ESCALA_RPM      (60.0 / (INTERVALO_SEGUNDOS * PULSOS_POR_REVOLUCION))

// Estructura de datos compartida
typedef struct {
    float latitud;
    float longitud;
    float voltaje_ac;   // Voltaje AC (valor RMS)
    float rpm;          // RPM del anemómetro
    bool gps_fix;
} mensaje_t;

static const char *TAG = "ESP1_Transmisor";
uint8_t esp2_mac[6] = MAC_ESP2;

// Variables para ADC
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle = NULL;

// Variables para sensor de efecto Hall
volatile uint32_t contador_pulsos = 0;
volatile uint32_t ultimo_tiempo_pulso = 0;
volatile bool flag_rpm_ready = false;
gptimer_handle_t gptimer = NULL;

// Función de conversión de coordenadas
static void convertir_a_decimal(struct minmea_float *lat_raw, struct minmea_float *lon_raw, 
                              float *lat, float *lon, char lat_dir, char lon_dir) {
    float raw_lat = minmea_tofloat(lat_raw);
    float raw_lon = minmea_tofloat(lon_raw);
    
    *lat = (int)(raw_lat / 100) + fmod(raw_lat, 100) / 60.0f;
    *lon = (int)(raw_lon / 100) + fmod(raw_lon, 100) / 60.0f;
    
    if (lat_dir == 'S') *lat *= -1;
    if (lon_dir == 'W') *lon *= -1;
}

// Inicialización del ADC (nueva API)
void init_adc() {
    // Configuración del ADC
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    // Configuración del canal
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, VOLTAGE_ADC, &config));

    // Calibración usando curve_fitting
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    if (adc_cali_create_scheme_curve_fitting(&cali_config, &adc1_cali_handle) != ESP_OK) {
        ESP_LOGE(TAG, "No se pudo crear esquema de calibración");
    }
}

// Tarea GPS
void gps_task(void *pvParameter) {
    mensaje_t *mensaje = (mensaje_t*)pvParameter;
    
    vTaskDelay(pdMS_TO_TICKS(3000));
    
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_GPS, BUF_SIZE*2, 0, 0, NULL, 0);
    uart_param_config(UART_GPS, &uart_config);
    uart_set_pin(UART_GPS, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    char line[MINMEA_MAX_SENTENCE_LENGTH];
    char buffer[BUF_SIZE];
    int pos = 0;

    while(1) {
        int len = uart_read_bytes(UART_GPS, (uint8_t*)buffer, BUF_SIZE, 100 / portTICK_PERIOD_MS);
        if(len > 0) {
            for(int i=0; i<len; i++) {
                if(buffer[i] == '\n') {
                    line[pos] = '\0';
                    if(minmea_sentence_id(line, false) == MINMEA_SENTENCE_RMC) {
                        struct minmea_sentence_rmc frame;
                        if(minmea_parse_rmc(&frame, line)) {
                            if(frame.valid) {
                                float lat, lon;
                                convertir_a_decimal(&frame.latitude, &frame.longitude, &lat, &lon, frame.lat_dir, frame.lon_dir);
                                mensaje->latitud = lat;
                                mensaje->longitud = lon;
                                mensaje->gps_fix = true;
                                ESP_LOGI(TAG, "Fix OK! Lat=%.6f Lon=%.6f", lat, lon);
                            } else {
                                mensaje->gps_fix = false;
                            }
                        }
                    }
                    pos = 0;
                } else if(pos < sizeof(line)-1) {
                    line[pos++] = buffer[i];
                }
            }
        }
        vTaskDelay(1);
    }
}

// Función para aplicar filtro de media móvil
float aplicar_filtro(float nuevo_valor, float *historial, int *indice, int tamano) {
    historial[*indice] = nuevo_valor;
    *indice = (*indice + 1) % tamano;
    
    float suma = 0;
    for(int i = 0; i < tamano; i++) {
        suma += historial[i];
    }
    
    return suma / tamano;
}

// Tarea de lectura de voltaje AC con offset
void voltage_task(void *pvParameter) {
    mensaje_t *mensaje = (mensaje_t*)pvParameter;
    const int num_muestras = 200; // Aumentamos muestras para mejor precisión
    const float offset = 1.65f;   // Offset aplicado por el circuito
    const float voltage_divider_ratio = 2.0f; // Ajustar según divisor de voltaje
    
    // Variables para filtrado
    static float voltaje_historial[5] = {0};
    static int hist_indice = 0;
    
    init_adc();
    
    while(1) {
        int raw;
        float sum_cuadrados = 0;
        int muestras_validas = 0;
        
        // Tomar múltiples muestras para cálculo RMS
        for(int i = 0; i < num_muestras; i++) {
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, VOLTAGE_ADC, &raw));
            
            // Convertir a voltaje (eliminando el offset)
            float voltaje_inst;
            if (adc1_cali_handle) {
                int voltage_mv;
                ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, raw, &voltage_mv));
                voltaje_inst = (voltage_mv / 1000.0f) - offset;
            } else {
                voltaje_inst = ((raw * 3.3f / 4095.0f) * voltage_divider_ratio) - offset;
            }
            
            // Solo considerar valores positivos (hemos eliminado el semiciclo negativo)
            if(voltaje_inst > 0) {
                sum_cuadrados += voltaje_inst * voltaje_inst;
                muestras_validas++;
            }
            
            vTaskDelay(pdMS_TO_TICKS(1)); // Pequeño delay entre muestras
        }
        
        // Calcular voltaje RMS solo si tenemos suficientes muestras válidas
        if(muestras_validas > num_muestras/2) {
            float voltaje_rms = sqrt(sum_cuadrados / muestras_validas);
            // Aplicar filtro de media móvil
            mensaje->voltaje_ac = aplicar_filtro(voltaje_rms, voltaje_historial, &hist_indice, 5);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay principal
    }
}

// ISR del sensor de efecto Hall con debounce
void IRAM_ATTR sensor_isr_handler(void *arg) {
    static int64_t ultimo_tiempo = 0;
    int64_t ahora = esp_timer_get_time(); // Obtiene tiempo en microsegundos
    
    // Debounce de 10ms (10,000 µs)
    if((ahora - ultimo_tiempo) > 10000) {
        contador_pulsos++;
        ultimo_tiempo = ahora;
        
        // Opcional: toogle rápido de LED para depuración visual
        gpio_set_level(LED_GPIO, 1);
        gpio_set_level(LED_GPIO, 0);
    }
}

// Callback del timer para cálculo de RPM
bool IRAM_ATTR timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    mensaje_t *mensaje = (mensaje_t*)user_ctx;
    static float rpm_historial[3] = {0};
    static int rpm_hist_indice = 0;
    
    float rpm_calculada = contador_pulsos * ESCALA_RPM;
    
    // Filtrado básico para eliminar valores absurdos
    if(rpm_calculada < 500) { // Asumimos que las RPM no superarán 500
        // Aplicar filtro de media móvil
        mensaje->rpm = aplicar_filtro(rpm_calculada, rpm_historial, &rpm_hist_indice, 3);
    }
    
    contador_pulsos = 0;
    flag_rpm_ready = true;
    return true;
}

// Inicialización del sensor de efecto Hall y timer
void init_hall_sensor(mensaje_t *mensaje) {
    // Configuración GPIO
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SENSOR_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,  // Habilitar pull-up para mejor detección
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    
    // Configuración LED (se inicializa pero se controla en espnow_task)
    ESP_ERROR_CHECK(gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT));
    gpio_set_level(LED_GPIO, 0);  // Inicialmente apagado
    
    // Instalar ISR
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(SENSOR_GPIO, sensor_isr_handler, NULL));

    // Configuración del timer
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1 µs por tick
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_event_callbacks_t callbacks = {
        .on_alarm = timer_callback,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &callbacks, mensaje));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = INTERVALO_SEGUNDOS * 1000000, // Conversión a µs
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

// Inicialización ESP-NOW
void inicializar_espnow() {
    ESP_ERROR_CHECK(esp_now_init());
    
    esp_now_peer_info_t peer = {
        .channel = 1,
        .encrypt = false
    };
    memcpy(peer.peer_addr, esp2_mac, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

// Tarea de envío periódico con control del LED
void espnow_task(void *pvParameter) {
    mensaje_t *mensaje = (mensaje_t*)pvParameter;
    
    while(1) {
        esp_err_t result = esp_now_send(esp2_mac, (uint8_t*)mensaje, sizeof(mensaje_t));
        
        // Encender LED al enviar
        gpio_set_level(LED_GPIO, 1);
        
        if(result == ESP_OK) {
            ESP_LOGI(TAG, "Datos enviados: %.2fV AC | %.1f RPM | %s | Lat=%.6f Lon=%.6f",
                    mensaje->voltaje_ac,
                    mensaje->rpm,
                    mensaje->gps_fix ? "GPS OK" : "Sin Fix",
                    mensaje->latitud, mensaje->longitud);
        } else {
            ESP_LOGE(TAG, "Error al enviar: %s", esp_err_to_name(result));
        }
        
        // Pequeña pausa con LED encendido (200ms)
        vTaskDelay(pdMS_TO_TICKS(200));
        
        // Apagar LED
        gpio_set_level(LED_GPIO, 0);
        
        // Esperar hasta el próximo envío
        vTaskDelay(pdMS_TO_TICKS(1800)); // Total 2000ms (2 segundos)
    }
}

void app_main() {
    // Inicialización básica
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    // Configurar canal WiFi fijo
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    // Inicializar ESP-NOW
    inicializar_espnow();

    // Estructura compartida
    static mensaje_t mensaje = {0};

    // Inicializar sensor de efecto Hall
    init_hall_sensor(&mensaje);

    // Crear tareas con prioridades adecuadas
    xTaskCreate(gps_task, "gps_task", 4096, &mensaje, 6, NULL);
    xTaskCreate(voltage_task, "voltage_task", 4096, &mensaje, 5, NULL);
    xTaskCreate(espnow_task, "espnow_task", 4096, &mensaje, 3, NULL);

    ESP_LOGI(TAG, "Sistema iniciado. Enviando datos a ESP2...");
}