#!/bin/bash

mkdir -p test_disks

# MBR with 2 partitions
python3 scripts/diskCreator.py -o ./test_disks/disk_01.bin  -s 200  -t msdos      -p 1:100:primary:ext4     -p 101:200:primary:fat

# GPT with XFS and BTRFS
python3 scripts/diskCreator.py \
-o ./test_disks/disk_02.bin \
-s 520 \
-t gpt \
-p 2:355:primary:xfs \
-p 357:495:primary:btrfs

# GPT with 4 partitions & gaps
sudo python3 scripts/diskCreator.py \
-o ./test_disks/disk_03.bin \
-s 100 \
-t gpt \
-p 2:24:primary:ext4 \
-p 26:48:primary:ext4 \
-p 50:72:primary:ext4 \
-p 74:96:primary:ext4

# GPT with 1 FAT32 partition
python3 scripts/diskCreator.py \
-o ./test_disks/disk_04.bin \
-s 100 \
-t gpt \
-p 2:96:primary:vfat