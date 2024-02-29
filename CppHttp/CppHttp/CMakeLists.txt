add_executable (CppHttp "src/CppHttp.cpp" "include/CppHttp.hpp" "src/CppHttp.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CppHttp PROPERTY CXX_STANDARD 20)
endif()
