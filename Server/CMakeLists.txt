# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(Server)

include_directories(include)
add_executable(Server main.cpp include src)
# We need hello.h and the hello library
target_link_libraries(Server
    PRIVATE simplgui)
target_link_libraries(Server
    PRIVATE common)

# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(Server PUBLIC cxx_std_20)