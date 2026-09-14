






///// Código Funcional ESP2 final?


// ========================== LIBRERÍAS ==========================
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/uart.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "hal/i2c_types.h"
#include "soc/clk_tree_defs.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <sys/stat.h>
#include <dirent.h>

#include "bme680.h"
#include "ssd1306.h"

// ===================== PROTOTIPOS ==============================
void GPIOInit(void);
void GPTimerInit(void);
void inicializar_espnow_receptor(void);
void I2CMasterInit(void);
void OLEDInit(void);
bool BMEInit(void);
bool read_bme680_values(BME680_values_float_t *values);
void UARTInit(void);
void uart_receive_task(void *arg);
void DS3231Init(void);
void DS3231SetHora(uint8_t hrs, uint8_t min, uint8_t seg,
                   uint8_t formt, uint8_t Am_Pm);
void DS3231SetFecha(uint8_t diaSem, uint8_t diaMes,
                    uint8_t mes, uint8_t anio);
void DS3231ReadHora(uint8_t *pHrs, uint8_t *pMin,
                    uint8_t *pSeg, char *pAmPm);
void DS3231ReadFecha(uint8_t *pDiaSem, uint8_t *pDiaMes,
                     uint8_t *pMes, uint8_t *pAnio);

// ===================== TAG DE LOGGING =========================
static const char *TAG = "ESP32C6_INTEGRADO";
static const char *TAG_UART = "UART_JSON_RX";
static const char *TAG_BME = "BME680";
static const char *TAG_ESPNOW = "ESP-NOW";
static const char *TAG_SD = "SD_CARD";
static const char *TAG_UART_TX = "UART_TX";

// ===================== DEFINICIONES ============================
#define UART_PORT_RX_TX   UART_NUM_1
#define UART_TX_PIN       15
#define UART_RX_PIN       9
#define UART_BAUDRATE     9600
#define UART_BUF_SIZE     512

#define SENSOR_GPIO            GPIO_NUM_7
#define LED_GPIO               GPIO_NUM_2
#define INTERVALO_SEGUNDOS     1
#define ESCALA_VELOCIDAD       0.47f

#define I2C0_SDA               5
#define I2C0_SCL               6
#define I2C0_PORT              I2C_NUM_0

#define OLED_ADDR              0x3C
#define OLED_ANCHO             128
#define OLED_ALTO              64

#define BME_ADDR               0x77
#define SEA_LEVEL_PRESSURE_HPA 1202.5f

#define DS3231_ADDR            0x68
#define FORMAT_12              1
#define FORMAT_24              0
#define AM                     1
#define PM                     0

// Definiciones para SD Card
#define SD_MOSI_PIN            23
#define SD_MISO_PIN            22
#define SD_SCLK_PIN            21
#define SD_CS_PIN              3
#define SD_MOUNT_POINT         "/sd"
#define SD_MAX_FILES           5
#define SD_ALLOC_UNIT_SIZE     (16 * 1024)

// MAC de la ESP transmisora
#define ESP1_MAC {0x40, 0x4C, 0xCA, 0x4F, 0x33, 0x44}

// ===================== ESTRUCTURAS ==============================
typedef struct {
    float latitud;
    float longitud;
    float voltaje_ac;
    float rpm;
    bool gps_fix;
} mensaje_espnow_t;

typedef struct {
    uint8_t hora, min, seg;
    uint8_t diaSemana, diaMes, mes, anio;
} RTC_t;

typedef struct {
    FILE* csv_file;
    sdmmc_card_t* card;
    uint8_t sample_count;
    float sum_wind_speed;
    float sum_voltage;
    float sum_temperature;
    float sum_humidity;
    float sum_pressure;
    float sum_altitude;
    float sum_latitud;    
    float sum_longitud;
    float sum_rpm;
    uint8_t valid_gps_samples;
} sd_handler_t;

// ==================== VARIABLES GLOBALES =======================
volatile uint32_t        contador_pulsos        = 0;
volatile float           velocidad_viento       = 0.0f;
volatile float           voltaje_remoto         = -1.0f;
volatile float           rpm_recibida           = -1.0f;
float                    gps_latitud            = 0.0f;
float                    gps_longitud           = 0.0f;
int64_t                  ultima_recepcion_espnow= 0;

gptimer_handle_t         gptimer               = NULL;

