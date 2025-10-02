#include "types.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 512

#define PGROUNDUP(sz) (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

#define KERNBASE 0x80000000L
#define MAX_MEM_ADDR (KERNBASE + 128 * 1024 * 1024)

#define PTE_V 0x001
#define PTE_R 0x002
#define PTE_W 0x004
#define PTE_X 0x008
#define PTE_U 0x010
#define PTE_G 0x020
#define PTE_A 0x040
#define PTE_D 0x080

typedef uint64_t pte_t;
typedef uint64_t *pagetable_t;

struct page {
  struct page *next;
};

struct memory {
  struct page *free_list;
};

void init_memory(void);
void enable_virtual_memory(pagetable_t pagetable);
int mappages(pagetable_t pagetable, uint64_t va, uint64_t size, uint64_t pa, int perm);
pte_t *walk(pagetable_t pagetable, uint64_t va, uint8_t alloc);
void* alloc_page(void);
uint8_t memset(void *dst, uint8_t value, uint64_t size);
