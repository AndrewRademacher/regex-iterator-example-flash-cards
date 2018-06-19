cmake_minimum_required(VERSION 3.5.1)
project(nlohmann-json)

include(ExternalProject)
ExternalProject_Add(project_nlohmann-json
        URL https://github.com/nlohmann/json/archive/v3.1.2.tar.gz
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nlohmanjson
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )

ExternalProject_Get_Property(project_nlohmann-json SOURCE_DIR)

add_library(nlohmann-json INTERFACE)
add_dependencies(nlohmann-json project_nlohmann-json)
target_compile_definitions(nlohmann-json
        INTERFACE LIBRARY_HEADER_ONLY)
target_include_directories(nlohmann-json INTERFACE ${SOURCE_DIR}/include)