override CC = riscv32-unknown-elf-gcc

CFLAGS  = -ggdb -march=rv32i -mabi=ilp32
ASFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles -Wl,--no-relax -march=rv32i -mabi=ilp32

all: main
main: main.o irq.o bootstrap.o
sim: main
	tiny32-vp --intercept-syscalls $<

clean:
	rm -f *.o
	rm -f main

.PHONY: all sim clean
