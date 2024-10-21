
#include <rthw.h>
#include "rtscheduler.h"

#ifdef RT_USING_USER_MAIN

void rt_hw_board_init(void);

void rt_post_start_scheduler(void)
{
    rt_hw_interrupt_disable();
    rt_hw_board_init();
    rt_show_version();
    rt_system_timer_init();
    rt_system_scheduler_init();
    rt_system_timer_thread_init();
    rt_thread_idle_init();
}

void rt_start_scheduler(void)
{
    rt_system_scheduler_start();
}

#endif
