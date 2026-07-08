file(GLOB_RECURSE app_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

idf_component_register(
    SRCS ${app_sources}
    INCLUDE_DIRS
        "."
        "../../include"
    REQUIRES driver esp_driver_spi esp_driver_gpio esp_driver_i2c
)
