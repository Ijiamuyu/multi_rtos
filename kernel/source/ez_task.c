#include "ez.h"

void ez_task_creat(EzTcb *p_tcb, EzTaskPtr p_task, void *p_arg, ez_stk_t *p_stk_base,
                   ez_size_t stk_size, ez_err *p_err)
{
    ez_stk_t *p_sp;

    p_sp            = ez_task_init(p_task, p_arg, p_stk_base, stk_size);
    p_tcb->stk_ptr  = p_sp;
    p_tcb->stk_size = stk_size;

    *p_err = EZ_ERR_NONE;
}
