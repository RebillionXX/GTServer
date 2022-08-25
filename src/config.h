#pragma once
#include <string>
#include <string_view>

#define SERVER_VERSION "0.1.0"
#define HTTP_SERVER

namespace GTServer {
    namespace config {
        namespace http {
            constexpr std::string_view address = "0.0.0.0";
            constexpr uint16_t port = 443;
            namespace gt {
                constexpr std::string_view address = "51.141.10.170";
            }
        }
        namespace server_default {
            constexpr std::string_view address = "127.0.0.1";
            constexpr uint16_t port = 17091;
        }
        namespace database {
            inline const std::string& host = "127.0.0.1";
            inline const uint16_t& port = 3306;
            inline const std::string& user = "root";
            inline const std::string& password = "";
            inline const std::string& database = "gtserver";
            inline const bool& auto_reconnect{ true };
            inline const bool& debug{ false };
        }
        namespace settings {
            constexpr std::string_view server_name = "GTServer";
            constexpr std::string_view discord_server = "https://discord.gg/wHVSrHKbYH";
        }
    }
}