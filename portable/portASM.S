
#if __riscv_xlen == 64
	#define portWORD_SIZE 8
	#define store_x sd
	#define load_x ld
#elif __riscv_xlen == 32
	#define store_x sw
	#define load_x lw
	#define portWORD_SIZE 4
#else
	#error Assembler did not define __riscv_xlen
#endif

#define portCONTEXT_SIZE ( 30 * portWORD_SIZE )

.global ez_start_high_rdy
.global ezos_risc_v_trap_handler
.extern ez_tcb_curptr
.extern ez_tcb_hdyptr
.extern synchronous_exception
.extern software_handler
.extern external_handler
.extern yet_unhandler
.extern ez_time_tick
.extern mtimeptr
.extern mtimecmptr
.extern ez_onetick

.align 8
.func
ez_start_high_rdy:

	la t0, ezos_risc_v_trap_handler
	csrw mtvec, t0

	/* clear sp */
	addi sp,x0,0

	/* set MIE */
	csrr x5, mstatus
	addi x5, x5, 0x08
	csrw mstatus, x5

	/* enable timer interrupt */
	csrr x5, mie
	addi x5, x5, 0x80
	csrw mie, x5

	j ezos_risc_v_trap_handler

ez_hang:
	/* never here */
	j  ez_hang
	.endfunc

.align 8
.func
ezos_risc_v_trap_handler:
	/* if sp is zero, exec first task */
	beq  sp, x0, processed_source

	addi sp, sp, -portCONTEXT_SIZE
	store_x x1, 1 * portWORD_SIZE( sp )
	store_x x5, 2 * portWORD_SIZE( sp )
	store_x x6, 3 * portWORD_SIZE( sp )
	store_x x7, 4 * portWORD_SIZE( sp )
	store_x x8, 5 * portWORD_SIZE( sp )
	store_x x9, 6 * portWORD_SIZE( sp )
	store_x x10, 7 * portWORD_SIZE( sp )
	store_x x11, 8 * portWORD_SIZE( sp )
	store_x x12, 9 * portWORD_SIZE( sp )
	store_x x13, 10 * portWORD_SIZE( sp )
	store_x x14, 11 * portWORD_SIZE( sp )
	store_x x15, 12 * portWORD_SIZE( sp )
	store_x x16, 13 * portWORD_SIZE( sp )
	store_x x17, 14 * portWORD_SIZE( sp )
	store_x x18, 15 * portWORD_SIZE( sp )
	store_x x19, 16 * portWORD_SIZE( sp )
	store_x x20, 17 * portWORD_SIZE( sp )
	store_x x21, 18 * portWORD_SIZE( sp )
	store_x x22, 19 * portWORD_SIZE( sp )
	store_x x23, 20 * portWORD_SIZE( sp )
	store_x x24, 21 * portWORD_SIZE( sp )
	store_x x25, 22 * portWORD_SIZE( sp )
	store_x x26, 23 * portWORD_SIZE( sp )
	store_x x27, 24 * portWORD_SIZE( sp )
	store_x x28, 25 * portWORD_SIZE( sp )
	store_x x29, 26 * portWORD_SIZE( sp )
	store_x x30, 27 * portWORD_SIZE( sp )
	store_x x31, 28 * portWORD_SIZE( sp )

	csrr t0, mstatus					/* Required for MPIE bit. */
	store_x t0, 29 * portWORD_SIZE( sp )

	load_x  t0, ez_tcb_curptr			/* Load ez_tcb_curptr. */
	store_x  sp, 0( t0 )				/* Write sp to first TCB member. */

	csrr a0, mcause
	csrr a1, mepc

test_if_asynchronous:
	srli a2, a0, __riscv_xlen - 1		/* MSB of mcause is 1 if handing an asynchronous interrupt - shift to LSB to clear other bits. */
	beq a2, x0, handle_synchronous		/* Branch past interrupt handing if not asynchronous. */
	store_x a1, 0( sp )

handle_asynchronous:
    test_if_soft_interrupt:
		addi t0, x0, 1
		slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
		addi t1, t0, 3					/* 0x8000[]0003 == machine soft interrupt. */
        bne a0, t1, test_if_mtimer

		/* clear sw interrupt */
		addi x5, x0, 0
		li x10, 0x02000000
		store_x x5, 0(x10)
	    jal software_handler

	test_if_mtimer:
		addi t0, x0, 1

		slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
		addi t1, t0, 7					/* 0x8000[]0007 == machine timer interrupt. */
		bne a0, t1, test_if_external_interrupt

		/* add time tick code here */
		jal ez_time_tick

		load_x t0, mtimecmptr
		load_x t1, mtimeptr
		lw t2, 0(t1)
		lw t3, 4(t1)
		lw t5, ez_onetick
		add t4, t5, t2
		sltu t5, t4, t2
		add t6, t3, t5
		sw t4, 0(t0)
		sw t6, 4(t0)

		j processed_source

	test_if_external_interrupt:			/* If there is a CLINT and the mtimer interrupt is not pending then check to see if an external interrupt is pending. */
		addi t1, t1, 4					/* 0x80000007 + 4 = 0x8000000b == Machine external interrupt. */
		bne a0, t1, as_yet_unhandled	/* Something as yet unhandled. */
		jal external_handler

	as_yet_unhandled:
		jal yet_unhandler

handle_synchronous:
	jal synchronous_exception

processed_source:
	la  t1, ez_tcb_curptr
	load_x  t2, ez_tcb_hdyptr
	store_x t2, 0(t1)
	load_x  sp, 0(t2)				 	/* Read sp from first TCB member. */

	/* Load mret with the address of the next instruction in the task to run next. */
	load_x t0, 0( sp )
	csrw mepc, t0

	/* Load mstatus with the interrupt enable bits used by the task. */
	load_x  t0, 29 * portWORD_SIZE( sp )
	csrw mstatus, t0						/* Required for MPIE bit. */

	load_x  x1, 1 * portWORD_SIZE( sp )
	load_x  x5, 2 * portWORD_SIZE( sp )		/* t0 */
	load_x  x6, 3 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 4 * portWORD_SIZE( sp )		/* t2 */
	load_x  x8, 5 * portWORD_SIZE( sp )		/* s0/fp */
	load_x  x9, 6 * portWORD_SIZE( sp )		/* s1 */
	load_x  x10, 7 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 8 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 9 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 10 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 11 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 12 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 13 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 14 * portWORD_SIZE( sp )	/* a7 */
	load_x  x18, 15 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 16 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 17 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 18 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 19 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 20 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 21 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 22 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 23 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 24 * portWORD_SIZE( sp )	/* s11 */
	load_x  x28, 25 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 26 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 27 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 28 * portWORD_SIZE( sp )	/* t6 */
	addi sp, sp, portCONTEXT_SIZE

	mret
	.endfunc

