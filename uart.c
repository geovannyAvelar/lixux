#include "uart.h"

#define UART_ADDRESS 0x10000000

#define IER 1
#define FCR 2
#define LCR 3
#define LSR 5

#define LCR_BAUD_LATCH (1<<7)
#define LCR_EIGHT_BITS (3<<0)
#define FCR_FIFO_ENABLE (1<<0)
#define FCR_FIFO_CLEAR (3<<1)
#define IER_TX_ENABLE (1<<1)
#define IER_RX_ENABLE (1<<0)

#pragma pack(1)

typedef volatile struct uart_regs_t {
  uint8_t base_address;
  uint8_t ier;
  uint8_t fcr;
  uint8_t lcr;
  uint8_t: 8; // Padding of eight bits
  uint8_t lsr;
}uart_regs_t;

#pragma pack()

void uart_init(void) {
  uart_regs_t* regs = (uart_regs_t*) UART_ADDRESS;

  regs->ier = 0x0;
  regs->lcr = LCR_BAUD_LATCH;
  regs->base_address = 0x03;
  regs->lcr = LCR_EIGHT_BITS;
  regs->fcr = FCR_FIFO_ENABLE | FCR_FIFO_CLEAR;
  regs->ier = IER_TX_ENABLE | IER_RX_ENABLE;
}

char uart_getc(void) {
  uart_regs_t* regs = (uart_regs_t*) UART_ADDRESS;

  if ((regs->lsr & regs->ier) == 0) {
    return 0;
  }

  return regs->base_address;
}

void uart_putc(const char c) {
  uart_regs_t* regs = (uart_regs_t*) UART_ADDRESS;
  regs->base_address = c;
}

void uart_puts(char* str) {
  while (*str) uart_putc(*str++);
  uart_putc('\n');
}