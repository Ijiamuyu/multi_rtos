#include "cpuport.h"
    .align 4
    .globl rt_setup_vector
    .global rt_mtimer_enable
    .extern SW_handler
    .globl rt_hw_do_after_save_above
    .type rt_hw_do_after_save_above,@function
rt_hw_do_after_save_above:
    addi  sp, sp,  -4
    STORE ra,  0 * REGBYTES(sp)

    csrr  a0, mcause
    csrr  a1, mepc
    mv    a2, sp
    call  rt_rv32_system_irq_handler

    LOAD  ra,  0 * REGBYTES(sp)
    addi  sp, sp,  4
    ret
rt_setup_vector:
    la t0, SW_handler
    csrw mtvec, t0
    ret
rt_mtimer_enable:
    /* enable timer interrupt */
	csrr t0, mie
	addi t0, t0, 0x80
	csrw mie, t0
    ret
