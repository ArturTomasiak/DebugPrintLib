# DebugPrint

A lightweight, cross-platform C library for printing various data types and modifying the color of the standard output stream via a single, unified interface. This library is primarily designed for debugging purposes. Generates no warnings, except for value computed not used, which can be avoided by deleting unused args_x's in print.h.

---

## overview of functionalities

- **`print(...)`**  
  Supports up to 10 arguments (although one can easily add more). For floating-point types (`float` and `double`), the output is limited to an accuracy of 0.000001. `print(...)` supports following argument types (constants allowed, works with standard c variables):
  -  int8_t
  -  int16_t
  -  int32_t
  -  int64_t
  -  uint8_t
  -  uint16_t
  -  uint32_t
  -  uint64_t
  -  float
  -  double
  -  _Bool
  -  char
  -  char*
  <br>
  this excludes long double, void* and arrays

- **`change_print_color(Color color)`**  
  Changes the text color of the standard output stream. Can be called via `print_color(color)` macro. It accepts the following arguments:
  - `RED`
  - `GREEN`
  - `BLUE`
  - `YELLOW`
  - `WHITE`
  - `RESET`

---

## demonstration (example.c)

```c
#include "src/print.h"

void error(uint32_t line, char* file, char* error) {
    print(file, " line ", line, " | ");
    print_color(RED);
    print("ERROR: ");
    print_color(RESET);
    print(error);
}

int main() {
    error(__LINE__, __FILE__, "example error");
    print("\n");
    return 0;
}
```

---

## file structure

- **print.h**  
  Contains the primary API declarations, type definitions, and macros used for printing, changing text color, and handling multiple arguments.

- **LinPrint.c**  
  Provides the implementation for Linux systems using system calls to write directly to `STDOUT_FILENO`.

- **WinPrint.c**  
  Provides the implementation for Windows systems using the Win32 API.

- **PlatformCheck.h**  
  Detects the target platform (Linux or Windows) and conditionally includes the appropriate implementation file.

---

## compilation and usage

This library is free to use and integrate into any project. To experiment with the provided `example.c` file, simply download the source code and compile it using one of the provided build scripts:

- **Linux**: `bash build.sh`
- **Windows**: `build.bat`

The provided build files are configured for `gcc`, but gcc is not a requirement; the library is compatible with any compiler that supports C11 features.

## print.h pre processor explanation

```c
#define print_one(x) _Generic((x), \
    int8_t: print_i64, \
    ...
    const char*: print_string, \
    default: print_unknown \
)(x)
```

the print_one(x) macro chooses a function based on recieved argument type via C11's _Generic keyword

```c
#define args_1(WHAT, X) (WHAT(X))
#define args_2(WHAT, X, ...) ((WHAT(X)) + args_1(WHAT, __VA_ARGS__))
#define args_3(WHAT, X, ...) ((WHAT(X)) + args_2(WHAT, __VA_ARGS__))
...
#define args_9(WHAT, X, ...) ((WHAT(X)) + args_8(WHAT, __VA_ARGS__))
#define args_10(WHAT, X, ...) ((WHAT(X)) + args_9(WHAT, __VA_ARGS__))
```

above code prepares for dealing with up to 10 arguments passed, where WHAT is always print_one and `__VA_ARGS__` are the arguments not passed as X

the goal is to achieve f.e. print(a, b, c) expanding to print_one(a) + print_one(b) + print_one(c) without too much code and without the usage of GNU statement expressions

```c
#define get_macro(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME

#define print(...) do { \
    get_macro(__VA_ARGS__, args_10, args_9, args_8, args_7, args_6, args_5, args_4, args_3, args_2, args_1)(print_one, __VA_ARGS__); \
} while (0)
```

get_macro is called from a do while that ends always after one iteration so that the macro behaves like a single statemnt

get_macro will pick the right macro based on the amount of arguments in `__VA_ARGS__` as the 11th value NAME will increase depending on how many arguments are sent, for example when sending print(a, b)
_1 = a,
_2 = b,
_3 = args_10,
_4 = args_9
...
_10 = args_3
NAME = args_2

after get_macro completes, it changes into the chosen macro that will get called, for print(a, b)
```c
get_macro(__VA_ARGS__, args_10, args_9, args_8, args_7, args_6, args_5, args_4, args_3, args_2, args_1)(print_one, __VA_ARGS__);
```
changes into
```c
args_2(print_one, __VA_ARGS__);
```

