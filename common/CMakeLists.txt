# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(common)


file(GLOB SRC_FILES src/*.cpp)
add_library(common ${SRC_FILES})

# PUBLIC needed to make both hello.h and hello library available elsewhere in project
target_include_directories(${PROJECT_NAME}
    PUBLIC include)

# Tell compiler to use C++20 features. The code doesn't actually use any of them.
target_compile_features(common PUBLIC cxx_std_20)