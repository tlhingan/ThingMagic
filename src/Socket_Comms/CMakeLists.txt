# CMakeLists file for Socket Comms Library
file( GLOB SRCS *.c *.h )

add_library(SOCKETS SHARED ${SRCS} )
install(TARGETS SOCKETS DESTINATION bin)
install(FILES socket_comms.h DESTINATION include)
