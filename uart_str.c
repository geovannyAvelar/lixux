#include "uart.h"

void uart_puts(char* str) {
  while (*str) uart_putc(*str++);
  uart_putc('\n');
}