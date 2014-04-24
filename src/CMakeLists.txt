#CMakeLists for SRC folder
file( GLOB SRCS *.c *.h)
add_subdirectory(Socket_Comms)
add_subdirectory(ConfigUtils)
add_subdirectory(AggLists)

include_directories(Socket_Comms ConfigUtils AggLists)

#link_directories(${TMR_BINARY_DIR}/ConfigUtils ${TMR_BINARY_DIR}/Socket_Comms)
#link_directories(ConfigUtils Socket_Comms)
#add_executable(TMReader ../src/TMReader.c)

#target_link_libraries(TMReader mercuryapi ltkc ltkctm CONFIGUTILS SOCKETS)

#install (TARGETS TMReader DESTINATION bin)
#install (FILES configUtils.h socket_comms.h DESTINATION include)

