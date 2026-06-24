.section .text		# Ensure the code goes in the correct section
.globl _start		# Make _start visible to the linker
.p2align 5		# Align to 32 bytes (2^5 = 32)

_start:
	# initialize stack pointer capability (see link.ld)
	cllc	csp, _stack_top

   	#.include "./Boot/MaxTest.S"
   	#.include "./Boot/TestRIG_to_assembly/benchmark_1000_1_new.S"
   	#.include "./Boot/TestRIG_to_assembly/benchmark_1000_5_new.S"
   	#.include "./Boot/TestRIG_to_assembly/benchmark_1000_10_new.S"
   	#.include "./Boot/TestRIG_to_assembly/benchmark_1000_8_new.S"
   	#.include "./Boot/TestRIG_to_assembly/benchmark_1000_15_new.S"
   	.include "./Boot/TestRIG_to_assembly/benchmark_1000_20_new.S"
    
	# call main
	# cllc	ct0, main
	# cjalr	ct0

exit:
	# Call exit (SYS_EXIT=93) with exit code 0 (argument in ca0)
	li		a7, 93		# System call number for exit
	li		a0, 0		# Set exit code (0) in ca0 (a0 in CHERI capability mode)
	ecall				# Make the system call
    
