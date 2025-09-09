#include <fstream>
#include <iostream>
#include <filesystem>

#define DISK_LOCATION_DIR "/test_disks/"

#include "partitionSchemes.hpp"

int main() {
    // Loop thru all disks

    std::string diskDir = std::filesystem::current_path().string() + DISK_LOCATION_DIR;

    for (const auto &file: std::filesystem::directory_iterator(diskDir)) {
        std::string diskPath = file.path().string();
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
        f.close();

        PartitionScheme scheme = detectPartitionScheme(buffer, bytesRead);
        switch (scheme) {
            case PARTITION_SCHEME_MBR:
                std::cout << "Detected Partition Scheme: MBR" << std::endl;
                break;
            case PARTITION_SCHEME_GPT:
                std::cout << "Detected Partition Scheme: GPT" << std::endl;
                break;
            default:
                std::cout << "Detected Partition Scheme: UNKNOWN" << std::endl;
                break;
        }

        std::cout << "----------------------------------------" << std::endl;
    }

}