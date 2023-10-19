cmake_minimum_required(VERSION 3.5)

project(CppInjector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the global build directory depending on target
if (DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
else ()
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/Debug)  # Default to Debug if not specified
endif ()


# Set the global output directory for all targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/Injector)
add_subdirectory(src/Dll)
