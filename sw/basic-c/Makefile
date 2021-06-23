all: main

main: main.c sum.c bootstrap.S
	riscv32-unknown-elf-gcc main.c sum.c bootstrap.S -o main -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim: main
	riscv-vp --intercept-syscalls main

dump-elf: main
	riscv32-unknown-elf-readelf -a main

dump-code: main
	riscv32-unknown-elf-objdump -D main

dump-comment: main
	objdump -s --section .comment main

clean:
	rm -f main
