RISCV_PREFIX ?= $(HOME)/cheri/output/sdk/bin/
ARCH    ?= rv32ixcheri
ABI     ?= il32pc64d
SYSROOT ?= $(HOME)/cheri/output/sdk/baremetal/baremetal-newlib-riscv32-purecap/riscv32-unknown-elf
BUILTINS ?= $(HOME)/cheri/output/sdk/baremetal/baremetal-riscv32-purecap/lib/libclang_rt.builtins-riscv32.a
VP = ../../../vp/build_TESTRIG/bin/riscv-vp

#VP-Flags
GCT_PREFIX ?= riscv32-unknown-elf-
VP ?= riscv-vp
VP_FLAGS ?= --intercept-syscalls --error-on-zero-traphandler=true --trace
ASFLAGS ?= $(CFLAGS)
SIM_TARGET ?= sim-default

override CC  = $(RISCV_PREFIX)clang
override LD  = $(RISCV_PREFIX)clang
override CXX = $(RISCV_PREFIX)clang++

CFLAGS  ?= -target riscv32-unknown-elf \
            -march=$(ARCH) \
            -mabi=$(ABI) \
            --sysroot=$(SYSROOT) \
             -fno-inline \
            -O2 -Wcheri -Wall -Wextra

LDFLAGS ?= -fuse-ld=lld \
           -rtlib=compiler-rt \
           -L$(SYSROOT)/lib \
            -lc -lm -lg \
            $(BUILTINS) \
            -Wl,-e,_start\
            -nodefaultlibs\
            -T link.ld \
            -Wl,--local-caprelocs=legacy\
            -static

# Only object files are generated; source files stay untouched
SRCS := $(wildcard *.c *.S)
OBJS := $(SRCS:.c=.o)

TARGET ?= hello
PREBUILT ?= hello

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

objdump:
	@echo "=== Disassembly of .text section ==="
	$(RISCV_PREFIX)objdump -D $(TARGET) > dump.txt

###############################################################
sim: $(SIM_TARGET)
sim-default: $(TARGET) 
	$(VP) $(VP_FLAGS) $<

sim-prebuilt:
	$(VP) $(VP_FLAGS) $(PREBUILT)
  
dump-elf: $(TARGET) 
	$(GCT_PREFIX)readelf -a main
  
dump-code: $(TARGET) 
	$(GCT_PREFIX)objdump -D main
###############################################################


# Clean only deletes object files and binary, never source files
clean:
	rm -f *.o $(TARGET) $(CLEAN_EXTRA)

.PHONY: all clean objdump 
