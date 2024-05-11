#include "ez.h"
#include <stdint.h>

#define TASK1_STK_SIZE 128
#define TASK2_STK_SIZE 128

static ez_stk_t task1_stk[TASK1_STK_SIZE];
static ez_stk_t task2_stk[TASK2_STK_SIZE];

static EzTcb task1_tcb;
static EzTcb task2_tcb;

volatile uint32_t task1_cnt, task2_cnt;

void task1(void *p_arg) {
    while (1) {
        task1_cnt++;
        uart_printf("hello task1 %d\n", task1_cnt);
        ez_schedule();
    }
}

void task2(void *p_arg) {
    while (1) {
        task2_cnt++;
        uart_printf("hello task2 %d\n", task2_cnt);
        ez_schedule();
    }
}

int main(void) {
    ez_err err;
    int i;
    for(i=0;i<10;i++)
        uart_printf("Hello App %d\n",i);

    ez_init(&err);
    /* 创建任务 */
    ez_task_creat(
            (EzTcb *)&task1_tcb,
            (EzTaskPtr)task1,
            (void *)0,
            (ez_stk_t *)&task1_stk[0],
            (ez_size_t)TASK1_STK_SIZE,
            (ez_err *)&err);

    ez_task_creat(
            (EzTcb *)&task2_tcb,
            (EzTaskPtr)task2,
            (void *)0,
            (ez_stk_t *)&task2_stk[0],
            (ez_size_t)TASK2_STK_SIZE,
            (ez_err *)&err);

    /* 将任务加入到就绪列表 */
    ez_rdylist[0].head = &task1_tcb;
    ez_rdylist[1].head = &task2_tcb;

    ez_start(&err);

    while (1)
        ;
    return 0;
}
