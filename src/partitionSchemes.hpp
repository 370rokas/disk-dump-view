#ifndef PARTITION_SCHEMES_HPP
#define PARTITION_SCHEMES_HPP

#include <cstddef>

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

#endif // PARTITION_SCHEMES_HPP