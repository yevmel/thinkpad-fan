cmake_minimum_required (VERSION 2.8)
project (thinkpad-fan) 

set (SOURCES
	sources/main.c
)

add_executable (${PROJECT_NAME} ${SOURCES})

install (TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
install (FILES "${PROJECT_SOURCE_DIR}/resources/etc/modprobe.d/thinkpad_acpi.conf" DESTINATION /etc/modprobe.d/)
install (FILES "${PROJECT_SOURCE_DIR}/resources/etc/systemd/system/thinkpad-fan.service" DESTINATION /etc/systemd/system)
