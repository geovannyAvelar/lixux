#include "uart.h"
#include "console.h"
#include "kalloc.h"
#include "virtio.h"

void kmain(void) {
  uart_init();
  kinit();
  virtio_init();

  show_banner();
  console_start(256);
}