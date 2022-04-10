
cmake_minimum_required(VERSION 3.11-3.18)

project(asio-client)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME}  ${SRC_FILES} main.cpp)


target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE boost_system)
# Tell compiler to use C++20 features. The code doesn't actually use any of them.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)