#ifndef FS_MAGIC_NUMS_HPP
#define FS_MAGIC_NUMS_HPP

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

// Filesystem signatures are stored in a hashmap for quick lookup
// The key is a combination of the offset and the magic number

struct FSSignatureKey {
	uint64_t offset;   // Offset in bytes
	std::string magic; // Magic number as a string

	bool operator==(const FSSignatureKey& other) const { return offset == other.offset && magic == other.magic; }
};

struct FSSignatureKeyHash {
	std::size_t operator()(const FSSignatureKey& key) const {
		std::hash<uint64_t> hash1;
		std::hash<std::string> hash2;

		return hash1(key.offset) ^ (hash2(key.magic) << 1);
	}
};

/**
 * Identifies the filesystem type based on magic numbers.
 * @param stream Input stream of the disk image or partition.
 * @param baseOffset Base offset to start reading from (default is 0).
 * @return Detected filesystem type as a string, or "UNKNOWN" if not identified.
 */
std::string identifyFilesystem(std::istream& stream, std::streamoff baseOffset = 0);

using FSMap = std::unordered_map<FSSignatureKey, std::string, FSSignatureKeyHash>;

static const FSMap fsMagicNums = {
	{{0x438, "\x53\xEF"}, "EXT4"}, // little-endian 0xEF53
	{{0x00, "XFSB"}, "XFS"},
};

#endif // FS_MAGIC_NUMS_HPP