i2c_master_bus_handle_t  I2C0BusHandle         = NULL;
SSD1306_t                OLEDDevice            = {0};
BME680_t                 BMEDevice             = {0};
i2c_master_dev_handle_t  DS3231DevHandle       = NULL;

uint32_t                 duracionMedicion      = 0;
BME680_values_float_t    bmeValues             = {0};
RTC_t                    rtcData               = {0};
sd_handler_t             sd_handler            = {0};

char strFormtAmPm[10];
char strTemp[20], strHumd[20], strPress[20], strAlt[20];
char strTime[20], strDate[20];

// Mutex para protección de recursos
SemaphoreHandle_t i2c_mutex = NULL;
SemaphoreHandle_t bme_mutex = NULL;
SemaphoreHandle_t espnow_mutex = NULL;
volatile bool bme_initialized = false;
volatile bool bme_measurement_in_progress = false;

// ================= FUNCIONES AUXILIARES ========================

void IRAM_ATTR sensor_isr_handler(void *arg)
{
    contador_pulsos++;
}

bool IRAM_ATTR timer_callback(gptimer_handle_t timer,
                              const gptimer_alarm_event_data_t *edata,
                              void *user_ctx)
{
    velocidad_viento = contador_pulsos * ESCALA_VELOCIDAD;
    contador_pulsos = 0;
    return true;
}

void on_receive_espnow(const esp_now_recv_info_t *info,
                       const uint8_t *data, int len)
{
    if (len == sizeof(mensaje_espnow_t)) {
        mensaje_espnow_t msg;
        memcpy(&msg, data, len);
        
        if (xSemaphoreTake(espnow_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            voltaje_remoto = msg.voltaje_ac + 0.2;
            rpm_recibida = msg.rpm;
            if(msg.gps_fix) {
                gps_latitud = msg.latitud;
                gps_longitud = msg.longitud;
            }
            ultima_recepcion_espnow = esp_timer_get_time();
            xSemaphoreGive(espnow_mutex);
        }
        
        ESP_LOGI(TAG_ESPNOW, "Datos recibidos: Lat=%.6f, Lon=%.6f, Volt=%.2fV, RPM=%.1f, Fix=%d",
                msg.latitud, msg.longitud, msg.voltaje_ac, msg.rpm, msg.gps_fix);
    } else {
        ESP_LOGE(TAG_ESPNOW, "Tamaño incorrecto: %d (esperaba %d)", 
                len, sizeof(mensaje_espnow_t));
    }
}

// ======================== TAREAS ===============================
void monitor_memoria_task(void *arg)
{
    const size_t HEAP_TOTAL = 320000;
    while (1) {
        size_t libre = esp_get_free_heap_size();
        float pct = 100.0f * (1.0f - (float)libre/HEAP_TOTAL);
        ESP_LOGI(TAG, "Heap libre: %u bytes (%.2f%% usado)", libre, pct);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// ==================== FUNCIONES SD CARD ========================
void sd_card_init(void) {
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = SD_MAX_FILES,
        .allocation_unit_size = SD_ALLOC_UNIT_SIZE
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_MOSI_PIN,
        .miso_io_num = SD_MISO_PIN,
        .sclk_io_num = SD_SCLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH_AUTO));

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS_PIN;
    slot_config.host_id = host.slot;

    esp_err_t ret = esp_vfs_fat_sdspi_mount(SD_MOUNT_POINT, &host, &slot_config, &mount_config, &sd_handler.card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Error al montar filesystem. ¿Formatear la tarjeta?");
        } else {
            ESP_LOGE(TAG, "Error al inicializar la tarjeta (%s)", esp_err_to_name(ret));
        }
        return;
    }

    // Mostrar información de la tarjeta
    sdmmc_card_print_info(stdout, sd_handler.card);
    
    // Verificar espacio libre
    FATFS* fs;
    DWORD free_clusters;
    if(f_getfree("0:", &free_clusters, &fs) == FR_OK) {
        uint32_t total_space = (fs->n_fatent - 2) * fs->csize / 2; // KB
        uint32_t free_space = free_clusters * fs->csize / 2; // KB
        ESP_LOGI(TAG, "Espacio en SD: %"PRIu32" KB total, %"PRIu32" KB libre", total_space, free_space);
    }

    // Abrir archivo CSV principal
    char filepath[64];
    snprintf(filepath, sizeof(filepath), "%s/data.csv", SD_MOUNT_POINT);
    sd_handler.csv_file = fopen(filepath, "w"); // Siempre crear nuevo archivo
    
    if (sd_handler.csv_file == NULL) {
        ESP_LOGE(TAG, "Error al abrir archivo CSV para escritura");
        return;
    }

    // Escribir encabezado
    fprintf(sd_handler.csv_file,
        "timestamp,velocidad_viento,voltaje,temperatura,humedad,presion,altitud,latitud,longitud,esp_now_conectado,uso_memoria,rpm\n");
    fflush(sd_handler.csv_file);
    fsync(fileno(sd_handler.csv_file));

    // Inicializar contadores
    sd_handler.sample_count = 0;
    sd_handler.sum_wind_speed = 0;
    sd_handler.sum_voltage = 0;
    sd_handler.sum_temperature = 0;
    sd_handler.sum_humidity = 0;
    sd_handler.sum_pressure = 0;
    sd_handler.sum_altitude = 0;
    sd_handler.sum_latitud = 0;
    sd_handler.sum_longitud = 0;
    sd_handler.sum_rpm = 0;
    sd_handler.valid_gps_samples = 0;
}

