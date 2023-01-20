cmake_minimum_required(VERSION 3.18)
project(riscv-vp
	VERSION 1.2.2
)

option(USE_SYSTEM_SYSTEMC "use systemc version provided by the system" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")        #"-fsanitize=address -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Allows running tests without invoking `make install` first.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(cmake/AddGitSubmodule.cmake)

find_package( Boost REQUIRED COMPONENTS iostreams program_options log)

if(NOT USE_SYSTEM_SYSTEMC)
	find_package( SystemC REQUIRED)
	include_directories( ${SystemC_INCLUDE_DIRS})
else()
	find_library(SystemC_LIBRARIES libsystemc.a)
endif()

find_package( SoftFloat REQUIRED )
include_directories( ${SoftFloat_INCLUDE_DIRS} )

subdirs(src)

enable_testing()
list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")

add_test(NAME libgdb
	COMMAND ./test.sh
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/libgdb")
add_test(NAME gdb
	COMMAND ./test.sh
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/gdb")
add_test(NAME integration
	COMMAND ./test.sh
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/integration")
add_test(NAME sw
	COMMAND ./test.sh
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../sw")

set_tests_properties(gdb integration sw PROPERTIES ENVIRONMENT
	PATH=$ENV{PATH}:${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set_tests_properties(libgdb PROPERTIES ENVIRONMENT
	RISCV_VP_BASE=${CMAKE_CURRENT_SOURCE_DIR}/..)
