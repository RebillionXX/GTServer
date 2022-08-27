#pragma once

namespace GTServer {
    enum {
        PLATFORM_ID_UNKNOWN = -1,
        PLATFORM_ID_WINDOWS,
        PLATFORM_ID_IOS,
        PLATFORM_ID_OSX,
        PLATFORM_ID_LINUX,
        PLATFORM_ID_ANDROID,
        PLATFORM_ID_WINDOWS_MOBILE,
        PLATFORM_ID_WEBOS,
        PLATFORM_ID_BBX,
        PLATFORM_ID_FLASH,
        PLATFORM_ID_HTML5
    };
    enum ePlayerFlags {
        PLAYER_FLAG_LOGGED_ON = (1 << 0),
        PLAYER_FLAG_IS_IN = (1 << 1),
        PLAYER_FLAG_UPDATING_ITEMS = (1 << 2),
        PLAYER_FLAG_UPDATING_TRIBUTE = (1 << 3),
        PLAYER_FLAG_IS_FACING_LEFT = (1 << 4),
        PLAYER_FLAG_IS_INVISIBLE = (1 << 5),
        PLAYER_FLAG_IS_MOD = (1 << 6),
        PLAYER_FLAG_IS_SUPER_MOD = (1 << 7)
    };
}