#include "string.h"
#include "elf.h"
#include "uart.h"

uint8_t parse_elf(const uint8_t *elf_data, struct elf *elf)
{
  uint8_t result = parse_elf_header(elf_data, elf->header);

  if (!result)
  {
    uart_puts("Error: Failed to parse ELF header\n");
    return 0;
  }

  struct section_header section_headers[elf->header->shnum];

  result = parse_section_headers(elf_data, elf->header, section_headers);

  if (!result)
  {
    uart_puts("Error: Failed to parse section headers\n");
    return 0;
  }

  parse_text_section(elf_data, elf->header, section_headers);

  if (!result)
  {
    uart_puts("Error: Failed to parse .text section\n");
    return 0;
  }

  return 1;
}

uint8_t parse_elf_header(const uint8_t *elf_data, struct elf_header *header)
{
  if (!elf_data || !header)
  {
    uart_puts("Error: NULL pointer in parse_elf_header\n");
    return 0;
  }

  for (uint8_t i = 0; i < sizeof(struct elf_header); i++)
  {
    ((uint8_t *)header)[i] = elf_data[i];
  }

  if (header->magic[0] != 0x7F || header->magic[1] != 'E' ||
      header->magic[2] != 'L' || header->magic[3] != 'F')
  {
    uart_puts("Error: Invalid ELF magic number\n");
    return 0;
  }

  return 1;
}

uint8_t parse_section_headers(const uint8_t *elf_data, const struct elf_header *header,
                              struct section_header *section_headers)
{
  if (!elf_data || !section_headers || !header)
  {
    uart_puts("Error: NULL pointer in parse_section_headers\n");
    return 0;
  }

  const struct section_header *sh_table = (const struct section_header *)(elf_data + header->shoff);

  if (!sh_table)
  {
    uart_puts("Error: Failed to obtain section header table pointer\n");
    return 0;
  }

  for (uint16_t i = 0; i < header->shnum; ++i)
  {
    section_headers[i] = sh_table[i];
  }

  return 1;
}

uint8_t parse_text_section(const uint8_t *elf_data, const struct elf_header *header,
                           const struct section_header *section_headers)
{
  char *string_table = elf_data + section_headers[header->shstrndx].offset;

  for (uint16_t i = 0; i < header->shnum; ++i)
  {
    struct section_header section = section_headers[i];

    char *sec_name = string_table + section.name;

    if (strcmp(sec_name, ".text") == 0)
    {
      const uint8_t *text_section = elf_data + section.offset;
      if (!text_section)
      {
        uart_puts("Error: Failed to obtain .text section pointer\n");
        return 0;
      }

      size_t offset = 0;
      while (offset < section.size)
      {
        uint8_t b0 = text_section[offset];
        uint8_t b1 = text_section[offset + 1];
        uint16_t first2bytes = b0 | (b1 << 8);
        size_t instrSize = (first2bytes & 0x3) == 0x3 ? 4 : 2;

        uint8_t opcode = 0;
        uint32_t instruction = 0;
        if (instrSize == 4)
        {
          instruction = text_section[offset] |
                        (text_section[offset + 1] << 8) |
                        (text_section[offset + 2] << 16) |
                        (text_section[offset + 3] << 24);
          opcode = instruction & 0x7F;
        }
        else
        {
          instruction = text_section[offset] |
                        (text_section[offset + 1] << 8);
          opcode = instruction & 0x3;
          offset += instrSize;
          continue;
        }

        offset += instrSize;
      }
    }
  }

  return 1;
}