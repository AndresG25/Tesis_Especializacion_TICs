# La configuración y la identidad del dispositivo se proporcionan localmente.
foreach(local_file IN ITEMS
    "config_local.h"
    "certs/AmazonRootCA1.pem"
    "certs/device_cert.crt"
    "certs/private_key.key")
    if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${local_file}")
        message(FATAL_ERROR
            "Falta main/${local_file}. Consulte ESP3/README.md antes de compilar.")
    endif()
endforeach()

idf_component_register(
    SRCS "main2.c"
    INCLUDE_DIRS "."
    REQUIRES 
        mbedtls 
        esp-tls 
        esp_netif 
        esp_event
        esp_wifi
        nvs_flash
        mqtt
        driver
        freertos
        esp_timer
    EMBED_FILES 
        "certs/AmazonRootCA1.pem"
        "certs/device_cert.crt"
        "certs/private_key.key"
)
