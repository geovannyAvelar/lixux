#include "uart.h"
#include "console.h"

void kmain(void) {
  uart_init();

  show_banner();
  console_start(256);
}