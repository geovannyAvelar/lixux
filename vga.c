#include "vga.h"

void print(char *str, int color) {
    char* video_memory = (char*) VGA_ADDRESS;
    while (*str != '\0') {
        *video_memory = *str;
        *(video_memory + 1) = color;
        video_memory += 2;
        str++;
    }
}

void send_char(char c, int color) {
    char* video_memory = (char*) VGA_ADDRESS;
    *video_memory = c;
    *(video_memory + 1) = color;
}

