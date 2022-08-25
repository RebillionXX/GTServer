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
    enum ePlayerBits {
        PLAYER_BIT_LOGGED_ON = (1 << 0),
        PLAYER_BIT_IS_IN = (1 << 1),
        PLAYER_BIT_UPDATING_ITEMS = (1 << 2),
        PLAYER_BIT_UPDATING_TRIBUTE = (1 << 3),
        PLAYER_BIT_IS_FACING_LEFT = (1 << 4)
    };
}