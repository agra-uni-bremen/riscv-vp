file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(qemu32-vp
        ${HEADERS}
        qemu32_main.cpp)

target_link_libraries(qemu32-vp rv32 platform-common gdb-mc ${Boost_LIBRARIES} systemc pthread)

INSTALL(TARGETS qemu32-vp RUNTIME DESTINATION bin)
