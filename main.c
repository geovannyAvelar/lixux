#include "console.h"
#include "vm.h"

void kmain(void)
{
    mem_init();
    console_init(256);
}