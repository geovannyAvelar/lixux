#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#define SERIAL_PORT 0x3F8

char serial_read();
void serial_write(char a);
void serial_print(const char* str);
void serial_print_int(uint32_t val);

#endif