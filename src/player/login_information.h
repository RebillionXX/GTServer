#pragma once
#include <string>

namespace GTServer {
    class LoginInformation
    {
    public:
        LoginInformation() = default;
        ~LoginInformation() = default;
        
    public:
        std::string m_fz{};
        std::string m_wk{};
        std::string m_zf{};
        std::string m_tr{};
        std::string m_aid{};
        std::string m_vid{};
        std::string m_gid{};
        std::string m_mac{};
    };
}