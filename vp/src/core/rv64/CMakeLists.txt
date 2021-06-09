file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(rv64
		iss.cpp
		syscall.cpp
        ${HEADERS})

target_compile_definitions(rv64 PUBLIC -DRV64)
target_link_libraries(rv64 core-common ${SoftFloat_LIBRARIES})


if(COLOR_THEME STREQUAL "LIGHT")
	message("> using color theme LIGHT")
	target_compile_definitions(rv64 PRIVATE COLOR_THEME_LIGHT)
elseif(COLOR_THEME STREQUAL "DARK")
	message("> using color theme DARK")
	target_compile_definitions(rv64 PRIVATE COLOR_THEME_DARK)
endif()


target_include_directories(rv64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
