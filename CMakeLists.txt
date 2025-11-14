idf_component_register(SRCS "debugTools.c" "i2c.c" "hello_world_main.c"
                       PRIV_REQUIRES spi_flash
                       INCLUDE_DIRS "include")
