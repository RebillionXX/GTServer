#pragma once
#include <cctype>
#include <regex>
#include <string_view>

namespace utils {
    inline bool is_valid_email(const std::string& val) {
        const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
        return std::regex_match(val, pattern); //https://www.codespeedy.com/program-to-validate-email-id-using-regex-in-cpp/
    }
    inline bool is_valid_discord(const std::string& val) {
        const std::regex pattern("(\\w+)#(\\d)+");
        return std::regex_match(val, pattern); //took example from is_valid_email(string)
    }
    inline constexpr bool to_lowercase(std::string& str) {
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
    inline std::vector<std::string> split (std::string s, std::string delimiter) {
        std::size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back (s.substr(pos_start));
        return res;
    }
    inline constexpr uint32_t quick_hash(const std::string_view& data) {
        uint32_t hash = 5381;
        for (const auto& c : data)
            hash = ((hash << 5) + hash) + c;
        return hash;
    }
}

namespace GTServer {
    constexpr uint32_t operator "" _qh(const char* str, std::size_t len) {
        return utils::quick_hash(std::string_view{ str, len });
    }
}