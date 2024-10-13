#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char sprint_buf[2408];

static char *i2a(int value, int base, char *result) {
    // 确定符号位
    int isNegative = 0;
    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    int i = 0;
    // 处理特殊情况：value 为 0
    if (value == 0) {
        result[i++] = '0';
        result[i] = '\0';
        return result;
    }

    // 将整数 value 转换为对应进制的字符串（逆序）
    while (value != 0) {
        int remainder = value % base;
        result[i++] =
                (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value /= base;
    }

    // 如果是十进制且为负数，则添加负号
    if (isNegative) {
        result[i++] = '-';
    }

    // 将结果字符串反转过来
    for (int j = 0; j < i / 2; j++) {
        char temp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = temp;
    }

    // 添加字符串结束符
    result[i] = '\0';

    return result;
}

static void vsLprintf(char *buf, const char *fmt, va_list args) {
    char *p = sprint_buf;
    int m;
    char inner_buf[64];
    char cs;
    int align_nr;

    va_list p_next_arg = args;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        } else { /* a format string begins */
            align_nr = 0;
        }

        fmt++;

        if (*fmt == '%') {
            *p++ = *fmt;
            continue;
        } else if (*fmt == '0') {
            cs = '0';
            fmt++;
        } else {
            cs = ' ';
        }
        while (((unsigned char)(*fmt) >= '0') &&
               ((unsigned char)(*fmt) <= '9')) {
            align_nr *= 10;
            align_nr += *fmt - '0';
            fmt++;
        }

        char *q = inner_buf;
        memset(q, 0, sizeof(inner_buf));

        switch (*fmt) {
        case 'c':
            *q++ = *((char *)p_next_arg);
            p_next_arg += 4;
            break;
        case 'x':
            m = *((int *)p_next_arg);
            i2a(m, 16, q);
            p_next_arg += 4;
            break;
        case 'd':
            m = *((int *)p_next_arg);
            if (m < 0) {
                m = m * (-1);
                *q++ = '-';
            }
            i2a(m, 10, q);
            p_next_arg += 4;
            break;
        case 's':
            strcpy(q, (*((char **)p_next_arg)));
            q += strlen(*((char **)p_next_arg));
            p_next_arg += 4;
            break;
        default:
            break;
        }

        int k;
        for (k = 0; k < ((align_nr > strlen(inner_buf))
                                 ? (align_nr - strlen(inner_buf))
                                 : 0);
             k++) {
            *p++ = cs;
        }
        q = inner_buf;
        while (*q) {
            *p++ = *q++;
        }
    }

    *p = '\0';
}

extern void uart_send_string(const char *s);

void console_printf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    vsLprintf(sprint_buf, str, args);
    va_end(args);
    uart_send_string(sprint_buf);
}
