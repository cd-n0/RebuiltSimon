add_library(ResParser STATIC "libresparser.c" "libresparser.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ResParser PROPERTY C_STANDARD 99)
endif()
