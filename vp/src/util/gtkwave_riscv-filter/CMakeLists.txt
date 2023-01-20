cmake_minimum_required(VERSION 3.18)
project(GTKWAVE_RISCV_INSTR_FILTER CXX)

SET(INSTR ${CMAKE_CURRENT_SOURCE_DIR}/../../core/common)

add_executable(gtkwave_riscv-filter
	riscv-filter.cpp
	${INSTR}/instr.cpp
)
target_include_directories(gtkwave_riscv-filter PRIVATE
	${INSTR}
	${CMAKE_CURRENT_SOURCE_DIR}/../..
)
target_compile_features(gtkwave_riscv-filter PRIVATE cxx_std_20)