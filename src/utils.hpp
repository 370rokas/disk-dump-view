#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <cstdio>
#include <string>

inline uint16_t readLE16(const char* buf) {
    return static_cast<uint16_t>(
        static_cast<uint8_t>(buf[0]) |
        (static_cast<uint8_t>(buf[1]) << 8));
}

inline uint32_t readLE32(const char* buf) {
    return static_cast<uint32_t>(
        static_cast<uint8_t>(buf[0]) |
        (static_cast<uint8_t>(buf[1]) << 8) |
        (static_cast<uint8_t>(buf[2]) << 16) |
        (static_cast<uint8_t>(buf[3]) << 24));
}

inline uint64_t readLE64(const char* buf) {
    return static_cast<uint64_t>(
        static_cast<uint8_t>(buf[0])) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[1])) << 8) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[2])) << 16) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[3])) << 24) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[4])) << 32) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[5])) << 40) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[6])) << 48) |
        (static_cast<uint64_t>(static_cast<uint8_t>(buf[7])) << 56);
}

inline std::string guidToString(const char* guid) {
    char buffer[37]; // 36 characters + null terminator
    
    std::snprintf(buffer, sizeof(buffer),
                  "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                  static_cast<uint8_t>(guid[3]), static_cast<uint8_t>(guid[2]),
                  static_cast<uint8_t>(guid[1]), static_cast<uint8_t>(guid[0]),
                  static_cast<uint8_t>(guid[5]), static_cast<uint8_t>(guid[4]),
                  static_cast<uint8_t>(guid[7]), static_cast<uint8_t>(guid[6]),
                  static_cast<uint8_t>(guid[8]), static_cast<uint8_t>(guid[9]),
                  static_cast<uint8_t>(guid[10]), static_cast<uint8_t>(guid[11]),
                  static_cast<uint8_t>(guid[12]), static_cast<uint8_t>(guid[13]),
                  static_cast<uint8_t>(guid[14]), static_cast<uint8_t>(guid[15]));

    return std::string(buffer);
}

inline std::u16string utf16leToU16String(const char* data, size_t length) {
    std::u16string result;
    for (size_t i = 0; i + 1 < length; i += 2) {
        char16_t ch = static_cast<uint8_t>(data[i]) | (static_cast<uint8_t>(data[i + 1]) << 8);
        if (ch == u'\0') {
            break; // Stop at the first null character
        }
        result += ch;
    }
    return result;
}

#endif // UTILS_HPP