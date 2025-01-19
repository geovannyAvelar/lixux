
all:
	riscv64-unknown-linux-gnu-gcc -Wall -Wextra -c -mcmodel=medany kernel.c -o kernel.o -ffreestanding
	riscv64-unknown-elf-as -c entry.s -o entry.o
	riscv64-unknown-elf-ld -T linker.ld  -nostdlib kernel.o entry.o -o kernel.elf

clean:
	rm *.o *.elf

qemu:
	riscv64-unknown-linux-gnu-gcc -g -ffreestanding -nostartfiles -nostdlib -nodefaultlibs -O0 -Wl,--gc-sections -c -mcmodel=medany terminal.c uart_str.c kernel.c
	riscv64-unknown-elf-as -c entry.s -o entry.o
	riscv64-unknown-elf-as -c uart.s -o uart.o
	riscv64-unknown-elf-ld -T linker.ld -nostdlib kernel.o entry.o uart.o uart_str.o terminal.o -o kernel.elf
	qemu-system-riscv64 -machine virt -bios none -m 128M -gdb tcp::1234 -S -kernel kernel.elf -serial mon:stdio
