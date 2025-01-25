
clean:
	rm -f *.o *.elf

qemu:
	riscv64-unknown-elf-gcc -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs \
		-O0 -Wl,--gc-sections -c -mcmodel=medany terminal.c uart.c kernel.c

	riscv64-unknown-elf-as -c entry.s -o entry.o
	
	riscv64-unknown-elf-ld -T linker.ld -nostdlib \
		kernel.o entry.o uart.o terminal.o -o kernel.elf
	
	qemu-system-riscv64 -machine virt -bios none -m 128M -gdb tcp::1234 \
		-device virtio-blk-pci,drive=drive0,id=virtblk0,num-queues=4 \
		-drive file=disk.qcow2,if=none,id=drive0 \
		-S -kernel kernel.elf -serial mon:stdio

debug:
	gdb-multiarch -ex "target remote :1234" -annotate riscv:rv64 --tui kernel.elf