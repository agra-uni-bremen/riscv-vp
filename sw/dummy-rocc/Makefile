all : dummy_rocc_test2.c 
	riscv32-unknown-elf-g++ -std=c++14 dummy_rocc_test2.c -o main -march=rv32imafd -mabi=ilp32d
	
sim: all
	riscv-vp --intercept-syscalls main
	
dump-elf: all
	riscv32-unknown-elf-readelf -a main
	
dump-code: all
	riscv32-unknown-elf-objdump -D main
	
clean:
	rm -f main
