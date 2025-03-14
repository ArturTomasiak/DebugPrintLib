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