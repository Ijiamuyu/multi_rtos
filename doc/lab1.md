# 任务切换
实现俩个任务不停切换执行

```c
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
```
如上述代码所示，仿照ucos逻辑，在线程函数中打印各自的变量并切换

## 代码解读

```c
void ez_rdylist_init(void) {
    ez_prio_t i;
    EzRdyList *p_rdy_list;

    for (i = 0u; i < EZ_PRIORITY_MAX; i++) {
        p_rdy_list = &ez_rdylist[i];
        p_rdy_list->head = (EzTcb *)0;
        p_rdy_list->tail = (EzTcb *)0;
    }
}

void ez_init(ez_err *p_err) {
    ez_running = EZ_STATE_STOPPED;

    ez_tcb_curptr = (EzTcb *)0;
    ez_tcb_hdyptr = (EzTcb *)0;

    ez_rdylist_init();

    *p_err = EZ_ERR_NONE;
}
```

- ez_init
  - 将ez_tcb_curptr、ez_tcb_hdyptr指向0
  - 将优先级列表指向0，暂未用到优先级

```c
ez_stk_t *ez_task_init(
        EzTaskPtr p_task,
        void *arg,
        ez_stk_t *p_stk_base,
        ez_size_t stk_size) {
    ez_stk_t *p_stk;

    p_stk = &p_stk_base[stk_size];

    *--p_stk = (ez_stk_t)0x002839a0;  // mstatus
    *--p_stk = (ez_stk_t)0x28282828;  // x31
    *--p_stk = (ez_stk_t)0x27272727;  // x30
    *--p_stk = (ez_stk_t)0x26262626;  // x29
    *--p_stk = (ez_stk_t)0x25252525;  // x28
    *--p_stk = (ez_stk_t)0x24242424;  // x27
    *--p_stk = (ez_stk_t)0x23232323;  // x26
    *--p_stk = (ez_stk_t)0x22222222;  // x25
    *--p_stk = (ez_stk_t)0x21212121;  // x24
    *--p_stk = (ez_stk_t)0x20202020;  // x23
    *--p_stk = (ez_stk_t)0x19191919;  // x22
    *--p_stk = (ez_stk_t)0x18181818;  // x21
    *--p_stk = (ez_stk_t)0x17171717;  // x20
    *--p_stk = (ez_stk_t)0x16161616;  // x19
    *--p_stk = (ez_stk_t)0x15151515;  // x18
    *--p_stk = (ez_stk_t)0x14141414;  // x17
    *--p_stk = (ez_stk_t)0x13131313;  // x16
    *--p_stk = (ez_stk_t)0x12121212;  // x15
    *--p_stk = (ez_stk_t)0x11111111;  // x14
    *--p_stk = (ez_stk_t)0x10101010;  // x13
    *--p_stk = (ez_stk_t)0x09090909;  // x12
    *--p_stk = (ez_stk_t)0x08080808;  // x11
    *--p_stk = (ez_stk_t)arg;         // x10
    *--p_stk = (ez_stk_t)0x06060606;  // x9
    *--p_stk = (ez_stk_t)0x05050505;  // x8
    *--p_stk = (ez_stk_t)0x04040404;  // x7
    *--p_stk = (ez_stk_t)0x03030303;  // x6
    *--p_stk = (ez_stk_t)0x02020202;  // x5
    *--p_stk = (ez_stk_t)0x01010101;  // x1
    *--p_stk = (ez_stk_t)p_task;      //

    return p_stk;
}

void ez_task_creat(
        EzTcb *p_tcb,
        EzTaskPtr p_task,
        void *p_arg,
        ez_stk_t *p_stk_base,
        ez_size_t stk_size,
        ez_err *p_err) {
    ez_stk_t *p_sp;

    p_sp = ez_task_init(p_task, p_arg, p_stk_base, stk_size);
    p_tcb->stk_ptr = p_sp;
    p_tcb->stk_size = stk_size;

    *p_err = EZ_ERR_NONE;
}
```

- ez_task_creat
  - 创建任务，初始化栈
  - ez_task_init
    - mstatus初始化注意mie位，切换任务时会加载到mstatus
    - 联系poarASM脚本，将任务入口地址，arg初始化到合适的位置


```c
void ez_start(ez_err *p_err) {
    if (ez_running == EZ_STATE_STOPPED) {
        ez_tcb_hdyptr = ez_rdylist[0].head;
        ez_start_high_rdy();
        *p_err = EZ_ERR_FATAL_RETURN;
    } else {
        *p_err = EZ_STATE_RUNNING;
    }
}

void ez_schedule(void) {
    if (ez_tcb_curptr == ez_rdylist[0].head) {
        ez_tcb_hdyptr = ez_rdylist[1].head;
    } else {
        ez_tcb_hdyptr = ez_rdylist[0].head;
    }

    ez_task_sw();
}
```
- ez_start
  - 从优先级列表中取出
  - 执行指定的任务
- ez_schedule
  - 轮流切换task1、task2

## 重点解读(portASM)

```asm
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

	/* enable sw interrupt */
	csrr x5, mie
	addi x5, x5, 0x08
	csrw mie, x5

	/* active sw interrupt */
	addi x5, x0, 0x1
	li x10, 0x02000000
	store_x x5,0(x10)

ez_hang:
	/* never here */
	j  ez_hang
	.endfunc
```
- ez_start_high_rdy
  - 执行第一个任务
  - 将ezos_risc_v_trap_handler存入mtvec，指定异常基地址
  - 清0 sp寄存器，后续执行第一个任务不需要存入上文
  - 置位mstatus，使能全局中断
  - 打开mie寄存器的sw中断使能
  - 触发软件中断，clint的msip挂载于0x2000000

```asm
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
        bne a0, t1, test_if_external_interrupt

		/* clear sw interrupt */
		addi x5, x0, 0
		li x10, 0x02000000
		store_x x5, 0(x10)

	    j processed_source


	test_if_external_interrupt:			/* If there is a CLINT and the mtimer interrupt is not pending then check to see if an external interrupt is pending. */
		addi t1, t1, 4					/* 0x80000007 + 4 = 0x8000000b == Machine external interrupt. */
		bne a0, t1, as_yet_unhandled	/* Something as yet unhandled. */

	as_yet_unhandled:
		j as_yet_unhandled

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
```

- ezos_risc_v_trap_handler
  - 检查sp是否为0，若为0跳转到切换下文
  - 存入上文（寄存器）到sp中，包括mstaus
  - 保存mcause、mepc到a0、a1，检查进入了哪个中断
  - 若是sw中断，清除clint的msip位，切换下文
  - 若是异常，跳入synchronous_exception，串口打印mcause等寄存器到
  - processed_source
    - 将ez_tcb_hdyptr指向的地址存入到ez_tcb_curptr指针变量的地址，完成hdy->cur转换
    - 将高优先级任务入口地址恢复到mepc
    - 恢复mstatus
    - 恢复其余寄存器
    - mret跳到mepc中的入口地址
