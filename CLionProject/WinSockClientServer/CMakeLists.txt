cmake_minimum_required(VERSION 3.28)
project(WinSockClientServer C)

set(CMAKE_C_STANDARD 11)

add_subdirectory("${PROJECT_SOURCE_DIR}/client" "${PROJECT_SOURCE_DIR}/output/client/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/server" "${PROJECT_SOURCE_DIR}/output/server/bin")