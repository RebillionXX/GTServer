#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <string_view>

namespace GTServer {
    namespace constants {
        namespace http {
            constexpr std::string_view address = "127.0.0.1";
            constexpr uint16_t port = 17091;
            constexpr uint16_t http_port = 80;
        };
        namespace mysql {
            const std::string& host = "tcp://127.0.0.1:3306";
            const std::string& username = "root";
            const std::string& password = "";
            const std::string& schema = "gtserver";
        };
        namespace settings {
            const std::string& svr_name = "GTserver";
            const std::string& svr_discord = "https://discord.gg/wHVSrHKbYH";
        }
    };
}

#endif // CONSTANTS_H