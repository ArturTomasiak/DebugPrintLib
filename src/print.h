#pragma once

#include <stdlib.h>
#include <stdint.h>

typedef enum {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE,
    RESET
} Color;

#define print_one(x) _Generic((x), \
    int8_t: print_i64, \
    int16_t: print_i64, \
    int32_t: print_i64, \
    int64_t: print_i64, \
    uint8_t: print_u64, \
    uint16_t: print_u64, \
    uint32_t: print_u64, \
    uint64_t: print_u64, \
    float: print_f64, \
    double: print_f64, \
    _Bool: print_bool, \
    char: print_char, \
    char*: print_string, \
    const int8_t: print_i64, \
    const int16_t: print_i64, \
    const int32_t: print_i64, \
    const int64_t: print_i64, \
    const uint8_t: print_u64, \
    const uint16_t: print_u64, \
    const uint32_t: print_u64, \
    const uint64_t: print_u64, \
    const float: print_f64, \
    const double: print_f64, \
    const _Bool: print_bool, \
    const char: print_char, \
    const char*: print_string, \
    default: print_unknown \
)(x)

#define args_1(WHAT, X) (WHAT(X))
#define args_2(WHAT, X, ...) ((WHAT(X)) + args_1(WHAT, __VA_ARGS__))
#define args_3(WHAT, X, ...) ((WHAT(X)) + args_2(WHAT, __VA_ARGS__))
#define args_4(WHAT, X, ...) ((WHAT(X)) + args_3(WHAT, __VA_ARGS__))
#define args_5(WHAT, X, ...) ((WHAT(X)) + args_4(WHAT, __VA_ARGS__))
#define args_6(WHAT, X, ...) ((WHAT(X)) + args_5(WHAT, __VA_ARGS__))
#define args_7(WHAT, X, ...) ((WHAT(X)) + args_6(WHAT, __VA_ARGS__))
#define args_8(WHAT, X, ...) ((WHAT(X)) + args_7(WHAT, __VA_ARGS__))
#define args_9(WHAT, X, ...) ((WHAT(X)) + args_8(WHAT, __VA_ARGS__))
#define args_10(WHAT, X, ...) ((WHAT(X)) + args_9(WHAT, __VA_ARGS__))

#define get_macro(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME

#define print(...) do { \
    get_macro(__VA_ARGS__, args_10, args_9, args_8, args_7, args_6, args_5, args_4, args_3, args_2, args_1)(print_one, __VA_ARGS__); \
} while (0)

int32_t print_i64(int64_t buffer);
int32_t print_u64(uint64_t buffer);
int32_t print_f64(double buffer);
int32_t print_bool(_Bool buffer);
int32_t print_char(char buffer);
int32_t print_string(char* buffer);
int32_t print_unknown();

void change_print_color(Color color);
#define print_color(color) change_print_color(color)