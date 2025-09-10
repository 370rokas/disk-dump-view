#include "ext4.hpp"

#include <iostream>

FS_EXT4::FS_EXT4(std::istream& stream, std::streamoff baseOffset) {
	// Superblock is located at an offset of 1024 bytes from the start of the partition
	stream.seekg(baseOffset + 1024, std::ios::beg);
	if (!stream.good()) {
		std::cerr << "Failed to seek to base offset: " << baseOffset << std::endl;
		return;
	}

	stream.read(reinterpret_cast<char*>(&superblock), sizeof(EXT4_SuperBlock));
	if (!stream.good()) {
		std::cerr << "Failed to read EXT4 superblock." << std::endl;
		return;
	}

	if (superblock.s_magic != 0xEF53) {
		std::cerr << "Invalid EXT4 magic number: " << std::hex << superblock.s_magic << std::dec << std::endl;
		return;
	}
}