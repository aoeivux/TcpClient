cmake_minimum_required(VERSION 3.10)

project(tcp_client VERSION 1.0 LANGUAGES CXX)



add_executable(tcp_client main.cpp)
# add_executable(test test.cpp)

target_link_libraries(tcp_client ws2_32.lib)


