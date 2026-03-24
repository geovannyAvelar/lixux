CC = gcc
AS = as
LD = ld
OBJCOPY = objcopy
CFLAGS = -m32 -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -O0 -Wl,--gc-sections -c
LDFLAGS = -m elf_i386 -nostdlib
ENTRY_S = entry.S
ENTRY_O = entry.o
SOURCES = vm.c serial.c vga.c console.c main.c
OBJECTS = $(SOURCES:.c=.o) $(ENTRY_O)
TARGET = kernel.elf
DISK_IMG = disk.img

all: $(TARGET)

$(ENTRY_O): $(ENTRY_S)
	$(AS) --32 -o $@ $<

$(SOURCES:.c=.o): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS) linker.ld
	$(LD) -T linker.ld $(LDFLAGS) $(OBJECTS) -o $(TARGET)

$(DISK_IMG): $(TARGET)
	./scripts/create_disk.sh

run: $(DISK_IMG)
	qemu-system-i386 -m 128M \
	-machine type=pc,accel=kvm \
	-hda $(DISK_IMG) \
	-serial stdio

clean:
	rm -f *.o *.elf
