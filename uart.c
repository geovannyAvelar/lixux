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

void uart_init() {
  volatile uint8_t* uart_ier = (volatile uint8_t*) UART_ADDRESS + IER;
  *uart_ier = 0x0;

  volatile uint8_t* uart_lcr = (volatile uint8_t*) UART_ADDRESS + LCR;
  *uart_lcr = LCR_BAUD_LATCH;

  volatile uint8_t* uart = (volatile uint8_t*) UART_ADDRESS;
  *uart = 0x03;

  *uart_lcr = (volatile uint8_t*) LCR_EIGHT_BITS;

  volatile uint8_t* uart_fcr = (volatile uint8_t*) UART_ADDRESS + FCR;
  *uart_fcr = FCR_FIFO_ENABLE | FCR_FIFO_CLEAR;

  *uart_ier = IER_TX_ENABLE | IER_RX_ENABLE;
}

char uart_getc() {
  uint8_t uart_lsr = UART_ADDRESS + LSR;
  uint8_t uart_line_status_data_ready = UART_ADDRESS + 1;

  if (uart_lsr & uart_line_status_data_ready == 0) {
    return 0;
  }

  char* c = UART_ADDRESS;

  return *c;
}

void uart_putc(const char c) {
  char* uart = UART_ADDRESS;
  *uart = c;
}

void uart_puts(char* str) {
  while (*str) uart_putc(*str++);
  uart_putc('\n');
}