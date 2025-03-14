#include "PlatformCheck.h"
#if linux

#include "print.h"
#include <unistd.h>
#include <sys/syscall.h>

int32_t print_i64(int64_t buffer) {
    _Bool negative = (buffer < 0);
    if (negative)
        buffer = -buffer;
    char* newbuffer = malloc(25 * sizeof(char));
    if(newbuffer == NULL) {
        print_string("memory allocation failed");
        return -1;
    }
    int64_t i = 0;
    do {
        newbuffer[i++] = '0' + (buffer % 10);
        buffer /= 10;
    } while (buffer > 0);
    if (negative)
        newbuffer[i++] = '-';
    newbuffer[i] = '\0';
    for (uint32_t j = 0, k = i - 1; j < k; j++, k--) {
        char temp = newbuffer[j];
        newbuffer[j] = newbuffer[k];
        newbuffer[k] = temp;
    }
    uint32_t output_length = print_string(newbuffer);
    free(newbuffer);
    return output_length;
}

int32_t print_u64(uint64_t buffer) {
    char* newbuffer = malloc(25 * sizeof(char));
    if(newbuffer == NULL) {
        print_string("memory allocation failed");
        return -1;
    }
    uint64_t i = 0;
    do {
        newbuffer[i++] = '0' + (buffer % 10);
        buffer /= 10;
    } while (buffer > 0);
    newbuffer[i] = '\0';
    for (uint32_t j = 0, k = i - 1; j < k; j++, k--) {
        char temp = newbuffer[j];
        newbuffer[j] = newbuffer[k];
        newbuffer[k] = temp;
    }
    uint32_t output_length = print_string(newbuffer);
    free(newbuffer);
    return output_length;
}

int32_t print_f64(double buffer) {
    _Bool negative = (buffer < 0);
    if (negative)
        buffer = -buffer;
    uint64_t int_part = (uint64_t)buffer;
    double fraction = buffer - (double)int_part;
    uint32_t int_length = 0;
    uint64_t temp = (int_part == 0) ? 1 : int_part;
    while (temp) {
        temp /= 10;
        int_length++;
    }
    uint32_t frac = (uint32_t)(fraction * 1000000);
    while (frac != 0 && frac % 10 == 0)
        frac /= 10;
    char* newbuffer = malloc(40 * sizeof(char));
    if(newbuffer == NULL) {
        print_string("memory allocation failed");
        return -1;
    }
    uint64_t i = 0;
    do {
        newbuffer[i++] = '0' + (frac % 10);
        frac /= 10;
    } while (frac > 0);
    newbuffer[i++] = '.';
    do {
        newbuffer[i++] = '0' + (int_part % 10);
        int_part /= 10;
    } while (int_part > 0);
    if (negative)
        newbuffer[i++] = '-';
    newbuffer[i] = '\0';
    for (uint32_t j = 0, k = i - 1; j < k; j++, k--) {
        char temp = newbuffer[j];
        newbuffer[j] = newbuffer[k];
        newbuffer[k] = temp;
    }
    uint32_t output_length = print_string(newbuffer);
    free(newbuffer);
    return output_length;
}

int32_t print_bool(_Bool buffer) {
    char ch = (buffer) ? '1' : '0';
    return print_char(ch);
}

int32_t print_char(char buffer) {
    return syscall(SYS_write, STDOUT_FILENO, &buffer, 1);
}

int32_t print_string(char* buffer) {
    if (!buffer) return 0;
    const char* buffer_copy = buffer;
    for (; *buffer; buffer++)
        ;
    uint32_t length = buffer - buffer_copy;
    return syscall(SYS_write, STDOUT_FILENO, buffer_copy, length);
}

int32_t print_unknown() {
    print_string("variable type not supported");
    return -1;
}

void change_print_color(Color color) {
    switch (color) {
        case RED: syscall(SYS_write, STDOUT_FILENO, "\033[31m", 5); break;
        case GREEN: syscall(SYS_write, STDOUT_FILENO, "\033[32m", 5); break;
        case BLUE: syscall(SYS_write, STDOUT_FILENO, "\033[34m", 5); break;
        case YELLOW: syscall(SYS_write, STDOUT_FILENO, "\033[33m", 5); break;
        case WHITE: syscall(SYS_write, STDOUT_FILENO, "\033[37m", 5); break;
        case RESET: syscall(SYS_write, STDOUT_FILENO, "\033[0m", 4); break;
        default: return;
    }
}

#endif