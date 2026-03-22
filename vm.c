#include "vm.h"
#include <stdint.h>

static struct run *freelist;

void mem_init(void)
{
    char *p = (char*) PGROUNDUP((uint64_t)&_end);
    for (; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE)
        kfree(p);
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