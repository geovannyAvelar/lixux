#!/bin/bash

sudo umount rootfs
sudo qemu-nbd --disconnect /dev/nbd0