#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ns16550.h"
#include "cpu.h"

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

