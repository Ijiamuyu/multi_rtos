/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Initialize                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

    .section .data
    .global  __tx_free_memory_start
__tx_free_memory_start:


    .section .text
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_initialize_low_level                           RISC-V64/GNU     */
/*                                                           6.2.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is responsible for any low-level processor            */
/*    initialization, including setting up interrupt vectors, setting     */
/*    up a periodic timer interrupt source, saving the system stack       */
/*    pointer for use in ISR processing later, and finding the first      */
/*    available RAM memory address for tx_application_define.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _tx_initialize_kernel_enter           ThreadX entry function        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  03-08-2023      Scott Larson            Initial Version 6.2.1         */
/*                                                                        */
/**************************************************************************/

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

.global risc_v_trap_handler
.extern synchronous_exception
.extern software_handler
.extern external_handler
.extern yet_unhandler
.extern threadx_setup_timer_interrupt
.extern mtimeptr
.extern mtimecmptr
.extern threadx_onetick

    .global  _tx_initialize_low_level
_tx_initialize_low_level:
    sw      sp, _tx_thread_system_stack_ptr, t0     // Save system stack pointer

	addi sp, sp, -4
    la      t0, __tx_free_memory_start              // Pickup first free address
    sw      t0, _tx_initialize_unused_memory, t1    // Save unused memory address

	la t0, risc_v_trap_handler
	csrw mtvec, t0

	sw x1, 0(sp)
	call threadx_setup_timer_interrupt
	lw x1, 0(sp)
	addi sp, sp, 4

	/* set MIE */
	csrr x5, mstatus
	addi x5, x5, 0x08
	csrw mstatus, x5

	/* enable timer interrupt */
	csrr x5, mie
	addi x5, x5, 0x80
	csrw mie, x5

#ifdef __riscv_flen
    fscsr x0
#endif

    ret


.align 8
.func
risc_v_trap_handler:
	/* for threadx thread save, if use another handler, may change it */
	addi sp, sp, -128

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

		load_x t0, mtimecmptr
		load_x t1, mtimeptr
		lw t2, 0(t1)
		lw t3, 4(t1)
		lw t5, threadx_onetick
		add t4, t5, t2
		sltu t5, t4, t2
		add t6, t3, t5
		sw t4, 0(t0)
		sw t6, 4(t0)

		call _tx_thread_context_save
		/* add time tick code here */
		call _tx_timer_interrupt
		j _tx_thread_context_restore

	test_if_external_interrupt:			/* If there is a CLINT and the mtimer interrupt is not pending then check to see if an external interrupt is pending. */
		addi t1, t1, 4					/* 0x80000007 + 4 = 0x8000000b == Machine external interrupt. */
		bne a0, t1, as_yet_unhandled	/* Something as yet unhandled. */
		jal external_handler

	as_yet_unhandled:
		jal yet_unhandler

handle_synchronous:
	jal synchronous_exception


