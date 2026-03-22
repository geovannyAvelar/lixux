#include "serial.h"

static unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

char serial_read() {
    while (!(inb(SERIAL_PORT + 5) & 1)); // Wait for data
    return inb(SERIAL_PORT);
}

int serial_is_transmit_empty() {
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_write(char a) {
    while (serial_is_transmit_empty() == 0);
    outb(SERIAL_PORT, a);
}

void serial_print(const char* str) {
    while (*str) {
        serial_write(*str++);
    }
}
