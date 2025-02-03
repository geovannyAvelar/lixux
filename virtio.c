#include "virtio.h"
#include "kalloc.h"

#define PGSIZE                      4096

#define VIRTIO0                     0x10001000

// STATUS BITS
#define VIRTIO_CONFIG_S_ACKNOWLEDGE 1
#define VIRTIO_CONFIG_S_DRIVER      2
#define VIRTIO_CONFIG_S_DRIVER_OK   4
#define VIRTIO_CONFIG_S_FEATURES_OK 8

// FEATURE BITS
#define VIRTIO_BLK_F_RO             5
#define VIRTIO_BLK_F_SCSI           7
#define VIRTIO_BLK_F_CONFIG_WCE     11
#define VIRTIO_BLK_F_MQ             12
#define VIRTIO_F_ANY_LAYOUT         27
#define VIRTIO_RING_F_INDIRECT_DESC 28
#define VIRTIO_RING_F_EVENT_IDX     29

#pragma pack(1)

typedef struct virito_mmio {
  uint32_t magic_value;
  uint32_t version;
  uint32_t device_id;
  uint32_t vendor_id;
  uint32_t device_features;
  uint32_t device_features_sel;
  uint32_t : 2;
  uint32_t driver_features;
  uint32_t driver_features_sel;
  uint32_t : 2;
  uint32_t queue_sel;
  uint32_t queue_num_max;
  uint32_t queue_num;
  uint32_t : 2;
  uint32_t queue_ready;
  uint32_t : 2;
  uint32_t queue_notify;
  uint32_t : 3;
  uint32_t interrupt_status;
  uint32_t interrupt_ack;
  uint32_t : 2;
  uint32_t status;
  uint32_t : 2;
  uint32_t queue_desc_low;
  uint32_t queue_desc_high;
  uint32_t : 2;
  uint32_t queue_avail_low;
  uint32_t queue_avail_high;
  uint32_t : 2;
  uint32_t queue_used_low;
  uint32_t queue_used_high;
  uint32_t : 21;
  uint32_t config_generation;
  uint32_t config[0];
} virito_mmio;

#pragma pack()

static struct disk {
  struct virtq_desc *desc;
  struct virtq_avail *avail;
  struct virtq_used *used;
  char free[NUM];
  uint16_t used_idx;

  struct {
    struct buf *b;
    char status;
  } info[NUM];

  struct virtio_blk_req ops[NUM];

} disk;

void virtio_init(void) {
  volatile virito_mmio *mmio = (volatile virito_mmio *) VIRTIO0;

  if (mmio->magic_value != 0x74726976 ||
      mmio->version != 2 ||
      mmio->device_id != 2 ||
      mmio->vendor_id != 0x554d4551) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  uint32_t status = 0;

  mmio->status = status;

  status |= VIRTIO_CONFIG_S_ACKNOWLEDGE;
  mmio->status = status;

  status |= VIRTIO_CONFIG_S_DRIVER;
  mmio->status = status;

  uint64_t features = mmio->device_features;
  features &= ~(1 << VIRTIO_BLK_F_RO);
  features &= ~(1 << VIRTIO_BLK_F_SCSI);
  features &= ~(1 << VIRTIO_BLK_F_CONFIG_WCE);
  features &= ~(1 << VIRTIO_BLK_F_MQ);
  features &= ~(1 << VIRTIO_F_ANY_LAYOUT);
  features &= ~(1 << VIRTIO_RING_F_EVENT_IDX);
  features &= ~(1 << VIRTIO_RING_F_INDIRECT_DESC);
  mmio->device_features = features;

  status |= VIRTIO_CONFIG_S_FEATURES_OK;
  mmio->status = status;

  status = mmio->status;
  if(!(status & VIRTIO_CONFIG_S_FEATURES_OK)) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  mmio->queue_sel = 0;

  if(mmio->queue_ready) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  uint32_t max = mmio->queue_num_max;
  if(max == 0) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  if(max < NUM) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  disk.desc = kalloc();
  disk.avail = kalloc();
  disk.used = kalloc();

  if(!disk.desc || !disk.avail || !disk.used) {
    // TODO: remember to handle this error. Probably kernel panic.
  }

  memset(disk.desc, 0, PGSIZE);
  memset(disk.avail, 0, PGSIZE);
  memset(disk.used, 0, PGSIZE);

  mmio->queue_num = NUM;

  mmio->queue_desc_low = (uint64_t) disk.desc;
  mmio->queue_desc_high = (uint64_t) disk.desc >> 32;
  mmio->queue_avail_low = (uint64_t) disk.avail;
  mmio->queue_avail_high = (uint64_t) disk.avail >> 32;
  mmio->queue_used_low = (uint64_t) disk.used;
  mmio->queue_used_high = (uint64_t) disk.used >> 32;

  mmio->queue_ready = 0x1;

  for(int i = 0; i < NUM; i++) {
    disk.free[i] = 1;
  }

  status |= VIRTIO_CONFIG_S_DRIVER_OK;
  mmio->status = status;
}