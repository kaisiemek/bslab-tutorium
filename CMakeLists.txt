cmake_minimum_required(VERSION 3.17)
project(modern_cpp_demo)

set(CMAKE_CXX_STANDARD 17)

add_executable(modern_cpp_demo main.cpp vector_demo.cpp mem_management_demo.cpp includes/mem_management_demo.h string_demo.cpp includes/string_demo.h includes/vector_demo.h smart_ptr_demo.cpp includes/smart_ptr_demo.h serialization.cpp includes/serialization.h)