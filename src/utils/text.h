#ifndef UTILS__TEXT_H
#define UTILS__TEXT_H
#include <string_view>

namespace utils::text {
    constexpr uint32_t quick_hash(const std::string_view& data) {
        uint32_t hash = 5381;
        for (const auto& c : data)
            hash = ((hash << 5) + hash) + c;
        return hash;
    }

    constexpr uint32_t operator "" _qh(const char* str, std::size_t len) {
        return utils::text::quick_hash(std::string_view{ str, len });
    }

    constexpr uint32_t hash_data(const void* src, uintmax_t length) {
        if (!src) //reb tell me if its wrong :D im kinda new in those shits (the hash is for items.dat decoding part)
           return 0;
        uint32_t hashed = 0x55555555;
        for (uintmax_t payload = 0; payload < length; ++payload)
            hashed = (hashed >> 27) + (hashed << 5) + static_cast<const uint8_t*>(src)[payload];
        return hashed;
    }

}

#endif // UTILS__TEXT_H