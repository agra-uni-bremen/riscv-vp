cmake_minimum_required(VERSION 3.18)
project(riscv-vp
	VERSION 1.2.2
)

option(USE_SYSTEM_SYSTEMC "use systemc version provided by the system" OFF)
#spandan: cmake -DUSE_PURE_CAPABILITY=ON ..
option(USE_PURE_CAPABILITY "use only capabilities instead of pointers" OFF)
#spandan: cmake -DUSE_TESTRIG=ON ..
option(USE_TESTRIG "generate instructions randomly through TestRIG" OFF)
#sayak : cmake -DUSE_NOTAGROOT=ON ..
option(USE_NOTAGROOT "Enable NOTAGROOT system" OFF)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wpedantic")
# spandan--------------------------------------------------------------------
if (USE_PURE_CAPABILITY)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPURE")
	message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
endif()

if (USE_TESTRIG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPURE -DNOTAGROOT -DTESTRIG")
	message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
endif()

# sayak--------------------------------------------------------------------
if (USE_NOTAGROOT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPURE -DNOTAGROOT")  #I want PURE should get activate when NOTAGROOT activates 
    message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
endif()
# sayak--------------------------------------------------------------------

# spandan--------------------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "-g3")        #"-fsanitize=address -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Allows running tests without invoking `make install` first.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

include(cmake/AddGitSubmodule.cmake)

find_package( Boost REQUIRED COMPONENTS iostreams program_options log)

if(USE_SYSTEM_SYSTEMC)
	find_library(SystemC libsystemc.a)
endif()

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
