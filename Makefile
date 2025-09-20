
CC = riscv64-elf-gcc
AS = riscv64-elf-as
LD = riscv64-elf-ld
OBJCOPY = riscv64-elf-objcopy
CFLAGS = -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -O0 -Wl,--gc-sections -c \
				-mcmodel=medany
LDFLAGS = -nostdlib
SOURCES = string.c scheduler.c kalloc.c console.c uart.c virtio.c main.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = kernel.elf

clean:
	rm -f *.o *.elf

build:
	$(CC) $(CFLAGS) $(SOURCES)
	$(AS) -c entry.s -o entry.o
	$(OBJCOPY) -I binary -O elf64-littleriscv -B riscv hello hello.o
	$(OBJCOPY) --rename-section .data=.hello hello.o
	$(LD) -T linker.ld $(LDFLAGS) entry.o hello.o $(OBJECTS) -o $(TARGET)

qemu:
	make build
	make fs

	qemu-system-riscv64 -machine virt -bios none -m 128M -cpu rv64 -smp 1 \
		-device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 \
		-drive file=fs.img,if=none,id=x0,format=raw \
		-global virtio-mmio.force-legacy=false \
		-serial mon:stdio \
		-gdb tcp::1234 \
		-kernel $(TARGET)

debug:
	gdb-multiarch -ex "target remote :1234" -annotate riscv:rv64 --tui $(TARGET)

fs:
	qemu-img create -f raw fs.img 512m