cmake_minimum_required(VERSION 3.10)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(chatServer)

add_executable(chatServer
                main.cpp
                global.cpp
                msgProcess.cpp
                user.cpp)

target_link_libraries(chatServer pthread)