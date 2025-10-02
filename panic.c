#include "panic.h"

void panic(char *s) {
  uart_puts("panic: ");
  uart_puts(s);
  uart_puts("\n");
  for(;;)
    ;
}