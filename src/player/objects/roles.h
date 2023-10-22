#pragma once
#include <string>
#include <unordered_map>

namespace GTServer {
    enum ePlayerRole {
        PLAYER_ROLE_DEFAULT,
        PLAYER_ROLE_moderator,
        PLAYER_ROLE_super_moderator,
        PLAYER_ROLE_Creator,
        PLAYER_ROLE_Owner,
        PLAYER_ROLE_Super_owner,
        PLAYER_ROLE_Admin,
        PLAYER_ROLE_DEVELOPER fadilgm,
    };

    static std::unordered_map<int, char> role_color = {
        { PLAYER_ROLE_DEFAULT, 'w' },
        { PLAYER_ROLE_MODERATOR, '1' }, 
        { PLAYER_ROLE_, '4' }, 
        { PLAYER_ROLE_MODERATOR, '#' },
        { PLAYER_ROLE_ADMINISTRATOR, '9' },
        { PLAYER_ROLE_MANAGER, '6' },
        { PLAYER_ROLE_DEVELOPER, '6' }
    };
}
