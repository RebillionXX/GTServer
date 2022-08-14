#pragma once
#include <string>
#include <player/player.h>

namespace GTServer {
    class WindowsPlayer
    {
    public:
        std::string m_fz;
        std::string m_mac;
        std::string m_wk;
        std::string m_zf;
    };

    class IOSPlayer
    {
    public:
        std::string m_aid;
        std::string m_vid;
        std::string m_mac;
    };

    class AndroidPlayer
    {
    public:
        std::string m_gid;
        std::string m_tr;
    };
}