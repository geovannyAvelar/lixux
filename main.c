#include <stdint.h>

#include "console.h"
#include "vm.h"

void kmain(uint32_t magic, uint32_t mbi_addr)
{
    if (magic != 0x2BADB002) {
        while (1);
    }

    mem_init();
    console_init(256);
}