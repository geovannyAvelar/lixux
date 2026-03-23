OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS
{
  . = 1M;

  .text : {
      KEEP(*(.multiboot_header))
      *(.text*)
  }

  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : {
    *(.bss*)
    *(COMMON)
  }

  .stack (NOLOAD) : {
      . = ALIGN(16);
      stack_bottom = .;
      . += 16K;
      stack_top = .;
  }

  . = ALIGN(4096);
  PROVIDE(_end = .);
}
