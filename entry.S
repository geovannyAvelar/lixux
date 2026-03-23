.section .multiboot_header
.align 4
.long 0x1BADB002          # Multiboot magic number
.long 0x0                 # Flags
.long -(0x1BADB002 + 0x0) # Checksum

.section .text
.global _start

.section .data
.align 8
gdt:
  .quad 0x0000000000000000   # null descriptor
  .quad 0x00CF9A000000FFFF   # code segment
  .quad 0x00CF92000000FFFF   # data segment

gdtr:
  .word (3 * 8) - 1
  .long gdt

_start:
  cli

  mov %eax, %esi
  mov %ebx, %edi

  lgdt gdtr

  ljmp $0x08, $protected_mode_entry

protected_mode_entry:
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss

  mov $0x90000, %esp

  push %edi   # mbi_addr
  push %esi   # magic

  call kmain

  cli
  hlt