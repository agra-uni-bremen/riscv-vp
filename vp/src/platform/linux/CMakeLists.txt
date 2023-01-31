file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set(SOURCES
	linux_main.cpp
	${HEADERS})
set(LIBS platform-common gdb-mc
	${Boost_LIBRARIES}
	${SystemC_LIBRARIES}
	pthread)

# Linux for RV32 with single worker core
add_executable(linux32-sc-vp ${SOURCES})
target_compile_definitions(linux32-sc-vp PUBLIC
	TARGET_RV32
	NUM_CORES=2)
target_link_libraries(linux32-sc-vp rv32 ${LIBS})

# Linux for RV32 with four worker cores
add_executable(linux32-vp ${SOURCES})
target_compile_definitions(linux32-vp PUBLIC
	TARGET_RV32
	NUM_CORES=5)
target_link_libraries(linux32-vp rv32 ${LIBS})

# Linux for RV32 with single worker core
add_executable(linux-sc-vp ${SOURCES})
target_compile_definitions(linux-sc-vp PUBLIC
	TARGET_RV64
	NUM_CORES=2)
target_link_libraries(linux-sc-vp rv64 ${LIBS})

# Linux for RV32 with four worker cores
add_executable(linux-vp ${SOURCES})
target_compile_definitions(linux-vp PUBLIC
	TARGET_RV64
	NUM_CORES=5)
target_link_libraries(linux-vp rv64 ${LIBS})

INSTALL(TARGETS
	linux32-vp
	linux32-sc-vp
	linux-vp
	linux-sc-vp
	RUNTIME DESTINATION bin)