void reset_sd_file() {
    if (sd_handler.csv_file) {
        fclose(sd_handler.csv_file);
    }
    
    char filepath[64];
    snprintf(filepath, sizeof(filepath), "%s/data.csv", SD_MOUNT_POINT);
    sd_handler.csv_file = fopen(filepath, "w");
    
    if (sd_handler.csv_file) {
        fprintf(sd_handler.csv_file,
            "timestamp,velocidad_viento,voltaje,temperatura,humedad,presion,altitud,latitud,longitud,esp_now_conectado,uso_memoria,rpm\n");
        fflush(sd_handler.csv_file);
        fsync(fileno(sd_handler.csv_file));
        ESP_LOGI(TAG, "Archivo CSV reiniciado");
    } else {
        ESP_LOGE(TAG, "Error al reiniciar archivo CSV");
    }
}

void sd_write_sample(void) {
    static uint32_t numero_muestra = 0;
    numero_muestra++;
    
    // Verificar conexión ESP-NOW
    bool conectado = (esp_timer_get_time() - ultima_recepcion_espnow) < 3000000;
    float voltaje_actual = conectado ? voltaje_remoto : -1.0f;
    float rpm_actual = conectado ? rpm_recibida : -1.0f;
    
    const size_t HEAP_TOTAL = 320000;
    size_t libre = esp_get_free_heap_size();
    float porcentaje_uso = 100.0f * (1.0f - (float)libre / HEAP_TOTAL);

    // Escribir datos en CSV
    fprintf(sd_handler.csv_file,
        "%s %s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.6f,%.6f,%s,%.2f,%.1f\n",
        strDate, strTime,
        velocidad_viento, 
        voltaje_actual,
        bmeValues.temperature,
        bmeValues.humidity,
        bmeValues.pressure,
        44330.0f * (1.0f - pow((bmeValues.pressure + 151.0f) / SEA_LEVEL_PRESSURE_HPA, 0.1903f)),
        gps_latitud, 
        gps_longitud,
        conectado ? "true" : "false",
        porcentaje_uso,
        rpm_actual
    );
    fflush(sd_handler.csv_file);
    fsync(fileno(sd_handler.csv_file));

    // Acumular para promedio
    sd_handler.sample_count++;
    sd_handler.sum_wind_speed += velocidad_viento;
    
    if(conectado) {
        sd_handler.sum_voltage += voltaje_actual;
        sd_handler.sum_rpm += rpm_actual;
    }
    
    sd_handler.sum_temperature += (bmeValues.temperature - 3.8f);
    sd_handler.sum_humidity += (bmeValues.humidity + 20.0f);
    sd_handler.sum_pressure += (bmeValues.pressure + 151.0f);
    sd_handler.sum_altitude += 44330.0f * (1.0f - pow((bmeValues.pressure + 151.0f) / SEA_LEVEL_PRESSURE_HPA, 0.1903f));
    
    if(fabs(gps_latitud) > 0.000001 || fabs(gps_longitud) > 0.000001) {
        sd_handler.sum_latitud += gps_latitud;
        sd_handler.sum_longitud += gps_longitud;
        sd_handler.valid_gps_samples++;
    }
}

