#include "partitionTables.hpp"

#include <cstdint>
#include <stdexcept>

MBRPartitionTable::MBRPartitionTable(const char* data, size_t size) {

    // https://en.wikipedia.org/wiki/Master_boot_record#Sector_layout

    // First 446 bytes are bootstrap code   
    // Then we have 4 partition entries of 16 bytes each

    // Last 2 bytes are the boot signature (0x55, 0xAA)
    if (size < 512) {
        throw std::runtime_error("Data size too small for MBR");
    }

    // Read partition entries one by one
    for (int curPartition = 0; curPartition < 4; ++curPartition) {
        const char* entry = data + 446 + curPartition * 16;

        uint8_t status = static_cast<uint8_t>(entry[0]); // 1 byte
        // CHS address of first absolute sector in partition (3 bytes)
        uint8_t partitionType = static_cast<uint8_t>(entry[4]); // 1 byte
        // CHS address of last absolute sector in partition (3 bytes)
        uint32_t firstSector = *reinterpret_cast<const uint32_t*>(entry + 8); // 4 bytes
        uint32_t numSectors = *reinterpret_cast<const uint32_t*>(entry + 12); // 4 bytes
        
        if (partitionType == static_cast<uint8_t>(MBRPartitionType::EMPTY)) {
            // Skip empty partitions
            continue;
        }

        bool isBootable = (status == 0x80);
        MBRPartitionType pType = static_cast<MBRPartitionType>(partitionType);

        partitions.push_back(MBRPartition(firstSector, numSectors, pType, isBootable));
    }
}

void MBRPartitionTable::printInfo() const {
    for (const auto& partition : partitions) {
        partition.printInfo();
    }
}

std::vector<Partition*> MBRPartitionTable::getPartitions() const {
    std::vector<Partition*> partPtrs;
    for (const auto& part : partitions) {
        partPtrs.push_back(const_cast<MBRPartition*>(&part));
    }
    return partPtrs;
}