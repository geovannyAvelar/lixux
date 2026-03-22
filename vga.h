#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000

void print(char *str, int color);
void send_char(char c, int color);

#endif