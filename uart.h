#include "types.h"

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

void uart_init();
void uart_putc(const char c);
char uart_getc();
void uart_puts(char* str);
void uart_putint(uint64_t num);
void uart_puthex(uint64_t num);