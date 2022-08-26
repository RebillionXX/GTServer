#pragma once
#include <format>
#include <string>
#include <enet/enet.h>
#include <player/objects/enums.h>
#include <player/objects/login_information.h>
#include <player/objects/roles.h>
#include <player/objects/variantlist_sender.h>
#include <server/server.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    class Player {
    public:
        explicit Player(ENetPeer* peer);
        ~Player();

        bool is_flag_on(const ePlayerFlags& bits);
        void set_flag(const ePlayerFlags& bits);
        void remove_flag(const ePlayerFlags& bits);

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }

        void set_user_id(const uint32_t& uid) { m_user_id = uid; }
        [[nodiscard]] uint32_t get_user_id() const { return m_user_id; }
        void set_raw_name(const std::string& name) { m_raw_name = name; }
        [[nodiscard]] std::string get_raw_name() const { return m_raw_name; }
        [[nodiscard]] std::string get_display_name() const { 
            char tag{}, color{ role_color[m_role] };
            if (m_role >= PLAYER_ROLE_MODERATOR)
                tag = char{ '@' };
            return fmt::format("`{}{}{}``", color, tag == char{} ? "" : std::string{ 1, tag }, m_raw_name);
        }
        void set_role(const uint32_t& role) { m_role = role; }
        [[nodiscard]] uint32_t get_role() const { return m_role; }

        void set_world(const std::string& name) { m_world = name; }
        [[nodiscard]] std::string get_world() const { return m_world; }
        void set_net_id(const uint32_t& net_id) { m_net_id = net_id; }
        [[nodiscard]] uint32_t get_net_id() const { return m_net_id; }

        void disconnect(const enet_uint32& data) {
            enet_peer_disconnect_later(this->get_peer(), data);
        }

        bool update_internet_protocol() {
            return enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16) < 0 ? false : true;
        }

        void send_packet(TankUpdatePacket tank_packet, uintmax_t data_size);
        void send_packet(TankUpdatePacket* tank_packet, uintmax_t data_size);
        void send_packet(int32_t type, const void* data, uintmax_t data_size);
        
        template <typename... Args>
        void send_log(const std::string& format, Args&&... args) {
            const auto& data = fmt::format("action|log\nmsg|{}", std::vformat(format, std::make_format_args(args...)));
            this->send_packet(NET_MESSAGE_GAME_MESSAGE, data.data(), data.size());
        }

        [[nodiscord]] std::shared_ptr<LoginInformation> get_login_info() { return m_login_info; }
    public:
        enum eDialogType {
            DIALOG_TYPE_REGISTRATION
        };
        void send_dialog(const eDialogType& type, text_scanner parser);
        
    public:
        int32_t m_platform{ PLATFORM_ID_UNKNOWN };
        std::shared_ptr<LoginInformation> m_login_info;

        VariantListSender v_sender;
    private:
        ENetPeer* m_peer;

        uint32_t m_flags{ 0 };
        uint32_t m_user_id{ 0 };
        uint8_t m_role{ 0 };

        std::string m_requested_name{};
        std::string m_tank_id_name{};
        std::string m_tank_id_pass{};
        std::string m_raw_name{};
        std::string m_display_name{};
        std::string m_ip_address{};

        std::string m_email{};
        std::string m_discord{};

        std::string m_world{ "EXIT" };
        uint32_t m_net_id{ 0 };
    };
}