set(SOURCES cc1101_hal.c
			cc1101.c)

idf_component_register(SRCS "${SOURCES}"
					INCLUDE_DIRS .
					REQUIRES 	foreach 
								spi_master_ext)
