astra_add_library(astra_platform
    SOURCES
        platform.cpp
        platform.hpp
        window.cpp
        window.hpp
    DEPS astra::astra_core glfw)
