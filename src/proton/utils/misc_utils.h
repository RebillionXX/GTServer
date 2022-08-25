#pragma once
#include <stdint.h>
#include <string>

namespace proton::utils {
    inline uint32_t hash(const void* data, std::size_t data_len) {
        if (!data) return 0;
        uint32_t hash = 0x55555555;

        for (std::size_t i = 0; i < data_len; ++i)
            hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(data)[i];

        return hash;
    }
};