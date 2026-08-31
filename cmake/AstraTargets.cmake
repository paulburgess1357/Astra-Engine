# astra_add_library(<name> SOURCES <files...> [DEPS <targets...>])
#
# Creates a static library with the project-wide warning, sanitizer, and
# language settings applied. Headers are included relative to src/, e.g.
# #include "core/log.hpp".
function(astra_add_library name)
    cmake_parse_arguments(ARG "" "" "SOURCES;DEPS" ${ARGN})

    add_library(${name} STATIC ${ARG_SOURCES})
    add_library(astra::${name} ALIAS ${name})
    target_compile_features(${name} PUBLIC cxx_std_23)
    target_include_directories(${name} PUBLIC "${CMAKE_SOURCE_DIR}/src")
    if(ARG_DEPS)
        target_link_libraries(${name} PUBLIC ${ARG_DEPS})
    endif()
    astra_enable_sanitizers(${name})
    astra_enable_warnings(${name})
endfunction()

# astra_add_executable(<name> SOURCES <files...> [DEPS <targets...>])
function(astra_add_executable name)
    cmake_parse_arguments(ARG "" "" "SOURCES;DEPS" ${ARGN})

    add_executable(${name} ${ARG_SOURCES})
    target_compile_features(${name} PRIVATE cxx_std_23)
    if(ARG_DEPS)
        target_link_libraries(${name} PRIVATE ${ARG_DEPS})
    endif()
    astra_enable_sanitizers(${name})
    astra_enable_warnings(${name})
endfunction()
