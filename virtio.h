#include "types.h"

#define NUM 8

struct virtq_desc {
  uint64_t addr;
  uint32_t len;
  uint16_t flags;
  uint16_t next;
};

struct virtio_blk_req {
  uint32_t type;
  uint32_t reserved;
  uint64_t sector;
};

struct virtq_avail {
  uint16_t flags;
  uint16_t idx;
  uint16_t ring[NUM];
  uint16_t unused;
};

struct virtq_used_elem {
  uint32_t id;
  uint32_t len;
};

struct virtq_used {
  uint16_t flags;
  uint16_t idx;
  struct virtq_used_elem ring[NUM];
};

void virtio_init(void);