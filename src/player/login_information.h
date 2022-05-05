#ifndef PLAYER__LOGIN_INFORMATION_H
#define PLAYER__LOGIN_INFORMATION_H
#include <string>
#include <player/player.h>

namespace GTServer {
    class WindowsPlayer
    {
    public:
        std::string m_fz;
        std::string m_machine_hash;
        std::string m_wk;
        std::string m_zf;
    };

    class IOSPlayer
    {
    public:
        std::string m_aid;
        std::string m_vid;
        std::string m_machine_hash;
    };

    class AndroidPlayer
    {
    public:
        std::string m_gid;
        std::string m_tr;
    };
}

#endif //PLAYER__LOGIN_INFORMATION_H