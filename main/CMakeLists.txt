# CMakeLists.txt
idf_component_register(
    SRCS "sht3x.c" "example.c"
    INCLUDE_DIRS "."
    REQUIRES freertos driver
)
