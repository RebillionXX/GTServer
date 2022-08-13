#ifndef CONFIGS_H
#define CONFIGS_H
#include <string>
#include <string_view>

#define HTTP_SERVER

namespace GTServer {
    namespace constants {
        namespace http {
            constexpr std::string_view address = "127.0.0.1";
            constexpr uint16_t port = 17091;
        }
        namespace mysql {
            inline const std::string& host = "tcp://127.0.0.1:3306";
            inline const std::string& username = "root";
            inline const std::string& password = "";
            inline const std::string& schema = "gtserver";
        }
        namespace settings {
            constexpr std::string_view server_name = "GTServer";
            constexpr std::string_view discord_server = "https://discord.gg/wHVSrHKbYH";
        }
    }
}

#endif // CONFIGS_H