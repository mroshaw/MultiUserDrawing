cmake_minimum_required(VERSION 3.28)
project(WinSockClientServer C)

set(CMAKE_C_STANDARD 11)

# Links the WinSock library for Windows targets
link_libraries(ws2_32 wsock32)

add_executable(Client
        winclient.c
        winclient.h)
