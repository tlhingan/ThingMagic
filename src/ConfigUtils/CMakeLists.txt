#CMakeLists file for ConfigUtils Library
file( GLOB SRCS *.c *.h )

project(libConfigUtils)

add_library(CONFIGUTILS SHARED ${SRCS})

install(TARGETS CONFIGUTILS DESTINATION lib)
install(FILES configUtils.h DESTINATION include)

