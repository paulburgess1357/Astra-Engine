cmake_minimum_required(VERSION 3.25)

project(AstraEngine VERSION 0.1.0 LANGUAGES CXX)

option(ASTRA_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(ASTRA_ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND UNIX)
    set(ASTRA_COMPILE_COMMANDS_LINK "${CMAKE_SOURCE_DIR}/build/compile_commands.json")
    file(REMOVE "${ASTRA_COMPILE_COMMANDS_LINK}")
    file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${ASTRA_COMPILE_COMMANDS_LINK}" SYMBOLIC)
endif()

include(CTest)
include(cmake/Sanitizers.cmake)
include(cmake/Warnings.cmake)

if(ASTRA_ENABLE_CLANG_TIDY)
    find_program(ASTRA_CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY
        "${ASTRA_CLANG_TIDY_EXECUTABLE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
endif()

find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(astra_engine src/main.cpp)
target_compile_features(astra_engine PRIVATE cxx_std_23)
target_link_libraries(astra_engine PRIVATE
    Vulkan::Vulkan
    GPUOpen::VulkanMemoryAllocator
    glfw
    spdlog::spdlog)
astra_enable_sanitizers(astra_engine)
astra_enable_warnings(astra_engine)

if(BUILD_TESTING)
    add_test(NAME astra_smoke COMMAND astra_engine)
    set_tests_properties(astra_smoke PROPERTIES
        PASS_REGULAR_EXPRESSION "Hello from Astra"
        TIMEOUT 60)
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
