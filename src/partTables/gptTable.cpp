#include "partitionTables.hpp"
#include <string>

#define MAX_GPT_PARTITION_ENTRY_SIZE 128

#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <ios>

#include "../utils.hpp"

GPTPartitionTable::GPTPartitionTable(std::istream& stream) {
    // https://en.wikipedia.org/wiki/GUID_Partition_Table#Partition_table_header_(LBA_1)

    // GPT header is located at offset 512 bytes
    // First 8 bytes are the signature "EFI PART"
    // Then we have misc values, including the number of partition entries and their size

    // Read the 512 bytes of the header
    char headerData[512];
    stream.seekg(512, std::ios::beg);
    stream.read(headerData, sizeof(headerData));
    size_t size = stream.gcount();

    if (size < 512) {
        throw std::runtime_error("Data size too small for GPT header");
    }

    // The following values are little endian, so we must cast appropriately

    // Starting LBA of partition entries (8 bytes at offset 72)
    uint64_t partitionEntriesLBA = readLE64(headerData + 72);
    // Number of partition entries (4 bytes at offset 80)
    uint32_t numPartitionEntries = readLE32(headerData + 80);
    // Size of each partition entry (4 bytes at offset 84)
    uint32_t sizeOfPartitionEntry = readLE32(headerData + 84);

    // Read partition entries
    stream.seekg(partitionEntriesLBA * 512, std::ios::beg);
    for (uint32_t i = 0; i < numPartitionEntries; ++i) {
        char entry[MAX_GPT_PARTITION_ENTRY_SIZE];
        
        stream.read(entry, sizeOfPartitionEntry);
        size_t entrySize = stream.gcount();

        if (entrySize < sizeOfPartitionEntry) {
            throw std::runtime_error("Data size too small for GPT partition entry");
        }

        // First 16 bytes are the partition type GUID (LE)
        char MBRPartitionTypeGUID[16];
        std::memcpy(MBRPartitionTypeGUID, entry, 16);

        // Check if the partition type GUID is all zeros (unused entry)
        // 00000000-0000-0000-0000-000000000000
        if (std::all_of(MBRPartitionTypeGUID, MBRPartitionTypeGUID + 16, [](char c) { return c == 0; })) {
            continue; // Unused entry
        }

        // Next 16 bytes are the unique partition GUID (LE)
        char uniquePartitionGUID[16];
        std::memcpy(uniquePartitionGUID, entry + 16, 16);

        uint64_t firstLBA = readLE64(entry + 32); // Starting LBA (LE) (8 bytes)
        uint64_t lastLBA = readLE64(entry + 40); // Ending LBA (LE) (8 bytes)
        uint64_t attributes = readLE64(entry + 48); // Attributes (LE) (8 bytes)
        
        char partitionName[72]; // Partition name (UTF-16LE) (72 bytes)
        std::memcpy(partitionName, entry + 56, 72);
        std::u16string name = utf16leToU16String(partitionName, 72);

        partitions.push_back(GPTPartition(firstLBA, lastLBA - firstLBA + 1,
                                          guidToString(MBRPartitionTypeGUID),
                                          guidToString(uniquePartitionGUID),
                                          attributes, name));
    }
}

void GPTPartitionTable::printInfo() const {
    for (const auto& partition : partitions) {
        partition.printInfo();
    }
}

std::vector<Partition*> GPTPartitionTable::getPartitions() const {
    std::vector<Partition*> partPtrs;
    for (const auto& part : partitions) {
        partPtrs.push_back(const_cast<GPTPartition*>(&part));
    }
    return partPtrs;
}