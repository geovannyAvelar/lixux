CC = gcc
AS = as
LD = ld
OBJCOPY = objcopy
CFLAGS = -m32 -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -O0 -Wl,--gc-sections -c
LDFLAGS = -m elf_i386 -nostdlib
ENTRY_S = entry.S
ENTRY_O = entry.o
SOURCES = serial.c vga.c console.c main.c
OBJECTS = $(SOURCES:.c=.o) $(ENTRY_O)
TARGET = kernel.elf

all: $(TARGET) copy_kernel_to_rootfs

$(ENTRY_O): $(ENTRY_S)
	$(AS) --32 -o $@ $<

$(SOURCES:.c=.o): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS) linker.ld
	$(LD) -T linker.ld $(LDFLAGS) $(OBJECTS) -o $(TARGET)

compile: $(OBJECTS)

run:
	qemu-system-x86_64 -hda disk.img -serial stdio

clean:
	rm -f *.o *.elf
