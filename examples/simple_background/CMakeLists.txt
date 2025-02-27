# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.15)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(SimpleBackground C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

include_directories(${CMAKE_BINARY_DIR}/include)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

# Add all the source files in the lib directory to the project
AUX_SOURCE_DIRECTORY(lib SUB_SOURCES)

# Add the source files to the project
target_sources(${PROJECT_NAME} PUBLIC 
    ${SUB_SOURCES}
)

# Add the include directories to the project
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/../../src ${CMAKE_BINARY_DIR}/build)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    PicoGFX
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)