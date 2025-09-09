#include "fsMagicNums.hpp"

#include <istream>

std::string identifyFilesystem(std::istream& stream, std::streamoff baseOffset) {
    if (!stream.good()) {
        return "INVALID STREAM";
    }
    
    for (const auto& [key, fsName] : fsMagicNums) {
        stream.seekg(baseOffset + key.offset, std::ios::beg);
        if (!stream.good()) continue;

        std::string buffer(key.magic.size(), '\0');
        stream.read(&buffer[0], key.magic.size());
        if (!stream.good()) continue;

        size_t bytesRead = stream.gcount();
        if (bytesRead == key.magic.size() && buffer == key.magic) {
            return fsName;
        }
    }

    return "UNKNOWN";
}