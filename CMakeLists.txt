cmake_minimum_required(VERSION 3.10)

project(chatServer)

add_executable(chatServer
                main.cpp
                global.cpp
                msgProcess.cpp
                user.cpp)

target_link_libraries(chatServer pthread)