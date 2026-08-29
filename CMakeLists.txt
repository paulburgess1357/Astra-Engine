cmake_minimum_required(VERSION 3.25)

project(AstraEngine VERSION 0.1.0 LANGUAGES CXX)

option(ASTRA_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(ASTRA_ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(cmake/Warnings.cmake)

if(ASTRA_ENABLE_CLANG_TIDY)
    find_program(ASTRA_CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY
        "${ASTRA_CLANG_TIDY_EXECUTABLE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
endif()

add_executable(astra_engine src/main.cpp)
target_compile_features(astra_engine PRIVATE cxx_std_23)
astra_enable_warnings(astra_engine)

if(BUILD_TESTING)
    add_test(NAME astra_smoke COMMAND astra_engine)
    set_tests_properties(astra_smoke PROPERTIES PASS_REGULAR_EXPRESSION "Hello from Astra")
endif()

find_program(ASTRA_CLANG_FORMAT_EXECUTABLE NAMES clang-format)
file(GLOB_RECURSE ASTRA_FORMAT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/tests/*.hpp")

if(ASTRA_CLANG_FORMAT_EXECUTABLE)
    add_custom_target(format-check
        COMMAND "${ASTRA_CLANG_FORMAT_EXECUTABLE}" --dry-run --Werror --style=file
            ${ASTRA_FORMAT_SOURCES}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Checking C++ formatting")

    add_custom_target(format
        COMMAND "${ASTRA_CLANG_FORMAT_EXECUTABLE}" -i --style=file ${ASTRA_FORMAT_SOURCES}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Formatting C++ sources")
else()
    add_custom_target(format-check
        COMMAND "${CMAKE_COMMAND}" -E echo "clang-format was not found on PATH."
        COMMAND "${CMAKE_COMMAND}" -E false)
endif()
