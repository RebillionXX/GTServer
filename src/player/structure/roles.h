#pragma once
#include <string>
#include <unordered_map>

namespace GTServer {
    enum ePlayerRole {
        PLAYER_ROLE_DEFAULT,
        PLAYER_ROLE_VIP,
        PLAYER_ROLE_VIP_PLUS,
        PLAYER_ROLE_MODERATOR,
        PLAYER_ROLE_ADMINISTRATOR,
        PLAYER_ROLE_MANAGER,
        PLAYER_ROLE_DEVELOPER
    };

    static std::unordered_map<int, char> role_color = {
        { PLAYER_ROLE_DEFAULT, 'w' },
        { PLAYER_ROLE_VIP, '1' }, 
        { PLAYER_ROLE_VIP_PLUS, '4' }, 
        { PLAYER_ROLE_MODERATOR, '#' },
        { PLAYER_ROLE_ADMINISTRATOR, '9' },
        { PLAYER_ROLE_MANAGER, '6' },
        { PLAYER_ROLE_DEVELOPER, '6' }
    };
}