#ifndef PTTYPES_HPP
#define PTTYPES_HPP

#include <string>
#include <unordered_map>

// https://en.wikipedia.org/wiki/Partition_type
enum MBRPartitionType {
    EMPTY = 0x00,
    FAT12 = 0x01,
    // 0x02 - 0x03: Ignored
    FAT16 = 0x04,
    EXTENDED = 0x05,
    FAT16B = 0x06,
    NTFS = 0x07, // Also exFAT, HPFS
    // 0x08 - 0x0A: Ignored
    FAT32 = 0x0B,
    FAT32_LBA = 0x0C,
    FAT16_LBA = 0x0E,
    EXTENDED_LBA = 0x0F,
    // 0x10 - x41: Ignored
    LINUX_OLD_SWAP = 0x42,
    LINUX_OLD_NATIVE = 0x43,
    // 0x44 - 0x81: Ignored
    LINUX_SWAP = 0x82,
    LINUX_NATIVE = 0x83,
    // 0x84: Ignored
    LINUX_EXTENDED = 0x85,
    // Others ignored. See link for full list
};

const std::unordered_map<std::string, std::string> GPTPartitionTypeGUIDs = {
    {"EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "WINDOWS_BDP"},
    {"0FC63DAF-8483-4772-8E79-3D69D8477DE4", "LINUX_FS"},
    {"A19D880F-05FC-4D3B-A006-743F0F84911E", "RAID"},
};

#endif // PTTYPES_HPP