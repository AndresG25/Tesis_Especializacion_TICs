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
      bme680
      ssd1306
      esp_timer
      fatfs
      sdmmc
      mqtt
)