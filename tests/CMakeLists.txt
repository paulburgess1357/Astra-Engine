# astra_add_test(<lib>  SOURCES <files...>)
# Creates tests/<lib> exe astra_<lib>_tests linking astra::astra_<lib> + gtest_main.
function(astra_add_test lib)
    cmake_parse_arguments(ARG "" "" "SOURCES" ${ARGN})
    set(target astra_${lib}_tests)
    astra_add_executable(${target}
        SOURCES ${ARG_SOURCES}
        DEPS astra::astra_${lib} GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()

add_subdirectory(core)
add_subdirectory(platform)
add_subdirectory(gpu)
add_subdirectory(renderer)
