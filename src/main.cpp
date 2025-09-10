#include <filesystem>
#include <fstream>
#include <iostream>

#define DISK_LOCATION_DIR "/test_disks/"

#include "fs/ext4.hpp"
#include "fs/fsMagicNums.hpp"
#include "partTables/partitionTables.hpp"

int main() {
	// Loop thru all disks

	std::string diskDir = std::filesystem::current_path().string() + DISK_LOCATION_DIR;

	for (const auto& file : std::filesystem::directory_iterator(diskDir)) {
		std::string diskPath = file.path().string();

		std::cout << "----------------------------------------" << std::endl;
		std::cout << "Processing disk: " << diskPath << std::endl;

		std::ifstream f(diskPath, std::ios::binary);
		if (!f.is_open()) {
			std::cerr << "Failed to open disk file: " << diskPath << std::endl;
			continue;
		}

		// Read the first 1024 bytes (to cover both MBR and GPT)
		f.seekg(0, std::ios::beg);
		char buffer[1024];

		f.read(buffer, sizeof(buffer));
		size_t bytesRead = f.gcount();

		PartitionScheme scheme = detectPartitionScheme(buffer, bytesRead);
		std::unique_ptr<PartitionTable> table;

		switch (scheme) {
		case PARTITION_SCHEME_MBR: {
			std::cout << "Detected Partition Scheme: MBR" << std::endl;

			try {
				table = std::make_unique<MBRPartitionTable>(buffer, bytesRead);
			} catch (const std::exception& e) {
				std::cerr << "Error reading MBR: " << e.what() << std::endl;
			}

			break;
		}
		case PARTITION_SCHEME_GPT: {
			std::cout << "Detected Partition Scheme: GPT" << std::endl;

			try {
				table = std::make_unique<GPTPartitionTable>(f);
			} catch (const std::exception& e) {
				std::cerr << "Error reading GPT: " << e.what() << std::endl;
			}

			break;
		}
		default: {
			std::cout << "Detected Partition Scheme: UNKNOWN" << std::endl;
			continue;
			break;
		}
		}

		std::cout << "----------------------------------------" << std::endl;

		// Identify filesystems on each partition
		for (const auto& part : table->getPartitions()) {
			std::cout << "Identifying filesystem for partition:" << std::endl;
			part->printInfo();

			std::streamoff partOffset = part->startSector * 512;
			std::string fsType = identifyFilesystem(f, partOffset);

			std::cout << "Detected Filesystem: " << fsType << std::endl;

			if (fsType == "EXT4") {
				FS_EXT4 ext4fs(f, partOffset);
				std::cout << "Inodes: " << ext4fs.superblock.s_free_inodes_count << "/"
						  << ext4fs.superblock.s_inodes_count << std::endl;

				std::cout << "Blocks: " << ext4fs.superblock.s_free_blocks_count_lo << "/"
						  << ext4fs.superblock.s_blocks_count_lo << std::endl;

				std::cout << "Block size: " << (1024U << ext4fs.superblock.s_log_block_size) << " bytes" << std::endl;

				std::cout << "Inode size: " << ext4fs.superblock.s_inode_size << " bytes" << std::endl;
			}

			std::cout << "----------------------------------------" << std::endl;
		}

		// Cleanup
		f.close();
	}

	return 0;
}