#include "console.h"
#include "string.h"
#include "elf.h"

extern uint8_t _binary_hello_elf_start[];
extern uint8_t _binary_hello_elf_end[];
extern uint8_t _binary_hello_elf_size[];

void show_banner(void)
{
  uart_puts("Lixux 0.0.1");
  uart_puts("\n");
  uart_putc('>');
  uart_putc(' ');
}

void show_prompt(void)
{
  uart_putc('\n');
  uart_putc('>');
  uart_putc(' ');
}

void console_start(uint64_t buffer_size)
{
  char buffer[buffer_size];
  int counter = 0;

  for (;;)
  {
    char character = uart_getc();

    switch (character)
    {
    case 10:
    case 13:
      buffer[counter] = '\0';
      counter = 0;

      uart_putc('\n');

      if (strcmp(buffer, "hello") == 0)
      {
        struct elf elf;
        elf.header = &(struct elf_header){0};
        elf.section_headers = &(struct section_header){0};

        parse_elf(_binary_hello_elf_start, &elf);
      }

      show_prompt();

      break;
    case 127:
      if (counter <= 0)
      {
        continue;
      }

      counter--;
      uart_putc(0x08);
      uart_putc(0x20);
      uart_putc(0x08);
      break;
    default:
      if (character == 0 || counter >= (buffer_size - 1))
      {
        continue;
      }

      buffer[counter] = character;
      uart_putc(character);
      counter++;
      break;
    }
  }
}