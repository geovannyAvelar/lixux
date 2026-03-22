#ifndef VM_H
#define VM_H

#define PGSIZE 4096
#define PHYSTOP 0x8000000  // 128 * 1024 * 1024 = 128MB

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

extern void* _end;

struct run {
    struct run *next;
};

void mem_init(void);
void freerange(void *start, void *end);
void *kalloc(void);
void kfree(void *);

#endif
