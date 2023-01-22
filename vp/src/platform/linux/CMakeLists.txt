file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set(SOURCES
	linux_main.cpp
	${HEADERS})
set(LIBS platform-common gdb-mc
	${Boost_LIBRARIES}
	${SystemC_LIBRARIES}
	pthread)

# Linux for RV32
add_executable(linux32-vp ${SOURCES})
target_compile_definitions(linux32-vp PUBLIC TARGET_RV32)
target_link_libraries(linux32-vp rv32 ${LIBS})

# Linux for RV64
add_executable(linux-vp ${SOURCES})
target_compile_definitions(linux-vp PUBLIC TARGET_RV64)
target_link_libraries(linux-vp rv64 ${LIBS})

# install
INSTALL(TARGETS
	linux32-vp
	linux-vp
	RUNTIME DESTINATION bin)
