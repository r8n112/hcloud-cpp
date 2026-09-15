cmake_minimum_required(VERSION 3.28)

project(
    hcloud_cpp
    VERSION 0.1.0
    DESCRIPTION "Low-level C++ client for the Hetzner Cloud API"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProjectOptions)

add_library(hcloud_cpp src/client.cpp)
add_library(hcloud::hcloud_cpp ALIAS hcloud_cpp)

target_include_directories(
    hcloud_cpp
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
)

target_compile_features(hcloud_cpp PUBLIC cxx_std_23)
set_target_properties(
    hcloud_cpp
    PROPERTIES CXX_STANDARD 23
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF
)

r8n112_enable_warnings(hcloud_cpp)
r8n112_enable_sanitizers(hcloud_cpp)

option(R8N112_BUILD_TESTS "Build the test suite" ON)
option(R8N112_BUILD_EXAMPLES "Build the examples" ON)

if(R8N112_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(R8N112_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
