add_subdirectory(core)
add_subdirectory(platform)
add_subdirectory(gpu)
add_subdirectory(renderer)

astra_add_executable(astra_engine
    SOURCES main.cpp
    DEPS astra::astra_core astra::astra_platform astra::astra_renderer)
