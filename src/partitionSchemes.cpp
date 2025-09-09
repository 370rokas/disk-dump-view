#include "partitionSchemes.hpp"

#include <string>
#include <cstdint>

PartitionScheme detectPartitionScheme(const char* data, size_t size) {
    // Not enough data to determine
    if (size < 512) {
        return PARTITION_SCHEME_UNKNOWN;
    }

    // GPT has "EFI PART" signature at offset 512
    if (size >= 1024 && std::string(data + 512, 8) == "EFI PART") {
        return PARTITION_SCHEME_GPT;
    }

    // MBR has 0x55 at offset 510 and 0xAA at offset 511
    if (static_cast<uint8_t> (data[510]) == 0x55 && static_cast<uint8_t>(data[511]) == 0xAA) {
        return PARTITION_SCHEME_MBR;
    }

    // Unknown scheme
    return PARTITION_SCHEME_UNKNOWN;
}