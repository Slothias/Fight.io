# version 3.11 or later of CMake or needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(Client)

include_directories(include)

file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME}  main.cpp  )

target_include_directories(${PROJECT_NAME} PUBLIC include)
#target_link_libraries(Client PRIVATE simplgui)
target_link_libraries(Client PRIVATE common)

target_link_libraries(${PROJECT_NAME} PRIVATE -lsfml-graphics -lsfml-window -lsfml-system)

target_link_libraries(Client PRIVATE asio-client)


# Tell C++ compiler to use C++20 features. We don't actually use any of them.
target_compile_features(Client PUBLIC cxx_std_20)
