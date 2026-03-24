#include "vm.h"

#include "serial.h"

#include <stdint.h>

static struct run *freelist;

void mem_init(void)
{
    int count = 0;

    char *p = (char*) PGROUNDUP((uint64_t)&_end);
    for (; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE) {
        count++;
        kfree(p);
    }

    serial_print_int(count);
    serial_print(" memory pages allocated");
}

void freerange(void *start, void *end) {
    char *p;
    for (p = (char*)start; p + PGSIZE <= (char*)end; p += PGSIZE)
        kfree(p);
}

void *kalloc(void) {
    struct run *r = freelist;
    if (r)
        freelist = r->next;
    return (void*)r;
}

void kfree(void *p) {
    struct run *r = (struct run*)p;
    r->next = freelist;
    freelist = r;
}