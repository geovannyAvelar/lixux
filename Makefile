
clean:
	rm -f *.o *.elf

qemu:
	riscv64-unknown-elf-gcc -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs \
		-O0 -Wl,--gc-sections -c -mcmodel=medany console.c uart.c main.c

	riscv64-unknown-elf-as -c entry.s -o entry.o
	
	riscv64-unknown-elf-ld -T linker.ld -nostdlib \
		main.o entry.o uart.o console.o -o kernel.elf

	qemu-system-riscv64 -machine virt -bios none -m 128M -gdb tcp::1234 \
		-device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0 \
		-drive file=fs.img,if=none,id=x0 \
		-S -kernel kernel.elf -serial mon:stdio

debug:
	gdb-multiarch -ex "target remote :1234" -annotate riscv:rv64 --tui kernel.elf