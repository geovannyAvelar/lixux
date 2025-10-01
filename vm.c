#include "vm.h"
#include "uart.h"

extern char etext[];
extern char end[];

struct memory memory = {0};

void init_memory(void) {
  uart_puts("Initializing memory...\n");

  uint64_t allocated_mem = 0;
  uint8_t pages_number = 0;

  for (void *addr = (void *) end; (addr + PAGE_SIZE) < MAX_MEM_ADDR; addr += PAGE_SIZE) {
    memset((void *)addr, pages_number, PAGE_SIZE);
    struct page *page = (struct page *) addr;
    page->next = memory.free_list;
    memory.free_list = page;
    allocated_mem += PAGE_SIZE;
    pages_number++;
  }

  uart_putint(pages_number - 1);
  uart_puts(" memory page(s) created (");
  uart_putint(allocated_mem / (1024 * 1024));
  uart_puts(" MB(s) of memory allocated)\n");

  pagetable_t *kernel_pagetable = create_pagetable();

  mappages(kernel_pagetable, UART_ADDRESS, UART_ADDRESS, PAGE_SIZE, PTE_R | PTE_W);
  mappages(kernel_pagetable, KERNBASE, KERNBASE, (uint64_t)etext-KERNBASE, PTE_R | PTE_X);
  mappages(kernel_pagetable, (uint64_t)etext, (uint64_t)etext, MAX_MEM_ADDR-(uint64_t)etext, PTE_R | PTE_W);
}

pagetable_t * create_pagetable(void) {
  pagetable_t *kernel_pagetable = (pagetable_t *)((uint64_t)&end);

  const uint64_t SATP_MODE_SV39;
  uint64_t ppn = ((uint64_t)kernel_pagetable) >> 12;
  uint64_t satp_val = (SATP_MODE_SV39 << 60) | ppn;

  asm volatile("csrw satp, %0" :: "r"(satp_val) : "memory");
  asm volatile("sfence.vma" ::: "memory");

  return kernel_pagetable;
}

int mappages(pagetable_t pagetable, uint64_t va, uint64_t size, uint64_t pa, int perm) {
  uint64_t a, last;
  pte_t *pte;

  a = va;
  last = va + size - PAGE_SIZE;
  while (1) {
    if ((pte = walk(pagetable, va, 1)) == NULL) {
      return -1;
    }

    if (*pte & PTE_V) {
      return -1;
    }

    *pte = (pa >> 12) << 10 | perm | PTE_V;

    if (a == last) {
      break;
    }

    a += PAGE_SIZE;
    pa += PAGE_SIZE;
  }

  return 0;
}

pte_t *walk(pagetable_t pagetable, uint64_t va, uint8_t alloc) {
  for (uint8_t level = 2; level > 0; level--) {
    uint64_t index = (va >> (12 + level * 9)) & 0x1FF;
    pte_t *pte = &pagetable[index];

    if ((*pte & PTE_V) && ((*pte & (PTE_R|PTE_W|PTE_X)) == 0)) {
      uint64_t ppn = (*pte >> 10) & (1ULL << 44) - 1;
      pagetable = (pagetable_t)(ppn << 12);

      if (*pte & PTE_V) {
        pagetable = (pagetable_t) ((*pte >> 10) << 12);
      } else {
        if (!alloc || (pagetable = (pte_t *) alloc_page()) == NULL) {
          return NULL;
        }

        uint64_t new_ppn = ((uint64_t)pagetable) >> 12;
        *pte = (new_ppn << 10) | PTE_V;
      }
    }
  }

  return &pagetable[(va >> 12) & 0x1FF];
}

void* alloc_page(void) {
  struct page *page = memory.free_list;

  if (page) {
    memory.free_list = page->next;
  }

  if (page) {
    memset((void *)page, 0, PAGE_SIZE);
  }

  return (void *)page;
}

uint8_t memset(void *dst, uint8_t value, uint64_t size) {
  uint8_t *p = (uint8_t *)dst;
  for (uint64_t i = 0; i < size; i++) {
    p[i] = value;
  }
  return 0;
}