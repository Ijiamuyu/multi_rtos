#include "ez.h"

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
