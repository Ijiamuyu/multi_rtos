#ifndef __COMMON_H__
#define __COMMON_H__

#include "data_type.h"

#define read_mreg64(addr)       (*(volatile u64 *)((u64)(addr)))
#define write_mreg64(addr, val) (*(volatile u64 *)((u64)(addr)) = (u64)(val))

#define read_mreg32(addr)       (*(volatile u32 *)((u64)(addr)))
#define write_mreg32(addr, val) (*(volatile u32 *)((u64)(addr)) = (u32)(val))

#define read_mreg16(addr)       (*(volatile u16 *)((u64)(addr)))
#define write_mreg16(addr, val) (*(volatile u16 *)((u64)(addr)) = (u16)(val))

#define read_mreg8(addr)       (*(volatile u8 *)((u64)(addr)))
#define write_mreg8(addr, val) (*(volatile u8 *)((u64)(addr)) = (u8)(val))

void hang(void);

#endif /* __COMMON_H__ */
