#include "vm.h"
#include "uart.h"
#include "console.h"

void kmain(void) {
  uart_init();
  init_memory();

  show_banner();
  console_start(256);
}