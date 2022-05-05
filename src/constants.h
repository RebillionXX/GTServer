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
    };
}

#endif // CONSTANTS_H