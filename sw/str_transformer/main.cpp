// The following is a RISC-V program to test the functionality of the
// dummy RoCC accelerator.
// Compile with riscv64-unknown-elf-gcc dummy_rocc_test.c
// Run with spike --extension=dummy_rocc pk a.out
// asm format: 
//    .insn r opcode, func3, func7, rd, rs1, rs2
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static int buffer_index = 0;
static const int num_buffers = 2;
static const int idx_intv = 10;        // reg index interval between buffers
static const int src_local_idx = 0;   // src_addr
static const int dst_local_idx = 1;   // dst_addr
static const int size_local_idx = 2;  // str_size
static const int func_local_idx = 3;  // buffer_func
static const int lower_func = 0;
static const int upper_func = 1;

int get_buffer_index() {
  buffer_index = (buffer_index++) % num_buffers;
  return buffer_index;
}

void do_transform(char* buf, int len, int func) {
  int buffer_idx = get_buffer_index();
  // 1. configure source address
  int src_reg_idx = buffer_idx * idx_intv + src_local_idx;
  asm volatile(".insn r 0x0b, 3, 0, x0, %0, %1" : : "r"(buf), "r"(src_reg_idx));

  // 2. configure dst address
  int dst_reg_idx = buffer_idx * idx_intv + dst_local_idx;
  asm volatile(".insn r 0x0b, 3, 0, x0, %0, %1" : : "r"(buf), "r"(dst_reg_idx));

  // 3. configure data length
  int size_reg_idx = buffer_idx * idx_intv + size_local_idx;
  asm volatile(".insn r 0x0b, 3, 0, x0, %0, %1" : : "r"(len), "r"(size_reg_idx));

  // 4. configure buffer function
  int func_reg_idx = buffer_idx * idx_intv + func_local_idx;
  asm volatile(".insn r 0x0b, 3, 0, x0, %0, %1" : : "r"(func), "r"(func_reg_idx));

  // 5. Kick off the buffer function
  asm volatile(".insn r 0x0b, 1, 1, x0, x0, %0" : : "r"(buffer_idx));

  // 6. fence on mem write
  // asm volatile("fence iorw, iorw" : : : "memory");
}

// change chars in buf to lower case in place
void to_lower_case(char* buf, int len) {
  do_transform(buf, len, lower_func);
}

void to_upper_case(char* buf, int len) {
  do_transform(buf, len, upper_func);
}

int main() {
  char lower[] = "hello";
  char upper[] = "WORLD";
  char mixed[] = "risc-V";

  to_upper_case(lower, sizeof(lower));
  assert(string(lower) == "HELLO");

  to_lower_case(upper, sizeof(upper));
  assert(string(upper) == "world");

  to_lower_case(mixed, sizeof(mixed));
  assert(string(mixed) == "risc-v");

  to_upper_case(mixed, sizeof(mixed));
  assert(string(mixed) == "RISC-V");

	printf("success!\n");
}
