#include "tx_port.h"
#include "tx_user.h"
#include "cpu.h"

ULONG64 *mtimeptr = (ULONG64 *)(CLINT_ADDR + CLINT_MTIME);
ULONG64 *mtimecmptr = (ULONG64 *)(CLINT_ADDR + CLINT_MTIMECMP);

ULONG threadx_onetick;

void threadx_setup_timer_interrupt(void) {
    ULONG *cur_time_high =
            (ULONG *)(CLINT_ADDR + CLINT_MTIME + 0x4);
    ULONG *cur_time_low = (ULONG *)(CLINT_ADDR + CLINT_MTIME);

    threadx_onetick = CPU_CLOCK_HZ / THREADX_TIMER_TICK_HZ;
    *mtimecmptr = (ULONG64)*cur_time_high;
    *mtimecmptr <<= 32;
    *mtimecmptr |= (ULONG64)*cur_time_low;
    *mtimecmptr += (ULONG64)threadx_onetick;
}

