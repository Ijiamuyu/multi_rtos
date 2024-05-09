#ifndef __EZ_H__
#define __EZ_H__

#include "riscv-virt.h"
#include "ez_cfg.h"
#include "ezdef.h"

#ifdef EZ_GLOBALS
#define EZ_EXT
#else
#define EZ_EXT extern
#endif

#define WRITE_MREG32(addr,val) (*(ez_uint32_t *)(addr) = (val))
#define READ_MREG32(addr)      (*(ez_uint32_t *)(addr))

#define EZ_STATE_STOPPED (ez_state_t)(0u)
#define EZ_STATE_RUNNING (ez_state_t)(1u)

typedef enum ez_err {
    EZ_ERR_NONE = 0u,

    EZ_ERR_A          = 10000u,
    EZ_ERR_ACCEPT_ISR = 10001u,

    EZ_ERR_B = 11000u,

    EZ_ERR_C          = 12000u,
    EZ_ERR_CREATE_ISR = 12001u,

    EZ_ERR_D       = 13000u,
    EZ_ERR_DEL_ISR = 13001u,

    EZ_ERR_E = 14000u,

    EZ_ERR_F            = 15000u,
    EZ_ERR_FATAL_RETURN = 15001u,

    EZ_ERR_FLAG_GRP_DEPLETED = 15101u,
    EZ_ERR_FLAG_NOT_RDY      = 15102u,
    EZ_ERR_FLAG_PEND_OPT     = 15103u,
    EZ_ERR_FLUSH_ISR         = 15104u,

    EZ_ERR_G = 16000u,

    EZ_ERR_H = 17000u,

    EZ_ERR_I                       = 18000u,
    EZ_ERR_ILLEGAL_CREATE_RUN_TIME = 18001u,
    EZ_ERR_INT_Q                   = 18002u,
    EZ_ERR_INT_Q_FULL              = 18003u,
    EZ_ERR_INT_Q_SIZE              = 18004u,
    EZ_ERR_INT_Q_STK_INVALID       = 18005u,
    EZ_ERR_INT_Q_STK_SIZE_INVALID  = 18006u,

    EZ_ERR_J = 19000u,

    EZ_ERR_K = 20000u,

    EZ_ERR_L                = 21000u,
    EZ_ERR_LOCK_NESTING_OVF = 21001u,

    EZ_ERR_M = 22000u,

    EZ_ERR_MEM_CREATE_ISR     = 22201u,
    EZ_ERR_MEM_FULL           = 22202u,
    EZ_ERR_MEM_INVALID_P_ADDR = 22203u,
    EZ_ERR_MEM_INVALID_BLKS   = 22204u,
    EZ_ERR_MEM_INVALID_PART   = 22205u,
    EZ_ERR_MEM_INVALID_P_BLK  = 22206u,
    EZ_ERR_MEM_INVALID_P_MEM  = 22207u,
    EZ_ERR_MEM_INVALID_P_DATA = 22208u,
    EZ_ERR_MEM_INVALID_SIZE   = 22209u,
    EZ_ERR_MEM_NO_FREE_BLKS   = 22210u,

    EZ_ERR_MSG_POOL_EMPTY    = 22301u,
    EZ_ERR_MSG_POOL_NULL_PTR = 22302u,

    EZ_ERR_MUTEX_NOT_OWNER = 22401u,
    EZ_ERR_MUTEX_OWNER     = 22402u,
    EZ_ERR_MUTEX_NESTING   = 22403u,

    EZ_ERR_N                = 23000u,
    EZ_ERR_NAME             = 23001u,
    EZ_ERR_NO_MORE_ID_AVAIL = 23002u,

    EZ_ERR_O            = 24000u,
    EZ_ERR_OBJ_CREATED  = 24001u,
    EZ_ERR_OBJ_DEL      = 24002u,
    EZ_ERR_OBJ_PTR_NULL = 24003u,
    EZ_ERR_OBJ_TYPE     = 24004u,

    EZ_ERR_OPT_INVALID = 24101u,

    EZ_ERR_EZ_NOT_RUNNING = 24201u,
    EZ_ERR_EZ_RUNNING     = 24202u,

    EZ_ERR_P                = 25000u,
    EZ_ERR_PEND_ABORT       = 25001u,
    EZ_ERR_PEND_ABORT_ISR   = 25002u,
    EZ_ERR_PEND_ABORT_NONE  = 25003u,
    EZ_ERR_PEND_ABORT_SELF  = 25004u,
    EZ_ERR_PEND_DEL         = 25005u,
    EZ_ERR_PEND_ISR         = 25006u,
    EZ_ERR_PEND_LOCKED      = 25007u,
    EZ_ERR_PEND_WOULD_BLOCK = 25008u,

    EZ_ERR_PEZT_NULL_PTR = 25101u,
    EZ_ERR_PEZT_ISR      = 25102u,

    EZ_ERR_PRIO_EXIST   = 25201u,
    EZ_ERR_PRIO         = 25202u,
    EZ_ERR_PRIO_INVALID = 25203u,

    EZ_ERR_PTR_INVALID = 25301u,

    EZ_ERR_Q       = 26000u,
    EZ_ERR_Q_FULL  = 26001u,
    EZ_ERR_Q_EMPTY = 26002u,
    EZ_ERR_Q_MAX   = 26003u,
    EZ_ERR_Q_SIZE  = 26004u,

    EZ_ERR_R                    = 27000u,
    EZ_ERR_REG_ID_INVALID       = 27001u,
    EZ_ERR_ROUND_ROBIN_1        = 27002u,
    EZ_ERR_ROUND_ROBIN_DISABLED = 27003u,

    EZ_ERR_S                        = 28000u,
    EZ_ERR_SCHED_INVALID_TIME_SLICE = 28001u,
    EZ_ERR_SCHED_LOCK_ISR           = 28002u,
    EZ_ERR_SCHED_LOCKED             = 28003u,
    EZ_ERR_SCHED_NOT_LOCKED         = 28004u,
    EZ_ERR_SCHED_UNLOCK_ISR         = 28005u,

    EZ_ERR_SEM_OVF = 28101u,
    EZ_ERR_SET_ISR = 28102u,

    EZ_ERR_STAT_RESET_ISR        = 28201u,
    EZ_ERR_STAT_PRIO_INVALID     = 28202u,
    EZ_ERR_STAT_STK_INVALID      = 28203u,
    EZ_ERR_STAT_STK_SIZE_INVALID = 28204u,
    EZ_ERR_STATE_INVALID         = 28205u,
    EZ_ERR_STATUS_INVALID        = 28206u,
    EZ_ERR_STK_INVALID           = 28207u,
    EZ_ERR_STK_SIZE_INVALID      = 28208u,
    EZ_ERR_STK_LIMIT_INVALID     = 28209u,

    EZ_ERR_T                        = 29000u,
    EZ_ERR_TASK_CHANGE_PRIO_ISR     = 29001u,
    EZ_ERR_TASK_CREATE_ISR          = 29002u,
    EZ_ERR_TASK_DEL                 = 29003u,
    EZ_ERR_TASK_DEL_IDLE            = 29004u,
    EZ_ERR_TASK_DEL_INVALID         = 29005u,
    EZ_ERR_TASK_DEL_ISR             = 29006u,
    EZ_ERR_TASK_INVALID             = 29007u,
    EZ_ERR_TASK_NO_MORE_TCB         = 29008u,
    EZ_ERR_TASK_NOT_DLY             = 29009u,
    EZ_ERR_TASK_NOT_EXIST           = 29010u,
    EZ_ERR_TASK_NOT_SUSPENDED       = 29011u,
    EZ_ERR_TASK_OPT                 = 29012u,
    EZ_ERR_TASK_RESUME_ISR          = 29013u,
    EZ_ERR_TASK_RESUME_PRIO         = 29014u,
    EZ_ERR_TASK_RESUME_SELF         = 29015u,
    EZ_ERR_TASK_RUNNING             = 29016u,
    EZ_ERR_TASK_STK_CHK_ISR         = 29017u,
    EZ_ERR_TASK_SUSPENDED           = 29018u,
    EZ_ERR_TASK_SUSPEND_IDLE        = 29019u,
    EZ_ERR_TASK_SUSPEND_INT_HANDLER = 29020u,
    EZ_ERR_TASK_SUSPEND_ISR         = 29021u,
    EZ_ERR_TASK_SUSPEND_PRIO        = 29022u,
    EZ_ERR_TASK_WAITING             = 29023u,

    EZ_ERR_TCB_INVALID = 29101u,

    EZ_ERR_TLS_ID_INVALID        = 29120u,
    EZ_ERR_TLS_ISR               = 29121u,
    EZ_ERR_TLS_NO_MORE_AVAIL     = 29122u,
    EZ_ERR_TLS_NOT_EN            = 29123u,
    EZ_ERR_TLS_DESTRUCT_ASSIGNED = 29124u,

    EZ_ERR_TICK_PRIO_INVALID     = 29201u,
    EZ_ERR_TICK_STK_INVALID      = 29202u,
    EZ_ERR_TICK_STK_SIZE_INVALID = 29203u,
    EZ_ERR_TICK_WHEEL_SIZE       = 29204u,

    EZ_ERR_TIME_DLY_ISR              = 29301u,
    EZ_ERR_TIME_DLY_RESUME_ISR       = 29302u,
    EZ_ERR_TIME_GET_ISR              = 29303u,
    EZ_ERR_TIME_INVALID_HOURS        = 29304u,
    EZ_ERR_TIME_INVALID_MINUTES      = 29305u,
    EZ_ERR_TIME_INVALID_SECONDS      = 29306u,
    EZ_ERR_TIME_INVALID_MILLISECONDS = 29307u,
    EZ_ERR_TIME_NOT_DLY              = 29308u,
    EZ_ERR_TIME_SET_ISR              = 29309u,
    EZ_ERR_TIME_ZERO_DLY             = 29310u,

    EZ_ERR_TIMEOUT = 29401u,

    EZ_ERR_TMR_INACTIVE         = 29501u,
    EZ_ERR_TMR_INVALID_DEST     = 29502u,
    EZ_ERR_TMR_INVALID_DLY      = 29503u,
    EZ_ERR_TMR_INVALID_PERIOD   = 29504u,
    EZ_ERR_TMR_INVALID_STATE    = 29505u,
    EZ_ERR_TMR_INVALID          = 29506u,
    EZ_ERR_TMR_ISR              = 29507u,
    EZ_ERR_TMR_NO_CALLBACK      = 29508u,
    EZ_ERR_TMR_NON_AVAIL        = 29509u,
    EZ_ERR_TMR_PRIO_INVALID     = 29510u,
    EZ_ERR_TMR_STK_INVALID      = 29511u,
    EZ_ERR_TMR_STK_SIZE_INVALID = 29512u,
    EZ_ERR_TMR_STOPPED          = 29513u,

    EZ_ERR_U = 30000u,

    EZ_ERR_V = 31000u,

    EZ_ERR_W = 32000u,

    EZ_ERR_X = 33000u,

    EZ_ERR_Y         = 34000u,
    EZ_ERR_YIELD_ISR = 34001u,

    EZ_ERR_Z = 35000u
} ez_err;

typedef void (*EzTaskPtr)(void *p_arg);

typedef struct EzTcb {
    ez_stk_t *stk_ptr;
    ez_size_t stk_size;
} EzTcb;

typedef struct EzRdyList {
    EzTcb *head;
    EzTcb *tail;
} EzRdyList;

EZ_EXT EzTcb *ez_tcb_curptr;
EZ_EXT EzTcb *ez_tcb_hdyptr;
EZ_EXT EzRdyList ez_rdylist[EZ_PRIORITY_MAX];
EZ_EXT ez_state_t ez_running;

void ez_task_creat(EzTcb *p_tcb, EzTaskPtr p_task, void *p_arg, ez_stk_t *p_stk_base,
                   ez_size_t stk_size, ez_err *p_err);

void ez_init(ez_err *p_err);
void ez_start(ez_err *p_err);
void ez_schedule(void);

ez_stk_t *ez_task_init(EzTaskPtr p_task, void *arg, ez_stk_t *p_stk_base, ez_size_t stk_size);

void ez_rdylist_init(void);

#endif /* __EZ_H__ */
