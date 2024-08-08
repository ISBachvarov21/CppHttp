cmake_minimum_required(VERSION 3.13)
project("CppHttp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/dependencies/cpphttp/include")

file (GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(docker ${SOURCES})

set_property(TARGET docker PROPERTY CXX_STANDARD 20)