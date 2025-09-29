#include "vm.h"
#include "uart.h"
#include "console.h"

void kmain(void) {
  kvminit();
  uart_init();

  show_banner();
  console_start(256);
}