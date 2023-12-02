cmake_minimum_required(VERSION 3.10)
set(THIS next-berryfrog)

include_directories(include)

set(SOURCE_FILES
	source/daemon.cpp
	source/main.cpp
    rc-switch/RCSwitch.cpp
)

set (CMAKE_CXX_STANDARD 17)

# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -Wall -Wextra -pedantic -O0")
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -Wall -Wextra -pedantic -O3")
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)

add_executable(${THIS} ${SOURCE_FILES})

target_include_directories(next-berryfrog
    PRIVATE ${wiringPi_INCLUDE_DIRS}
    PRIVATE ${libconfig++_INCLUDE_DIRS}
    PRIVATE ${MySQL_INCLUDE_DIRS}
)

target_link_libraries(next-berryfrog
    PRIVATE wiringPi
    PRIVATE libconfig++.a
    PRIVATE ${MySQL_LIBRARIES}
)

# Set compiler options
# -DRPI Flag for Raspberry systems
target_compile_options(next-berryfrog PRIVATE -std=c++11 -DRPI)

# Install the config file
install(FILES resource/config/next-berryfrog.conf DESTINATION /etc/next-berryfrog)

# Intall the systemd file
install(FILES resource/systemd/next-berryfrog.service DESTINATION /etc/systemd/system)

# Install the binary program
install(TARGETS ${THIS} DESTINATION /usr/bin)