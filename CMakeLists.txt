# version 3.11 or later of CMake needed later for installing GoogleTest
# so let's require it now.
cmake_minimum_required(VERSION 3.11-3.18)

project(
    Fight.IO
    VERSION 0.1
    DESCRIPTION "Fight.IO refactor")

add_subdirectory(simplgui)   # look in hello subdirectory for CMakeLists.txt to process
add_subdirectory(common)
add_subdirectory(asio-wrapper)    # look in apps subdirectory for CMakeLists.txt to process
add_subdirectory(Client)    # look in apps subdirectory for CMakeLists.txt to process
add_subdirectory(Server)    # look in apps subdirectory for CMakeLists.txt to process