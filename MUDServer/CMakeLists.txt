cmake_minimum_required(VERSION 3.28)
project(MUD C)
set(CMAKE_C_STANDARD 11)

# Links the WinSock library for Windows targets
link_libraries(ws2_32 wsock32)

# Main MUDSERVER executable
add_executable(MUDSERVER main.c
        help.c
        matrix.c
        muderror.c
        object.c
        parse.c
        scene.c
        server.c
        utils.c
        file.c
        winserver.c
        serverdetails.c
        serverdetails.h
)