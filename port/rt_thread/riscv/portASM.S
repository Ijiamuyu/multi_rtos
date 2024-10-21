
    .align 4
    .globl rt_set_vector
    .extern SW_handler
rt_set_vector:
    la t0, SW_handler
    csrw mtvec, t0
    ret
