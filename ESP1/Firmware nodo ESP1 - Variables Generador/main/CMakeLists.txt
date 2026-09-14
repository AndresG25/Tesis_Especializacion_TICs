idf_component_register(
    SRCS "main7.c"
    INCLUDE_DIRS "."
    REQUIRES 
      nvs_flash
      esp_netif
      esp_event
      esp_wifi        # sólo este
      driver
      freertos
      minmea
      esp_timer
      esp_adc
)