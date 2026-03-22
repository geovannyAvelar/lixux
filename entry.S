.section .multiboot_header
.align 4
.long 0x1BADB002          # Multiboot magic number
.long 0x0                 # Flags
.long -(0x1BADB002 + 0x0) # Checksum

.section .text
.global _start

_start:
    call kmain
    cli
    hlt

.section .note.GNU-stack,"",@progbits
