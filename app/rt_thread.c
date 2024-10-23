#include <stdint.h>
#include <stddef.h>
#include "common.h"
#include "console.h"
#include "rtthread.h"
#include "rtscheduler.h"

static rt_thread_t task1_pid;
static rt_thread_t task2_pid;

volatile uint32_t thread1_cnt=0;
volatile uint32_t thread2_cnt=0;

static void task1(void *param)
{

    while (1)
    {
        console_printf("rt-thread 1 application running %d times\r\n",++thread1_cnt);

        rt_thread_delay(100);

    }
}

static void task2(void *param)
{
    while (1)
    {
        console_printf("rt-thread 2 application running %d times\r\n",++thread2_cnt);
        rt_thread_delay(300);
    }
}

int main(void)
{
    console_printf("\nBuild in %s(%s)\n", __DATE__,__TIME__);
    rt_post_start_scheduler();
    console_printf("rt-thread start\n");
    task1_pid = rt_thread_create("task1", task1, NULL, 4096, 4, 32);
    task2_pid = rt_thread_create("task2", task2, NULL, 4096, 5, 32);
    rt_thread_startup(task1_pid);
    rt_thread_startup(task2_pid);
    rt_start_scheduler();
    hang();
    return 0; /* never here */
}
