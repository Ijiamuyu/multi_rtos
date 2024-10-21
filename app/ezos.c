#include <stdint.h>
#include "ez.h"
#include "console.h"


#define TASK1_STK_SIZE 128
#define TASK2_STK_SIZE 128

static ez_stk_t task1_stk[TASK1_STK_SIZE];
static ez_stk_t task2_stk[TASK2_STK_SIZE];

static EzTcb task1_tcb;
static EzTcb task2_tcb;

volatile uint32_t task1_cnt, task2_cnt, task1_flag = 1, task2_flag = 1;

void task1(void *p_arg) {
    while (1) {
        if (task1_flag) {
            task1_flag = 0;
            task2_flag = 1;
            task1_cnt++;
            console_printf("hello task1 %d\n", task1_cnt);
        }
    }
}

void task2(void *p_arg) {
    while (1) {
        if (task2_flag) {
            task2_flag = 0;
            task1_flag = 1;
            task2_cnt++;
            console_printf("hello task2 %d\n", task2_cnt);
        }
    }
}

int main(void) {
    ez_err err;

    console_printf("\nBuild in %s(%s)\n", __DATE__,__TIME__);
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
