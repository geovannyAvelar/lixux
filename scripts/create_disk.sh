#!/bin/bash

qemu-img create -f qcow2 disk.img 512M

sudo modprobe nbd
sudo qemu-nbd --connect=/dev/nbd0 disk.img

# Create a single primary partition non-interactively
sudo fdisk /dev/nbd0 <<EOF
n
p
1


w
EOF

# Wait for the partition device to appear
while [ ! -b /dev/nbd0p1 ]; do sleep 0.5; done

sudo mkfs.exfat /dev/nbd0p1

# Create rootfs as user, mount with user ownership
mkdir -p rootfs
sudo mount -o uid=$(id -u),gid=$(id -g) /dev/nbd0p1 rootfs

mkdir -p rootfs/boot

sudo grub-install \
  --boot-directory=rootfs/boot \
  --target=i386-pc \
  /dev/nbd0

echo "set timeout=10
set default=0

menuentry \"Lixux\" {
    multiboot /boot/lixux console=ttyS0
}
" > rootfs/boot/grub/grub.cfg

cp kernel.elf rootfs/boot/lixux

sudo umount rootfs
sudo qemu-nbd --disconnect /dev/nbd0
