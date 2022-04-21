# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(Client)

include_directories(include)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME}  main.cpp ${SRC_FILES} )

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(Client PRIVATE simplgui)
target_link_libraries(Client PRIVATE common)

target_link_libraries(Client PRIVATE asio-client)


# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(Client PUBLIC cxx_std_20)