void sd_card_deinit(void) {
    if (sd_handler.csv_file) {
        fclose(sd_handler.csv_file);
        sd_handler.csv_file = NULL;
    }
    
    if (sd_handler.card) {
        esp_vfs_fat_sdcard_unmount(SD_MOUNT_POINT, sd_handler.card);
        sd_handler.card = NULL;
    }
    
    spi_bus_free(SPI2_HOST);
}

// ============== I2C / BME680 / OLED / DS3231 init =============
void I2CMasterInit(void) {
    i2c_master_bus_config_t cfg = {
        .i2c_port = I2C0_PORT,
        .sda_io_num = I2C0_SDA,
        .scl_io_num = I2C0_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };
    i2c_new_master_bus(&cfg, &I2C0BusHandle);
}

void OLEDInit(void) {
    if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(1000))) {
        ssd1306_init(I2C0BusHandle, &OLEDDevice,
                    OLED_ADDR, OLED_ANCHO, OLED_ALTO);
        ssd1306_clear_screen(&OLEDDevice, false);
        ssd1306_contrast(&OLEDDevice, 255);
        xSemaphoreGive(i2c_mutex);
    } else {
        ESP_LOGE(TAG, "Timeout al inicializar OLED (no se pudo obtener mutex I2C)");
    }
}

void UARTInit(void) {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_RX_TX, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_RX_TX, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_RX_TX, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // Reset de buffers
    uart_flush(UART_PORT_RX_TX);
    ESP_LOGI(TAG, "UART inicializado. TX: GPIO%d, RX: GPIO%d", UART_TX_PIN, UART_RX_PIN);
}

void uart_receive_task(void *arg) {
    uint8_t rx_buffer[UART_BUF_SIZE];
    uint32_t total_bytes = 0;

    while (1) {
        int len = uart_read_bytes(UART_PORT_RX_TX, rx_buffer, sizeof(rx_buffer) - 1, pdMS_TO_TICKS(100));

        if (len > 0) {
            rx_buffer[len] = '\0';
            total_bytes += len;
            ESP_LOGI(TAG_UART, "Recibidos %d bytes (total: %"PRIu32")", len, total_bytes);
            ESP_LOG_BUFFER_HEXDUMP(TAG_UART, rx_buffer, len, ESP_LOG_INFO);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

bool BMEInit(void) {
    if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(1000))) {
        esp_err_t ret = bme680_init(I2C0BusHandle, &BMEDevice, BME_ADDR);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG_BME, "Error inicializando BME680: %s", esp_err_to_name(ret));
            xSemaphoreGive(i2c_mutex);
            return false;
        }
        
        ret = bme680_init_sensor(&BMEDevice);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG_BME, "Error configurando BME680: %s", esp_err_to_name(ret));
            xSemaphoreGive(i2c_mutex);
            return false;
        }
        
        bme680_set_oversampling_rates(&BMEDevice,
            BME680_OSR_2X, BME680_OSR_1X, BME680_OSR_1X);
        bme680_set_filter_size(&BMEDevice, BME680_IIR_SIZE_3);
        bme680_set_heater_profile(&BMEDevice, 0, 150, 50);
        bme680_use_heater_profile(&BMEDevice, 0);
        bme680_set_ambient_temperature(&BMEDevice, 25);
        bme680_get_measurement_duration(&BMEDevice, &duracionMedicion);
        
        xSemaphoreGive(i2c_mutex);
        bme_initialized = true;
        ESP_LOGI(TAG_BME, "BME680 inicializado correctamente");
        return true;
    } else {
        ESP_LOGE(TAG_BME, "Timeout al inicializar BME680");
        return false;
    }
}

