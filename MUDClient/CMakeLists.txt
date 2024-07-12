cmake_minimum_required(VERSION 3.28)
project(MUD C)
set(CMAKE_C_STANDARD 11)

# Include directories for Tcl/Tk 8.6
include_directories(MUDCLIENT PUBLIC D:/Dev/ActiveTcl/include)

# Tcl library to link
add_library(tcl STATIC IMPORTED)
set_property(TARGET tcl PROPERTY
        IMPORTED_LOCATION "D:/Dev/ActiveTcl/lib/tcl86t.lib")

# Tk library to link
add_library(tk STATIC IMPORTED)
set_property(TARGET tk PROPERTY
        IMPORTED_LOCATION "D:/Dev/ActiveTcl/lib/tk86t.lib")

# Links the WinSock library for Windows targets
link_libraries(ws2_32 wsock32)

# Main MUDCLIENT executable
add_executable(MUDCLIENT main.c
        tclInit.c
        utils.c
        winclient.c)

# Add targets for external libs
target_link_libraries(MUDCLIENT PRIVATE tcl)
target_link_libraries(MUDCLIENT PRIVATE tk)