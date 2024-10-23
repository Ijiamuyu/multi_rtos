#include <rthw.h>
#include <rtthread.h>
#include "cpu.h"

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

