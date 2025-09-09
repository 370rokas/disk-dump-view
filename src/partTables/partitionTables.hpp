#ifndef PARTITION_TABLES_HPP
#define PARTITION_TABLES_HPP

#include <cstddef>
#include <istream>
#include <vector>

#include "../partition.hpp"

enum PartitionScheme {
    PARTITION_SCHEME_MBR,
    PARTITION_SCHEME_GPT,
    PARTITION_SCHEME_UNKNOWN
};

/**
    * Detects the partition scheme of a disk image.
    * @param data Pointer to the beginning of the disk image data.
    * @param size Size of the disk image data in bytes.
    * @return Detected partition scheme.
*/
PartitionScheme detectPartitionScheme(const char* data, size_t size);

class PartitionTable {
    public:
        virtual ~PartitionTable() = default;
        
        virtual void printInfo() const = 0;
};

class MBRPartitionTable : public PartitionTable {
    public:
        MBRPartitionTable(const char* data, size_t size);

        std::vector<MBRPartition> partitions; // MBR supports up to 4 primary partitions

        void printInfo() const override;
};

class GPTPartitionTable : public PartitionTable {
    public:
        GPTPartitionTable(std::istream& stream);

        std::vector<GPTPartition> partitions; // GPT supports up to 128 primary partitions

        void printInfo() const override;
};

#endif // PARTITION_TABLES_HPP