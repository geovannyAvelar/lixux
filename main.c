#include "uart.h"
#include "console.h"
#include "virtio.h"

void kmain(void) {
  uart_init();
  virtio_init();

  show_banner();
  console_start(256);
}