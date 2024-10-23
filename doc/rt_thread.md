# overall
本文主要介绍rt_thread移植要点

# trace
追踪riscv平台的rt-thread代码流程，采用向量模式，需要准备rt_hw_interrupt_disable，rt_hw_board_init等操作，除此以外还有rt_hw_do_after_save_above，rt_trigger_software_interrupt（非向量模式不需要），准备一个systick
```c
main
    -->|rt_post_start_scheduler
        -->|rt_hw_interrupt_disable //port
            -->|soc_set_interrupt_threshold
        -->|rt_hw_board_init //port
            -->|uart_driver_init
            -->|hw_timer_init
            -->|rt_system_heap_init
        -->|rt_show_version
        -->|rt_system_timer_init
        -->|rt_system_scheduler_init
        -->|rt_system_timer_thread_init
            -->|......
            -->|rt_thread_init
            -->|rt_thread_startup
        -->|rt_thread_idle_init
            -->|rt_thread_init
            -->|rt_thread_startup
    -->|rt_thread_create
    -->|rt_thread_startup
    -->|rt_start_scheduler
        -->|rt_system_scheduler_start
            -->|rt_hw_context_switch_to
```

# riscv-virt平台
采用非向量模式组织异常，异常组织代码如下：
```c
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
```
- rt_hw_do_after_save_above
    - 统一使用rt_rv32_system_irq_handler来管理异常
    - 将参数传入到a0,a1,a2，并保存ra
- rt_setup_vector设置非向量模式
- rt_mtimer_enable开启mie中mtimer

设置mtimer中断
```c
extern void rt_setup_vector(void);
extern void rt_mtimer_enable(void);

static rt_uint64_t *mtimecmptr = (rt_uint64_t *)(CLINT_ADDR + CLINT_MTIMECMP);
static rt_uint64_t rt_onetick = CPU_CLOCK_HZ/RT_TICK_PER_SECOND;

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

static void  rt_timer_update(void)
{
    rt_uint32_t *cur_time_high =
            (rt_uint32_t *)(CLINT_ADDR + CLINT_MTIME + 0x4);
    rt_uint32_t *cur_time_low = (rt_uint32_t *)(CLINT_ADDR + CLINT_MTIME);

    *mtimecmptr = (rt_uint64_t)*cur_time_high;
    *mtimecmptr <<= 32;
    *mtimecmptr |= (rt_uint64_t)*cur_time_low;
    *mtimecmptr += (rt_uint64_t)rt_onetick;
}

rt_isr_handler_t rt_hw_mtimer_handle(rt_uint32_t mcause)
{
    rt_interrupt_enter();
    rt_timer_update();
    rt_tick_increase();
    rt_interrupt_leave();
    return RT_NULL;
}


static void rt_install_interrupt(void)
{
    rt_hw_interrupt_install(0x7, (rt_isr_handler_t)rt_hw_mtimer_handle, RT_NULL,"mtimerirq");
}


void rt_hw_board_init(void)
{
    rt_setup_vector();
    rt_hw_interrupt_init();
    rt_install_interrupt();
    rt_timer_update();
    rt_mtimer_enable();

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void *)rt_heap_begin_get(), (void *)rt_heap_end_get());
#endif
}
```
- rt_hw_board_init
    - 调用rt_setup_vector设置非向量模式的异常
    - rt_hw_interrupt_init初始化异常
    - rt_install_interrupt注册mtimer中断
    - rt_timer_update更新mtimecmp寄存器
    - rt_mtimer_enable使能mtimer，待到全局中断开启后即可接收中断
- rt_hw_mtimer_handle中断服务函数
    - rt_interrupt_enter进入临界区
    - rt_timer_update更新mtimercmp
    - rt_tick_increase通知内核增加tick
    - 离开临界区

mtimer任务切换流程
```c
SW_handler
    -->|保存现场
    -->|rt_interrupt_enter
    -->|rt_hw_do_after_save_above
        -->|rt_rv32_system_irq_handler
            -->|rt_hw_mtimer_handle
                -->|rt_interrupt_enter
                -->|rt_timer_update
                -->|rt_tick_increase
                -->|rt_interrupt_leave
    -->|rt_interrupt_leave
    -->|切换上下文
    -->|恢复任务环境
```
