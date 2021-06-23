all: main printf variable_arguments

main: main.c entry.S
	riscv64-unknown-elf-gcc main.c entry.S -o main -march=rv32imafd -mabi=ilp32d -Wl,-e_reset_vector

sim: main
	riscv-vp main

printf: printf.c entry.S
	riscv64-unknown-elf-gcc printf.c entry.S -o printf -march=rv32ima -mabi=ilp32 -Wl,-e_reset_vector

sim-printf: printf
	riscv-vp --intercept-syscalls printf

variable_arguments: variable_arguments.c entry.S
	riscv64-unknown-elf-gcc variable_arguments.c entry.S -o variable_arguments -march=rv32ima -mabi=ilp32 -Wl,-e_reset_vector

sim-variable_arguments: variable_arguments
	riscv-vp --intercept-syscalls variable_arguments

dump-elf: main
	riscv32-unknown-elf-readelf -a main

dump-code: main
	riscv32-unknown-elf-objdump -D main

clean:
	rm -f main
	rm -f printf
	rm -f variable_arguments
