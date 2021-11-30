/**
 * @file driver.h
 * @author cw (wei.c.chen@intel.com)
 * @brief
 * @version 0.1
 * @date 2021-11-30
 *
 * Copyright (C) 2020, Intel Corporation. All rights reserved.
 *
 */

#pragma once

#include <assert.h>
#include "barrier.h"

#define NUM_BUFFERS 2  // number of buffers

#define REG_IDX_FACTOR 10  // reg index interval between buffers

#define REG_OFFSET_SRC 0   // reg index offset of src_addr
#define REG_OFFSET_DST 1   // reg index offset of dst_addr
#define REG_OFFSET_LEN 2   // reg index offset of str_size
#define REG_OFFSET_FUNC 3  // reg index offset of buffer_func

#define LOWER_FUNC 0
#define UPPER_FUNC 1

#define config_reg(reg, val) asm volatile(".insn r 0x0b, 3, 0, x0, %0, %1" : : "r"(val), "r"(reg))

#define kick_off(pe_idx) asm volatile(".insn r 0x0b, 1, 1, x0, x0, %0" : : "r"(pe_idx))

static int buffer_index = 0;

int get_buffer_index() {
	buffer_index = (buffer_index++) % NUM_BUFFERS;
	return buffer_index;
}

void do_transform(char* src, char* dst, size_t len, int func) {
    if (len == 0) return;
    assert(src != nullptr);
    assert(dst != nullptr);
	assert(func == LOWER_FUNC || func == UPPER_FUNC);
	
	int buffer_idx = get_buffer_index();
	// 1. configure source address
	int src_reg_idx = buffer_idx * REG_IDX_FACTOR + REG_OFFSET_SRC;
	config_reg(src_reg_idx, src);

	// 2. configure dst address
	int dst_reg_idx = buffer_idx * REG_IDX_FACTOR + REG_OFFSET_DST;
	config_reg(dst_reg_idx, dst);

	// 3. configure data length
	int len_reg_idx = buffer_idx * REG_IDX_FACTOR + REG_OFFSET_LEN;
	config_reg(len_reg_idx, len);

	// 4. configure buffer function
	int func_reg_idx = buffer_idx * REG_IDX_FACTOR + REG_OFFSET_FUNC;
	config_reg(func_reg_idx, len);

	// 5. Kick off the buffer function
	kick_off(buffer_idx);

	// 6. barrier on mem write
	mb();
}

// change chars in buf to lower case in place
void to_lower_case(char* buf, size_t len) {
	do_transform(buf, buf, len, LOWER_FUNC);
}

// change chars in buf to upper case in place
void to_upper_case(char* buf, size_t len) {
	do_transform(buf, buf, len, UPPER_FUNC);
}

// change chars in src buffer to lower case and copy to dst buffer
void to_lower_case(char* src, char* dst, size_t len) {
	do_transform(src, dst, len, LOWER_FUNC);
}

// change chars in src buffer to upper case and copy to dst buffer
void to_upper_case(char* src, char* dst, size_t len) {
	do_transform(src, dst, len, UPPER_FUNC);
}