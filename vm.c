#include "vm.h"
#include "uart.h"
#include "panic.h"

extern char etext[];
extern char end[];

struct memory memory = {0};

void init_memory(void) {
  uart_puts("Initializing memory...\n");

  uint64_t allocated_mem = 0;
  uint8_t pages_number = 0;

  uart_puts("Kernel ends at physical address: ");
  uart_puthex((uint64_t)&end);
  uart_puts("\n");

  void *addr = (void *) PGROUNDUP((uint64_t) end);
  for (; (addr + PAGE_SIZE) <= MAX_MEM_ADDR; addr += PAGE_SIZE) {
    memset((void *)addr, 1, PAGE_SIZE);
    struct page *page = (struct page *) addr;
    page->next = memory.free_list;

    memory.free_list = page;

    allocated_mem += PAGE_SIZE;
    pages_number++;
  }

  uart_puts("Memory ends at physical address: ");
  uart_puthex((uint64_t)addr);
  uart_puts("\n");

  uart_putint(pages_number);
  uart_puts(" memory page(s) created (");
  uart_putint(allocated_mem / (1024 * 1024));
  uart_puts(" MB(s) of memory allocated)\n");

  pagetable_t *kernel_pagetable = alloc_page();
  memset((void *)kernel_pagetable, 0, PAGE_SIZE);

  uart_puts("Kernel pagetable allocated at physical address: ");
  uart_puthex((uint64_t)kernel_pagetable);
  uart_puts("\n");

  enable_virtual_memory(kernel_pagetable);

  mappages(kernel_pagetable, UART_ADDRESS, PAGE_SIZE, UART_ADDRESS, PTE_R | PTE_W);
}

void enable_virtual_memory(pagetable_t* pagetable) {
  pagetable_t *kernel_pagetable = (pagetable_t *)((uint64_t)&end);

  const uint64_t SATP_MODE_SV39;
  uint64_t ppn = ((uint64_t)kernel_pagetable) >> 12;
  uint64_t satp_val = (SATP_MODE_SV39 << 60) | ppn;

  asm volatile("csrw satp, %0" :: "r"(satp_val) : "memory");
  asm volatile("sfence.vma" ::: "memory");
}

int mappages(pagetable_t pagetable, uint64_t va, uint64_t size, uint64_t pa, int perm) {
  uint64_t a, last;
  pte_t *pte;

  a = va;
  last = va + size - PAGE_SIZE;

  for(;;) {
    uart_puts("Mapping virtual address: ");
    uart_puthex(a);
    uart_puts(" to physical address: ");
    uart_puthex(pa);
    uart_puts("\n");

    if ((pte = walk(pagetable, a, 1)) == 0) {
      uart_puts("Failed to walk page table for address ");
      uart_puthex(a);
      uart_puts("\n");
      return -1;
    }

    if (*pte & PTE_V) {
      uart_puts("Invalid PTE: Memory position ");
      uart_puthex((uint64_t)pte);
      uart_puts(", value ");
      uart_puthex(*pte);
      uart_puts(" ");
      panic("PANIC: Invalid page table entry");
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
    uint64_t index = (va >> (12 + (9 * level))) & 0x1FF;
    pte_t *pte = &pagetable[index];

    if ((*pte & PTE_V) == 1) {
      pagetable = (pagetable_t) ((*pte >> 10) << 12);
    } else {
      if (!alloc || (pagetable = (pte_t *) alloc_page()) == 0) {
        return 0;
      }

      memset(pagetable, 0, PAGE_SIZE);
      *pte = ((((uint64_t)pagetable) >> 12) << 10) | PTE_V;
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
    memset((void *)page, 5, PAGE_SIZE);
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