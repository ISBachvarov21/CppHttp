include_directories("./src/dependencies/CppHttp/include")

add_executable (CppHttp "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CppHttp PROPERTY CXX_STANDARD 20)
endif()