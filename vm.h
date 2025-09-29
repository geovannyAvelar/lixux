#include "types.h"

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 512

#define PTE_V 0x001
#define PTE_R 0x002
#define PTE_W 0x004
#define PTE_X 0x008
#define PTE_U 0x010
#define PTE_G 0x020
#define PTE_A 0x040
#define PTE_D 0x080

typedef uint64_t pte_t;

typedef struct {
  pte_t entries[PAGE_ENTRIES];
} pagetable_t;

uint8_t kvminit(void);
uint8_t memset(void *dst, uint8_t value, uint64_t size);
