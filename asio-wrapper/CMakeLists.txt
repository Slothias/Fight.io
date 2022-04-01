
cmake_minimum_required(VERSION 3.11-3.18)

project(asio-wrapper)


file(GLOB SRC_FILES src/*.cpp)
add_executable(asio-wrapper main.cpp ${SRC_FILES})

# PUBLIC needed to make both hello.h and hello library available elsewhere in project
target_include_directories(asio-wrapper
        PUBLIC ${PROJECT_SOURCE_DIR}/include)


# Tell compiler to use C++20 features. The code doesn't actually use any of them.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)