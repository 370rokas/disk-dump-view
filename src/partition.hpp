#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <cstdint>
#include <iostream>

#include "partTables/ptTypes.hpp"

class Partition {
public:
    uint64_t startSector;
    uint64_t sectorCount;
    bool bootable = false;

    Partition(uint64_t start, uint64_t count)
        : startSector(start), sectorCount(count) {}

    virtual ~Partition() = default;

    virtual void printInfo() const {
        std::cout << "Partition - Start: " << startSector << ", Size: " << sectorCount
                  << ", Bootable: " << bootable << "\n";
    }
};

class MBRPartition : public Partition {
public:
    MBRPartitionType type;

    MBRPartition(uint64_t start, uint64_t count, MBRPartitionType type, bool boot)
        : Partition(start, count), type(type) {
        bootable = boot;
    }

    void printInfo() const override {
        std::cout << "MBR Partition - Type: 0x" << std::hex << static_cast<int>(type) << std::dec
                  << ", Start: " << startSector << ", Size: " << sectorCount
                  << ", Bootable: " << bootable << "\n";
    }

};

class GPTPartition : public Partition {
public:
    std::string typeGUID;
    std::string uniqueGUID;
    std::u16string name; // UTF-16LE
    uint64_t attributes;

    GPTPartition(uint64_t start, uint64_t count, const std::string& typeGUID,
                 const std::string& uniqueGUID, uint64_t attrs, const std::u16string& name)
        : Partition(start, count), typeGUID(typeGUID), uniqueGUID(uniqueGUID), attributes(attrs), name(name) {}

    void printInfo() const override {
        std::cout << "GPT Partition - Type GUID: " << typeGUID
                  << ", Unique GUID: " << uniqueGUID
                  << ", Start: " << startSector
                  << ", Size: " << sectorCount
                  << ", Attributes: 0x" << std::hex << attributes << std::dec
                  << "\n";
    }
};

#endif // PARTITION_HPP