cmake_minimum_required(VERSION 3.28)
project(MUD C)

set(CMAKE_C_STANDARD 11)

add_subdirectory("${PROJECT_SOURCE_DIR}/MUDClient" "${PROJECT_SOURCE_DIR}/output/mudclient/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/MUDServer" "${PROJECT_SOURCE_DIR}/output/mudserver/bin")