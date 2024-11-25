include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(srcs "src/led_sht3x.c")
set(public_requires)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "include"
                       REQUIRES ${public_requires})
