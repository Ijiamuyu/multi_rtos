#ifndef __EZ_DEF_H__
#define __EZ_DEF_H__

/*
 *          数据类型
 */

/* RT-Thread 基础数据类型重定义*/
typedef signed char ez_int8_t;
typedef signed short ez_int16_t;
typedef signed long ez_int32_t;
typedef unsigned char ez_uint8_t;
typedef unsigned short ez_uint16_t;
typedef unsigned long ez_uint32_t;
typedef int ez_bool_t;

/* 32bit CPU*/
typedef long ez_base_t;
typedef unsigned long ez_ubase_t;
typedef ez_base_t ez_err_t;
typedef ez_uint32_t ez_time_t;
typedef ez_uint32_t ez_tick_t;
typedef ez_base_t ez_flag_t;
typedef ez_ubase_t ez_size_t;
typedef ez_ubase_t ez_stk_t;
typedef ez_base_t ez_off_t;
typedef ez_uint32_t ez_state_t;
typedef ez_uint8_t ez_prio_t;

/* 布尔数据类型重定义*/
#define EZ_TRUE 1
#define EZ_FALSE 0

#ifdef __CC_ARM
#define ez_inline static __inline
#define ALIGN(n) __attribute__((aligned(n)))

#elif defined(__IAR_SYSTEMS_ICC__)
#define ez_inline static inline
#define ALIGN(n) PRAGMA(data_alignment == == n)

#elif defined(__GNUC__)
#define ez_inline static __inline
#define ALIGN(n) __attribute__((aligned(n)))
#else
#error not suppoezed tool chain
#endif

#define EZ_ALIGN(size, align) (((size) + (align) - 1) & ~((align) - 1))
#define EZ_ALIGN_DOWN(size, align) ((size) & ~((align) - 1))

#define EZ_NULL (0)

#endif /* __EZ_DEF_H__*/
