#include "uart.h"
#include "terminal.h"

void kmain(void) {
  uart_init();

  show_banner();
  terminal_start(256);
}