bool read_bme680_values(BME680_values_float_t *values) {
    if (!bme_initialized) {
        ESP_LOGE(TAG_BME, "BME no inicializado");
        return false;
    }
    
    if (xSemaphoreTake(bme_mutex, pdMS_TO_TICKS(1000))) {
        esp_err_t ret;
        int retry_count = 0;
        const int max_retries = 3;
        
        do {
            if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(1000))) {
                ret = bme680_force_measurement(&BMEDevice);
                xSemaphoreGive(i2c_mutex);
            } else {
                ESP_LOGE(TAG_BME, "Timeout I2C mutex");
                xSemaphoreGive(bme_mutex);
                return false;
            }
            
            if (ret != ESP_OK) {
                ESP_LOGE(TAG_BME, "Error forzando medición (intento %d): %s", 
                        retry_count+1, esp_err_to_name(ret));
                vTaskDelay(pdMS_TO_TICKS(100));
                retry_count++;
                continue;
            }
            
            vTaskDelay(duracionMedicion + pdMS_TO_TICKS(50));
            
            if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(1000))) {
                ret = bme680_get_results_float(&BMEDevice, values);
                xSemaphoreGive(i2c_mutex);
            } else {
                ESP_LOGE(TAG_BME, "Timeout I2C mutex");
                xSemaphoreGive(bme_mutex);
                return false;
            }
            
            if (ret == ESP_OK) {
                xSemaphoreGive(bme_mutex);
                return true;
            } else {
                ESP_LOGE(TAG_BME, "Error leyendo resultados (intento %d): %s",
                        retry_count+1, esp_err_to_name(ret));
                retry_count++;
                vTaskDelay(pdMS_TO_TICKS(200));
            }
        } while (retry_count < max_retries);
        
        ESP_LOGW(TAG_BME, "Reiniciando BME680 después de %d fallos", max_retries);
        BMEInit();
        xSemaphoreGive(bme_mutex);
    }
    
    return false;
}

void bme_watchdog_task(void *arg) {
    while (1) {
        static int error_count = 0;
        const int max_errors = 5;
        
        BME680_values_float_t values;
        if (!read_bme680_values(&values)) {
            error_count++;
            ESP_LOGW(TAG_BME, "Error en lectura BME (contador: %d/%d)", error_count, max_errors);
        } else {
            error_count = 0;
        }
        
        if (error_count >= max_errors) {
            ESP_LOGE(TAG_BME, "Demasiados errores, reiniciando BME680");
            BMEInit();
            error_count = 0;
        }
        
        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void DS3231Init(void) {
    i2c_device_config_t dcfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DS3231_ADDR,
        .scl_speed_hz = 400000,
        .flags.disable_ack_check = false,
    };
    i2c_master_bus_add_device(I2C0BusHandle, &dcfg, &DS3231DevHandle);
}

void DS3231SetHora(uint8_t hrs, uint8_t min, uint8_t seg,
                   uint8_t formt, uint8_t Am_Pm) {
    switch(formt) {
        case FORMAT_12:
            seg = ((seg/10)<<4)|(seg%10);
            min = ((min/10)<<4)|(min%10);
            hrs = ((hrs/10)<<4)|(hrs%10);
            if (Am_Pm == AM) hrs = (hrs & 0b00011111) | 0b01000000;
            else             hrs = (hrs & 0b00011111) | 0b01100000;
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x00, seg}, 2, 10);
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x01, min}, 2, 10);
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x02, hrs}, 2, 10);
            break;
        case FORMAT_24:
            seg = ((seg/10)<<4)|(seg%10);
            min = ((min/10)<<4)|(min%10);
            hrs = (((hrs/10)<<4)|(hrs%10)) & 0b00111111;
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x00, seg}, 2, 10);
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x01, min}, 2, 10);
            i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x02, hrs}, 2, 10);
            break;
    }
}

void DS3231SetFecha(uint8_t diaSem, uint8_t diaMes,
                    uint8_t mes, uint8_t anio) {
    diaSem = diaSem & 0b00000111;
    diaMes = ((diaMes/10)<<4)|(diaMes%10);
    mes    = ((mes/10)<<4)|(mes%10);
    anio   = ((anio/10)<<4)|(anio%10);
    i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x03, diaSem}, 2, 10);
    i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x04, diaMes}, 2, 10);
    i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x05, mes},    2, 10);
    i2c_master_transmit(DS3231DevHandle, (uint8_t[]){0x06, anio},  2, 10);
}

void DS3231ReadHora(uint8_t *pHrs, uint8_t *pMin,
                    uint8_t *pSeg, char *pAmPm) {
    uint8_t data[3];
    i2c_master_transmit_receive(DS3231DevHandle, (uint8_t[]){0x00}, 1, data, 3, 10);
    *pSeg = data[0];
    *pMin = data[1];
    *pHrs = data[2];
    if (*pHrs & (1<<6)) {
        strcpy(pAmPm, (*pHrs & (1<<5)) ? "PM" : "AM");
        *pHrs &= 0b00011111;
    } else {
        strcpy(pAmPm, "");
        *pHrs &= 0b00111111;
    }
    *pHrs = ((*pHrs >> 4)*10) + (*pHrs & 0x0F);
    *pMin = ((*pMin >> 4)*10) + (*pMin & 0x0F);
    *pSeg = ((*pSeg >> 4)*10) + (*pSeg & 0x0F);
}

