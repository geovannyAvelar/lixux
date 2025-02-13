
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld
CFLAGS = -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -O0 -Wl,--gc-sections -c \
				-mcmodel=medany
LDFLAGS = -nostdlib
SOURCES = kalloc.c console.c uart.c virtio.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = kernel.elf

clean:
	rm -f *.o *.elf

build:
	$(CC) $(CFLAGS) $(SOURCES)
	$(AS) -c entry.s -o entry.o
	$(LD) -T linker.ld $(LDFLAGS) entry.o $(OBJECTS) -o $(TARGET)

qemu:
	make build

	qemu-system-riscv64 -machine virt -bios none -m 128M -cpu rv64 -smp 1 \
		-device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 \
		-drive file=fs.img,if=none,id=x0,format=raw \
		-global virtio-mmio.force-legacy=false \
		-serial mon:stdio \
		-gdb tcp::1234 \
		-kernel $(TARGET)

debug:
	gdb-multiarch -ex "target remote :1234" -annotate riscv:rv64 --tui $(TARGET)