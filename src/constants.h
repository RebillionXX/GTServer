#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <string_view>

namespace GTServer {
    namespace constants {
        namespace http {
            constexpr std::string_view address = "127.0.0.1";
            constexpr uint16_t port = 17091;
        };
        namespace mysql {
            const std::string& host = "tcp://127.0.0.1:3306";
            const std::string& username = "root";
            const std::string& password = "";
            const std::string& schema = "gtserver";
        };
        namespace cdn {
            constexpr std::basic_string url = "ubistatic-a.akamihd.net";
            constexpr std::basic_string cache = "0098/XXXX/cache/";
        }
        namespace svr {
            constexpr std::string_view name = "Buildo";
            constexpr std::string_view discord = "https://discord.gg/wHVSrHKbYH";
            constexpr std::_String_val version = "0.0.1";
        }
    };
}

#endif // CONSTANTS_H