void DS3231ReadFecha(uint8_t *pDiaSem, uint8_t *pDiaMes,
                     uint8_t *pMes, uint8_t *pAnio) {
    uint8_t data[4];
    i2c_master_transmit_receive(DS3231DevHandle, (uint8_t[]){0x03}, 1, data, 4, 10);
    *pDiaSem = data[0] & 0b00000111;
    *pDiaMes = ((data[1]>>4)*10) + (data[1]&0x0F);
    *pMes    = ((data[2]>>4)*10) + (data[2]&0x0F);
    *pAnio   = ((data[3]>>4)*10) + (data[3]&0x0F);
}

// ========================= data_task (1 Hz) ====================
void data_task(void *arg) {
    bool led_state = false;
    const size_t HEAP_TOTAL = 320000;
    uint32_t local_sample_counter = 0;

    while (1) {
        // Leer RTC
        DS3231ReadHora(&rtcData.hora, &rtcData.min, &rtcData.seg, strFormtAmPm);
        DS3231ReadFecha(&rtcData.diaSemana, &rtcData.diaMes, &rtcData.mes, &rtcData.anio);
        snprintf(strTime, sizeof(strTime), "%02d:%02d:%02d", rtcData.hora, rtcData.min, rtcData.seg);
        snprintf(strDate, sizeof(strDate), "%02d/%02d/%02d", rtcData.diaMes, rtcData.mes, rtcData.anio);

        if (read_bme680_values(&bmeValues)) {
            // Verificar conexión ESP-NOW
            bool conectado = (esp_timer_get_time() - ultima_recepcion_espnow) < 3000000;
            
            // Memoria
            size_t libre = esp_get_free_heap_size();
            float pct = 100.0f * (1.0f - (float)libre / HEAP_TOTAL);

            // Escribir en SD si está inicializada
            if (sd_handler.csv_file) {
                sd_write_sample();
                local_sample_counter++;
                
                ESP_LOGI("DATA_TASK", "Muestra %"PRIu32" de 10", local_sample_counter);

                if (local_sample_counter >= 10) {
                    // Calcular promedios
                    float avg_wind = sd_handler.sum_wind_speed / 10;
                    float avg_volt = (sd_handler.sum_voltage > 0) ? (sd_handler.sum_voltage / 10) : -1.0f;
                    float avg_rpm = (sd_handler.sum_rpm > 0) ? (sd_handler.sum_rpm / 10) : -1.0f;
                    float avg_temp = sd_handler.sum_temperature / 10;
                    float avg_hum = sd_handler.sum_humidity / 10;
                    float avg_pres = sd_handler.sum_pressure / 10;
                    float avg_alt = sd_handler.sum_altitude / 10;
                    
                    float avg_lat = 0.0f, avg_lon = 0.0f;
                    if(sd_handler.valid_gps_samples > 0) {
                        avg_lat = sd_handler.sum_latitud / sd_handler.valid_gps_samples;
                        avg_lon = sd_handler.sum_longitud / sd_handler.valid_gps_samples;
                    }

                    // Generar JSON
                    char json_buffer[512];
                    snprintf(json_buffer, sizeof(json_buffer),
                        "{\"timestamp\":\"%s %s\","
                        "\"wind_speed\":%.2f,"
                        "\"voltage\":%.2f,"
                        "\"temperature\":%.2f,"
                        "\"humidity\":%.2f,"
                        "\"pressure\":%.2f,"
                        "\"altitude\":%.2f,"
                        "\"lat\":%.6f,"
                        "\"lon\":%.6f,"
                        "\"rpm\":%.1f,"
                        "\"esp_now_connected\":%s,"
                        "\"memory_usage\":%.2f}\n",
                        strDate, strTime,
                        avg_wind,
                        avg_volt,
                        avg_temp,
                        avg_hum,
                        avg_pres,
                        avg_alt,
                        avg_lat,
                        avg_lon,
                        avg_rpm,
                        conectado ? "true" : "false",
                        pct
                    );

                    ESP_LOGI(TAG_SD, "JSON generado: %s", json_buffer);
                    
                    // Enviar por UART
                    int sent = uart_write_bytes(UART_PORT_RX_TX, json_buffer, strlen(json_buffer));
                    if (sent > 0) {
                        ESP_LOGI(TAG_UART_TX, "Enviados %d bytes por UART", sent);
                    } else {
                        ESP_LOGE(TAG_UART_TX, "Error al enviar por UART: %d", sent);
                    }

                    // Reiniciar contadores y archivo
                    local_sample_counter = 0;
                    sd_handler.sample_count = 0;
                    sd_handler.sum_wind_speed = 0;
                    sd_handler.sum_voltage = 0;
                    sd_handler.sum_temperature = 0;
                    sd_handler.sum_humidity = 0;
                    sd_handler.sum_pressure = 0;
                    sd_handler.sum_altitude = 0;
                    sd_handler.sum_latitud = 0;
                    sd_handler.sum_longitud = 0;
                    sd_handler.sum_rpm = 0;
                    sd_handler.valid_gps_samples = 0;
                    
                    reset_sd_file();
                }
            }
        }
        
        led_state = !led_state;
        gpio_set_level(LED_GPIO, led_state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void display_task(void *arg) {
    const int64_t DURATION_US = 10 * 1000000; // 10 segundos por sección

    while (1) {
        // Sección 1: Datos principales
        int64_t t0 = esp_timer_get_time();
        while (esp_timer_get_time() - t0 < DURATION_US) {
            ssd1306_clear_screen(&OLEDDevice, false);
            ssd1306_display_text(&OLEDDevice, 0, "TESIS ESP-TICs", strlen("TESIS ESP-TICs"), false);
            ssd1306_display_text(&OLEDDevice, 1, strTime, strlen(strTime), false);
            ssd1306_display_text(&OLEDDevice, 2, strDate, strlen(strDate), false);

            char buf[32];
            sprintf(buf, "Vel: %.2fm/s", velocidad_viento);
            ssd1306_display_text(&OLEDDevice, 3, buf, strlen(buf), false);

            float volt_actual = -1.0f;
            float rpm_actual = -1.0f;
            if (xSemaphoreTake(espnow_mutex, pdMS_TO_TICKS(100))) {
                volt_actual = voltaje_remoto;
                rpm_actual = rpm_recibida;
                xSemaphoreGive(espnow_mutex);
            }
            sprintf(buf, "Volt: %.2fV", volt_actual);
            ssd1306_display_text(&OLEDDevice, 4, buf, strlen(buf), false);

            sprintf(buf, "RPM: %.1f", rpm_actual);
            ssd1306_display_text(&OLEDDevice, 5, buf, strlen(buf), false);

            sprintf(buf, "Lat: %.6f", gps_latitud);
            ssd1306_display_text(&OLEDDevice, 6, buf, strlen(buf), false);

            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        // Sección 2: Datos ambientales
        t0 = esp_timer_get_time();
        while (esp_timer_get_time() - t0 < DURATION_US) {
            if (read_bme680_values(&bmeValues)) {
                size_t libre = esp_get_free_heap_size();
                float pct = 100.0f * (1.0f - (float)libre / 320000);

                ssd1306_clear_screen(&OLEDDevice, false);
                char strMem[20];
                snprintf(strMem, sizeof(strMem), "Mem: %.2f%%", pct);
                ssd1306_display_text(&OLEDDevice, 0, strMem, strlen(strMem), false);

                ssd1306_display_text(&OLEDDevice, 1, strTime, strlen(strTime), false);
                ssd1306_display_text(&OLEDDevice, 2, strDate, strlen(strDate), false);

                sprintf(strTemp,  "Temp: %.2f C", bmeValues.temperature - 3.8f);
                sprintf(strHumd,  "Hum:  %.2f%%", bmeValues.humidity + 22.0f);
                sprintf(strPress, "Pres: %.2fhPa", bmeValues.pressure + 151.0f);
                float alt = 44330.0f * (1.0f - pow((bmeValues.pressure + 151.0f) / SEA_LEVEL_PRESSURE_HPA, 0.1903f));
                sprintf(strAlt,   "Alt:  %.1fm", alt);

                ssd1306_display_text(&OLEDDevice, 3, strTemp,  strlen(strTemp),  false);
                ssd1306_display_text(&OLEDDevice, 4, strHumd,  strlen(strHumd),  false);
                ssd1306_display_text(&OLEDDevice, 5, strPress, strlen(strPress), false);
                ssd1306_display_text(&OLEDDevice, 6, strAlt,   strlen(strAlt),   false);
            }

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

// ========================= GPIOInit ============================
void GPIOInit(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SENSOR_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(LED_GPIO, 0));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(SENSOR_GPIO, sensor_isr_handler, NULL));
}

// ========================= GPTimerInit =========================
void GPTimerInit(void) {
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&config, &gptimer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    gptimer_alarm_config_t alarm = {
        .alarm_count = INTERVALO_SEGUNDOS * 1000000,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

// =============== inicializar_espnow_receptor ===================
void inicializar_espnow_receptor(void) {
    ESP_ERROR_CHECK(esp_now_init());
    
    const uint8_t pmk[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    ESP_ERROR_CHECK(esp_now_set_pmk(pmk));

    esp_now_peer_info_t peer_info = {
        .channel = 1,
        .encrypt = false,
    };
    uint8_t mac_addr[6] = ESP1_MAC;
    memcpy(peer_info.peer_addr, mac_addr, 6);

    esp_err_t result = esp_now_add_peer(&peer_info);
    if (result != ESP_OK) {
        ESP_LOGE(TAG_ESPNOW, "Error al añadir peer: %s", esp_err_to_name(result));
        return;
    }

    result = esp_now_register_recv_cb(on_receive_espnow);
    if (result != ESP_OK) {
        ESP_LOGE(TAG_ESPNOW, "Error al registrar callback: %s", esp_err_to_name(result));
        return;
    }
    
    uint8_t local_mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, local_mac);
    ESP_LOGI(TAG_ESPNOW, "MAC Local: %02X:%02X:%02X:%02X:%02X:%02X", 
            local_mac[0], local_mac[1], local_mac[2], 
            local_mac[3], local_mac[4], local_mac[5]);
    ESP_LOGI(TAG_ESPNOW, "MAC Remota: %02X:%02X:%02X:%02X:%02X:%02X", 
            mac_addr[0], mac_addr[1], mac_addr[2], 
            mac_addr[3], mac_addr[4], mac_addr[5]);
}

void app_main(void) {
    // Crear mutexes
    i2c_mutex = xSemaphoreCreateMutex();
    bme_mutex = xSemaphoreCreateMutex();
    espnow_mutex = xSemaphoreCreateMutex();
    
    if (!i2c_mutex || !bme_mutex || !espnow_mutex) {
        ESP_LOGE(TAG, "Error creando mutexes");
        return;
    }

    // Inicializar NVS, red y Wi-Fi
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t wcfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wcfg));
    
    // Configurar WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t wifi_config = { .sta = { .channel = 1 } };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Inicializar periféricos
    GPIOInit();
    UARTInit();
    GPTimerInit();
    inicializar_espnow_receptor();
    I2CMasterInit();
    OLEDInit();
    
    // Inicializar BME680
    int bme_init_attempts = 0;
    const int max_bme_init_attempts = 3;
    while (!BMEInit() && bme_init_attempts < max_bme_init_attempts) {
        bme_init_attempts++;
        ESP_LOGE(TAG, "Falló inicialización BME680 (intento %d/%d)", 
                bme_init_attempts, max_bme_init_attempts);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    
    if (bme_init_attempts >= max_bme_init_attempts) {
        ESP_LOGE(TAG, "No se pudo inicializar BME680");
        while(1) { vTaskDelay(1000); }
    }

    DS3231Init();
    sd_card_init();

    // Crear tareas
    xTaskCreate(monitor_memoria_task, "mem_task", 2048, NULL, 4, NULL);
    xTaskCreate(data_task, "data_task", 4096, NULL, 3, NULL);
    xTaskCreate(display_task, "display_task", 4096, NULL, 2, NULL);
    xTaskCreate(uart_receive_task, "uart_receive_task", 4096, NULL, 5, NULL);
    xTaskCreate(bme_watchdog_task, "bme_watchdog", 2048, NULL, 3, NULL);

    ESP_LOGI(TAG, "Sistema iniciado completamente");
}