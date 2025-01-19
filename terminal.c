#include "terminal.h"

void show_banner() {
  uart_puts("Lixux 0.0.1");
  uart_putc('>');
  uart_putc(' ');
}

void terminal_start(size_t buffer_size) {
  char buffer[buffer_size];
  int counter = 0;

  for (;;) {
    char character = uart_getc();

    switch (character) {
    case 10:
    case 13:
      uart_putc('\n');
      uart_putc('>');
      uart_putc(' ');

      buffer[counter] = '\n';
      counter = 0;

      break;
    case 127:
      if (counter <= 0) {
        continue;
      }

      counter--;
      uart_putc(0x08);
      uart_putc(0x20);
      uart_putc(0x08);
      break;
    default:
      if (character == 0 || counter >= (buffer_size - 1)) {
        continue;
      }

      buffer[counter] = character;
      uart_putc(character);
      counter++;
      break;
    }
  }
}