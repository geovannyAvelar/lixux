#include "types.h"

void uart_init();
void uart_putc(const char c);
char uart_getc();
void uart_puts(char* str);