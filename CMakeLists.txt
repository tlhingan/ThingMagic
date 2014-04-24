#CMakeLists file for the root (TMR) folder
project(TMReader C)

cmake_minimum_required(VERSION 2.8)
#set(CMAKE_C_FLAGS "-DDEBUG -Wall -g")
set(CMAKE_C_FLAGS "-Wall -g")
add_subdirectory(src)
link_directories(${TMR_BINARY_DIR}/src/ConfigUtils ${TMR_BINARY_DIR}/src/Socket_Comms ${TMR_BINARY_DIR}/src/AggLists)
add_executable(TMReader src/TMReader.c)
target_link_libraries(TMReader mercuryapi ltkc ltkctm CONFIGUTILS SOCKETS)
