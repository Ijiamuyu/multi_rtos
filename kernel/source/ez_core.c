#include "ez.h"

extern void ez_start_high_rdy(void);
extern void ez_setup_timer_interrupt();

void ez_task_sw(void) {
    WRITE_MREG32(CLINT_ADDR + CLINT_MSIP, 0x1);
}

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

void ez_schedule(void) {
    if (ez_tcb_curptr == ez_rdylist[0].head) {
        ez_tcb_hdyptr = ez_rdylist[1].head;
    } else {
        ez_tcb_hdyptr = ez_rdylist[0].head;
    }

}
