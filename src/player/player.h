#pragma once
#include <array>
#include <format>
#include <enet/enet.h>
#include <database/item/item_type.h>
#include <player/objects/enums.h>
#include <player/objects/roles.h>
#include <player/objects/character_state.h>
#include <player/objects/inventory.h>
#include <player/objects/login_information.h>
#include <player/objects/packet_sender.h>
#include <player/objects/variantlist_sender.h>
#include <server/server.h>
#include <proton/packet.h>
#include <proton/utils/common.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    class Player : public PacketSender, public CharacterState {
    public:
        explicit Player(ENetPeer* peer);
        ~Player();

        bool is_flag_on(const ePlayerFlags& bits) const;
        void set_flag(const ePlayerFlags& bits);
        void remove_flag(const ePlayerFlags& bits);

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }
        void disconnect(const enet_uint32& data) { enet_peer_disconnect_later(this->get_peer(), data); }

        void set_user_id(const uint32_t& uid) { m_user_id = uid; }
        [[nodiscard]] uint32_t get_user_id() const { return m_user_id; }
        void set_connect_id(const uint32_t& cid) { m_connect_id = cid; }
        [[nodiscard]] uint32_t get_connect_id() const { return m_connect_id; }
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
        void set_position(const int& x, const int& y) { m_position = CL_Vec2i{ x, y }; }
        [[nodiscard]] CL_Vec2i get_position() const { return m_position; }

        void set_cloth(const uint8_t& body_part, const uint16_t& id) { m_clothes[body_part] = id; }
        [[nodiscard]] std::array<uint16_t, NUM_BODY_PARTS> get_clothes() const { return m_clothes; }
        [[nodiscard]] Color get_skin_color() const { return m_skin_color; }

        TextScanner get_spawn_data(const bool& local = false) const;

        template <typename... Args>
        void send_log(const std::string& format, Args&&... args) {
            const auto& data = fmt::format("action|log\nmsg|{}", std::vformat(format, std::make_format_args(args...)));
            this->send_packet(NET_MESSAGE_GAME_MESSAGE, data.data(), data.size());
        }
        void send_character_state(std::shared_ptr<Player> player);

        [[nodiscord]] std::shared_ptr<LoginInformation> get_login_info() { return m_login_info; }
    public:
        enum eDialogType {
            DIALOG_TYPE_REGISTRATION
        };
        void send_dialog(const eDialogType& type, TextScanner parser);
        
    public:
        int32_t m_platform{ PLATFORM_ID_UNKNOWN };
        std::shared_ptr<LoginInformation> m_login_info;
        
        Inventory m_inventory;
        VariantListSender v_sender;
    private:
        ENetPeer* m_peer;

        uint32_t m_flags{ 0 };
        uint32_t m_user_id{ 0 };
        uint32_t m_connect_id{ 0 };
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
        CL_Vec2i m_position{ 0, 0 };

        std::array<uint16_t, NUM_BODY_PARTS> m_clothes{};
        Color m_skin_color = Color{ 0xB4, 0x8A, 0x78, 0xFF };
    };
}