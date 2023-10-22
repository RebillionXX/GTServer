#pragma once
#include <string>
#include <unordered_map>

namespace GTServer {
    enum ePlayerRole {
        PLAYER_ROLE_DEFAULT,
        PLAYER_ROLE_MODERATOR,
        PLAYER_ROLE_SUPER_MODERATOR,
        PLAYER_ROLE_OWNER,
        PLAYER_ROLE_SUPER_OWNER,
        PLAYER_ROLE_ADMIN,
        PLAYER_ROLE_DEVELOPER, FADILGM
    };

    static std::unordered_map<int, char> role_color = {
        { PLAYER_ROLE_DEFAULT, 'w' },
        { PLAYER_ROLE_MODERATOR, '1' }, 
        { PLAYER_ROLE_SUPER_MODERATOR, '4' }, 
        { PLAYER_ROLE_OWNER, '#' },
        { PLAYER_ROLE_SUPER_OWNER, '9' },
        { PLAYER_ROLE_ADMIN, '6' },
        { PLAYER_ROLE_DEVELOPER, '6' }
    };
}
