# timetick
任务：使用clint的定时器作为systick，在定时器中断服务函数切换任务

## code

```c
void task1(void *p_arg) {
    while (1) {
        if(task1_flag){
            task1_flag = 0;
            task2_flag = 1;
            task1_cnt++;
            uart_printf("hello task1 %d\n", task1_cnt);
        }
    }
}

void task2(void *p_arg) {
    while (1) {
        if(task2_flag){
            task2_flag = 0;
            task1_flag = 1;
            task2_cnt++;
            uart_printf("hello task2 %d\n", task2_cnt);
        }
    }
}
```
- 任务切换
  - 使用标志位控制打印一次输出
  - 任务切换ra寄存器保存当前程序pc，所以恢复时从ra到mepc，而不是从函数入口开始执行

```c
void ez_start(ez_err *p_err) {
    if (ez_running == EZ_STATE_STOPPED) {
        ez_tcb_hdyptr = ez_rdylist[0].head;
        ez_setup_timer_interrupt();
        ez_start_high_rdy();
        *p_err = EZ_ERR_FATAL_RETURN;
    } else {
        *p_err = EZ_STATE_RUNNING;
    }
}

void ez_setup_timer_interrupt(void) {
    ez_uint32_t *cur_time_high = (ez_uint32_t *)(CLINT_ADDR + CLINT_MTIME + 0x4);
    ez_uint32_t *cur_time_low = (ez_uint32_t *)(CLINT_ADDR + CLINT_MTIME);

    ez_onetick = CPU_CLOCK_HZ / EZ_TIMER_TICK_HZ;
    *mtimecmptr = (ez_time_t)*cur_time_high;
    *mtimecmptr <<= 32;
    *mtimecmptr |= (ez_time_t)*cur_time_low;
    *mtimecmptr += (ez_time_t)ez_onetick;
}

void ez_time_tick(void)
{
    ez_schedule();
}
```

- 在ez_start中初始化timer
- ez_setup_timer_interrupt中初始化mtimecmp寄存器
  - mtimecmp置初值，为mtime + ez_onetick
  - 每次进入定时器中断，任务切换

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

	/* enable timer interrupt */
	csrr x5, mie
	addi x5, x5, 0x80
	csrw mie, x5

	j ezos_risc_v_trap_handler

ez_hang:
	/* never here */
	j  ez_hang
	.endfunc

......
test_if_mtimer:
    addi t0, x0, 1

    slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
    addi t1, t0, 7					/* 0x8000[]0007 == machine timer interrupt. */
    bne a0, t1, test_if_external_interrupt

    /* add time tick code here */
    jal ez_time_tick

    load_x t0, mtimecmptr
    load_x t1, mtimeptr
    lw t2, 0(t1)
    lw t3, 4(t1)
    lw t5, ez_onetick
    add t4, t5, t2
    sltu t5, t4, t2
    add t6, t3, t5
    sw t4, 0(t0)
    sw t6, 4(t0)

    j processed_source

```

- ez_start_high_rdy中开启mtime的中断使能
- test_if_mtimer配置mtimecmp寄存器
  - ez_time_tick切换任务
  - 由于mtime、mtimecmp寄存器是64位，所以需要分成俩个32位计算
  - 获取最新的mtime，加上ez_onetick，存入mtimecmp，切换下文
