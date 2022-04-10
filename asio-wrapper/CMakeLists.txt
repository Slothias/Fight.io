cmake_minimum_required(VERSION 3.11-3.18)

project(
        asio-wrapper
        VERSION 0.1
        DESCRIPTION "asio-wrapper")

add_subdirectory(asio-server)
add_subdirectory(asio-client)
add_subdirectory(asio-common)
