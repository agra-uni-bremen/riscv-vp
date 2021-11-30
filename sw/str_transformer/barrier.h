/**
 * @file barrier.h
 * @author cw (wei.c.chen@intel.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * Copyright (C) 2020, Intel Corporation. All rights reserved.
 * 
 */

#pragma once

#define nop()		asm volatile ("nop")

#define RISCV_FENCE(p, s) \
	asm volatile ("fence " #p "," #s : : : "memory")

/* These barriers need to enforce ordering on both devices or memory. */
#define mb()		RISCV_FENCE(iorw,iorw)
#define rmb()		RISCV_FENCE(ir,ir)
#define wmb()		RISCV_FENCE(ow,ow)

/* These barriers do not need to enforce ordering on devices, just memory. */
#define __smp_mb()	RISCV_FENCE(rw,rw)
#define __smp_rmb()	RISCV_FENCE(r,r)
#define __smp_wmb()	RISCV_FENCE(w,w)
