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

add_executable(Client main.cpp src/ConnectScreen.cpp src/Drawable_Player.cpp src/GameScreen.cpp src/HpBar.cpp src/Menu.cpp src/Screen.cpp src/Window.cpp)
# We need hello.h and the hello library
target_link_libraries(Client
    PRIVATE simplgui)
target_link_libraries(Client
    PRIVATE common)


# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(Client PUBLIC cxx_std_20)