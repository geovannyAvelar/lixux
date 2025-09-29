#include "vm.h"

extern char end;

uint8_t kvminit(void) {
  pagetable_t *kernel_pagetable = (pagetable_t *)((uint64_t)&end);
  memset(kernel_pagetable, 0, PAGE_SIZE);

  const uint64_t SATP_MODE_SV39 = NULL;
  uint64_t ppn = ((uint64_t)kernel_pagetable) >> 12;
  uint64_t satp_val = (SATP_MODE_SV39 << 60) | ppn;

  asm volatile("csrw satp, %0" :: "r"(satp_val) : "memory");
  asm volatile("sfence.vma" ::: "memory");

  return 0;
}

pte_t *walk(uint64_t va, uint8_t alloc) {
  pagetable_t *kernel_pagetable = (pagetable_t *)((uint64_t)&end);

  for (uint8_t level = 2; level > 0; level--) {
    uint64_t index = (va >> (12 + level * 9)) & 0x1FF;
    pte_t *pte = &kernel_pagetable->entries[index];
    if ((*pte & PTE_V) && ((*pte & (PTE_R|PTE_W|PTE_X)) == 0)) {
      uint64_t ppn = (*pte >> 10) & (1ULL << 44) - 1;
      kernel_pagetable = (pagetable_t *)(ppn << 12);
    }
  }
}

uint8_t memset(void *dst, uint8_t value, uint64_t size) {
  uint8_t *p = (uint8_t *)dst;
  for (uint64_t i = 0; i < size; i++) {
    p[i] = value;
  }
  return 0;
}