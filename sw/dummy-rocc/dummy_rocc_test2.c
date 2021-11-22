// The following is a RISC-V program to test the functionality of the
// dummy RoCC accelerator.
// Compile with riscv64-unknown-elf-gcc dummy_rocc_test.c
// Run with spike --extension=dummy_rocc pk a.out

 #include <assert.h>
 #include <stdio.h>
#include <stdint.h>

int main() {
  uint32_t x = 123, y = 456, z = 0;
  // load x into accumulator 2 (funct=0) 
  // acc <- xs1
  asm volatile (".insn r 0x0b, 3, 0, x0, %0, x2" : : "r"(x));

  // read it back into z (funct=1) to verify it
  // xd <- acc
  asm volatile (".insn r 0x0b, 5, 1, %0, x0, x2" : "=r"(z) :);
  asm volatile ("fence iorw, iorw" : : : "memory");
  assert(z == x);

  // accumulate 456 into it (funct=3)
  // acc[rs2] <- accX + xs1
  asm volatile (".insn r 0x0b, 3, 3, x0, %0, x2" : : "r"(y));

  // verify it
  asm volatile (".insn r 0x0b, 5, 1, %0, x0, x2" : "=r"(z) :);
  asm volatile ("fence iorw, iorw" : : : "memory");
  assert(z == x+y);

  // do it all again, but initialize acc2 via memory this time (funct=2)
  asm volatile (".insn r 0x0b, 3, 2, x0, %0, x2" : : "r"(&x));
  asm volatile (".insn r 0x0b, 3, 3, x0, %0, x2" : : "r"(y));
  asm volatile (".insn r 0x0b, 5, 1, %0, x0, x2" : "=r"(z) :);
  asm volatile ("fence iorw, iorw" : : : "memory");
  assert(z == x+y);

  printf("success!\n");
}
