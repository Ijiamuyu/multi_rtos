/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://www.github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ns16550.h"
#include "riscv-virt.h"

int xGetCoreID(void) {
    int id;

    __asm("csrr %0, mhartid" : "=r"(id));

    return id;
}

void uart_send_string(const char *s) {
    struct device dev;
    size_t i;

    dev.addr = NS16550_ADDR;

    for (i = 0; i < strlen(s); i++) {
        vOutNS16550(&dev, s[i]);
    }
    vOutNS16550(&dev, '\n');
}

void uart_send(const char *s, int len) {
    struct device dev;
    size_t i;

    dev.addr = NS16550_ADDR;

    for (i = 0; i < len; i++) {
        vOutNS16550(&dev, s[i]);
    }
}

__attribute__((unused)) static void uart_putc(const char c) {
    struct device dev;

    dev.addr = NS16550_ADDR;

    vOutNS16550(&dev, c);
}

void handle_trap(void) {
    uart_send_string("in trap");
    while (1)
        ;
}

static char sprint_buf[2408];

void uart_printf(const char *str, ...) {
    va_list args;
    int n;
    // 第一个可变形参指针
    va_start(args, str);
    // 根据字符串fmt，将对应形参转换为字符串，并组合成新的字符串存储在sprint_buf[]缓存中，返回字符个数。
    n = vsprintf(sprint_buf, str, args);
    // c标准要求在同一个函数中va_start 和va_end 要配对的出现。
    va_end(args);
    // 调用相关驱动接口,将将sprintf_buf中的内容输出n个字节到设备，
    // 此处可以是串口、控制台、Telnet等,在嵌入式开发中可以灵活挂接
    uart_send(sprint_buf, n);
}
