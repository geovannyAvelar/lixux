#include "console.h"
#include "types.h"
#include "serial.h"

void console_init(uint64_t buffer_size)
{
    show_prompt();

    char buffer[buffer_size];
    int counter = 0;

    for (;;)
    {
        char character = serial_read();

        switch (character)
        {
        case 10:
        case 13:
            buffer[counter] = '\0';
            counter = 0;

            serial_write('\n');
            show_prompt();

            break;
        case 127:
            if (counter <= 0)
            {
                continue;
            }

            counter--;
            serial_write(0x08);
            serial_write(0x20);
            serial_write(0x08);
            break;
        default:
            if (character == 0 || counter >= (buffer_size - 1))
            {
                continue;
            }

            buffer[counter] = character;
            serial_write(character);
            counter++;
            break;
        }
    }
}

void show_prompt(void)
{
    serial_write('\n');
    serial_write('>');
    serial_write(' ');
}
