# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(ServerUI)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set (source "${PROJECT_SOURCE_DIR}/../common/assets")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")


include_directories(include)
file(GLOB SRC_FILES src/*.cpp src/*/*.cpp)
add_executable(ServerUI main.cpp ${SRC_FILES})

target_link_libraries(ServerUI PRIVATE asio-common)
target_link_libraries(ServerUI PRIVATE asio-server)
target_link_libraries(ServerUI PRIVATE simplgui)
target_link_libraries(ServerUI PRIVATE common)
target_link_libraries(${PROJECT_NAME} PRIVATE -lsfml-graphics -lsfml-window -lsfml-system)


target_compile_features(ServerUI PUBLIC cxx_std_20)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)