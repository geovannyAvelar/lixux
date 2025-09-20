#include "types.h"

struct elf {
  struct elf_header *header;
  struct section_header *section_headers;
};

struct elf_header
{
  unsigned char magic[4];
  unsigned char class_;
  unsigned char data;
  unsigned char version;
  unsigned char osabi;
  unsigned char abiversion;
  unsigned char pad[7];
  uint16_t type;
  uint16_t machine;
  uint32_t version2;
  uint64_t entry;
  uint64_t phoff;
  uint64_t shoff;
  uint32_t flags;
  uint16_t ehsize;
  uint16_t phentsize;
  uint16_t phnum;
  uint16_t shentsize;
  uint16_t shnum;
  uint16_t shstrndx;
};

struct section_header
{
  uint32_t name;
  uint32_t type;
  uint64_t flags;
  uint64_t addr;
  uint64_t offset;
  uint64_t size;
  uint32_t link;
  uint32_t info;
  uint64_t addralign;
  uint64_t entsize;
};

uint8_t parse_elf(const uint8_t *elf_data, struct elf *elf);
uint8_t parse_elf_header(const uint8_t *elf_data, struct elf_header *header);
uint8_t parse_section_headers(const uint8_t *elf_data, const struct elf_header *header,
                              struct section_header *sectionHeaders);
uint8_t parse_text_section(const uint8_t *elf_data, const struct elf_header *header,
  const struct section_header *sectionHeaders);