cmake_minimum_required(VERSION 3.14)
project(SmartLightingTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Google Test (descargado automáticamente) ───────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Evitar instalar gtest en el sistema
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── Biblioteca con la lógica del sistema y los mocks de hardware ───────────
add_library(smart_logic STATIC
    logic.cpp
    mocks/arduino_mock.cpp
)
target_include_directories(smart_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ── Ejecutable de tests (todos los módulos en un solo runner) ──────────────
add_executable(smart_lighting_tests
    unit/test_color.cpp
    unit/test_auto_mode.cpp
    unit/test_fiesta.cpp
    unit/test_handlers.cpp
    unit/test_estado.cpp
)

target_link_libraries(smart_lighting_tests
    PRIVATE
    smart_logic
    GTest::gtest_main
)

# ── Registro en CTest ──────────────────────────────────────────────────────
include(GoogleTest)
gtest_discover_tests(smart_lighting_tests)
