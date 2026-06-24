VP = /home/sayak/Documents/Workstudent_SW/NoTagRoot/riscv-vp/vp/build/bin/riscv-vp
OBJECTS  = main.o cheri.o bootstrap.o
CFLAGS   = -march=rv32i_ucheri -mabi=ilp32
LDFLAGS  = -nostartfiles -Wl,--no-relax
VP_FLAGS = --intercept-syscalls --error-on-zero-traphandler=true --trace

#ifdef DEBUG
DEBUGFLAG = -ggdb
#else
DEBUGFLAG = 
#endif

include ../../Makefile.common
