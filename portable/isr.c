#include "riscv-virt.h"

static void int_to_str(const char* s, unsigned int num, char* str) {
    int i, s_len;
    for (i = 0; s[i] != '\0'; i++) {
        str[i] = s[i];
    }
    s_len = i;
    do {
        int digit = num & 0xF;  // 取得最低 4 位数字
        str[i++] = (digit < 10)
                ? (digit + '0')
                : (digit - 10 + 'a');  // 将数字转换为十六进制字符
        num >>= 4;                     // 去掉最低 4 位数字
    } while (num != 0);
    str[i] = '\0';  // 字符串结尾
    // 反转字符串
    int start = s_len;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static void print_csr(void) {
    unsigned int mcause, mepc, mtval, mstatus, mtvec, mip, mie;
    char str[20];

    asm volatile("csrr %0, mcause" : "=r"(mcause));
    int_to_str("\tmcause:0x", mcause, str);
    uart_send_string(str);

    asm volatile("csrr %0, mtvec" : "=r"(mtvec));
    int_to_str("\tmtvec:0x", mtvec, str);
    uart_send_string(str);

    asm volatile("csrr %0, mepc" : "=r"(mepc));
    int_to_str("\tmepc:0x", mepc, str);
    uart_send_string(str);

    asm volatile("csrr %0, mtval" : "=r"(mtval));
    int_to_str("\tmtval:0x", mtval, str);
    uart_send_string(str);

    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    int_to_str("\tmstatus:0x", mstatus, str);
    uart_send_string(str);

    asm volatile("csrr %0, mie" : "=r"(mie));
    int_to_str("\tmie:0x", mie, str);
    uart_send_string(str);

    asm volatile("csrr %0, mip" : "=r"(mip));
    int_to_str("\tmip:0x", mip, str);
    uart_send_string(str);
}

void synchronous_exception(void) {
    uart_send_string("synchronous exception");
    print_csr();
    while (1)
        ;
}

void software_handler(void) {
    uart_send_string("software exception");
    print_csr();
    while (1)
        ;
}

void external_handler(void) {
    uart_send_string("external exception");
    print_csr();
    while (1)
        ;
}

void yet_unhandler(void) {
    uart_send_string("yet_unhandler exception");
    print_csr();
    while (1)
        ;
}
