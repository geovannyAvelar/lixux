#include "kalloc.h"

#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)
#define PGSIZE 4096

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

extern char end[];

struct run {
  struct run *next;
};

struct {
  struct run *freelist;
} kmem;

void* memset(void *dst, int c, uint64_t n) {
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

void freerange(void *vstart, void *vend) {
  char *p;
  p = (char*)PGROUNDUP((uint32_t) vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE) {
    kfree(p);
  }
}

void kinit() {
  freerange(end, (void*)PHYSTOP);
}

void kfree(void *v) {
  memset(v, 1, PGSIZE);

  struct run *r;
  r = (struct run*)v;

  r->next = kmem.freelist;
  kmem.freelist = r;
}

void * kalloc(void) {
  struct run *r;

  r = kmem.freelist;
  if(r) {
    kmem.freelist = r->next;
  }

  if(r) {
    memset((char*)r, 5, PGSIZE);
  }

  return (void*)r;
}