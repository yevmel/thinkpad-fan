cmake_minimum_required (VERSION 2.8)
project (thinkpad-fan) 

option (VERBOSE "print additional information to STDOUT" OFF)

configure_file (sources/config.h.in ${CMAKE_BINARY_DIR}/sources/config.h)
include_directories (${CMAKE_BINARY_DIR}/sources/)

set (SOURCES
	sources/main.c
)

add_executable (${PROJECT_NAME} ${SOURCES})

install (TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
install (FILES "${PROJECT_SOURCE_DIR}/resources/etc/modprobe.d/thinkpad_acpi.conf" DESTINATION /etc/modprobe.d/)
install (FILES "${PROJECT_SOURCE_DIR}/resources/etc/systemd/system/thinkpad-fan.service" DESTINATION /etc/systemd/system)
