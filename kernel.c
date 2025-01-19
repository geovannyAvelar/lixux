#include <stdint.h>
#include <stddef.h>

void uart_init();
void uart_putc(const char c);
char uart_getc();
void uart_puts(char* str);

void kmain(void) {
  uart_init();

  uart_puts("Lixux 0.0.1");
  uart_putc('>');
  uart_putc(' ');

  char buffer[256];
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
      if (character == 0) {
        continue;
      }

      buffer[counter] = character;
      uart_putc(character);
      counter++;
      break;
    }
  }
}

void uart_puts(char* str) {
  while (*str) uart_putc(*str++);
  uart_putc('\n');
}