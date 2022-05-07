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
}

#endif // UTILS__TEXT_H