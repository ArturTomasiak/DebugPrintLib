#include "PlatformCheck.h"
#if windows

#include "print.h"
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

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
    DWORD output_length;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), &buffer, 1, &output_length, NULL);
    return (uint32_t)output_length;
}

int32_t print_string(char* buffer) {
    if (!buffer) return 0;
    DWORD output_length;
    char* temp;
    for (temp = buffer; *temp; temp++)
        ;
    uint32_t length = temp - buffer;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, length, &output_length, NULL);
    return (uint32_t)output_length;
}

int32_t print_unknown() {
    print_string("variable type not supported");
    return -1;
}

void change_print_color(Color color) {
    WORD color_code;
    switch (color) {
        case RED: color_code = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case GREEN: color_code = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case BLUE: color_code = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case YELLOW: color_code = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case WHITE: color_code = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case RESET: color_code = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        default: return;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_code);
}

#endif