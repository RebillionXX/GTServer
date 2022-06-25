#ifndef UTILS__TEXT_H
#define UTILS__TEXT_H
#include <cctype>
#include <regex>
#include <string_view>

namespace utils::text {
    inline bool is_valid_email(const std::string& val) {
        const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        return std::regex_match(val, pattern); //https://www.codespeedy.com/program-to-validate-email-id-using-regex-in-cpp/
    }
    inline bool is_valid_discord(const std::string& val) {
        const std::regex pattern("(\\w+)#(\\d)+");
        return std::regex_match(val, pattern); //took example from is_valid_email(string)
    }
    constexpr bool to_lowercase(std::string& str) {
        if (str.empty()) 
            return false;

        for (auto& ch : str) {
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                ch = std::tolower(ch);
            else if (!(ch >= '0' && ch <= '9'))
                return false;
        }
        return true;
    }
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