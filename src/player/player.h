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
        void send_dialog(const eDialogType& type, text_scanner parser) {
            switch (type) {
                case DIALOG_TYPE_REGISTRATION: {
                    DialogBuilder db{};
                    db.set_default_color('o')
                        ->add_label_with_icon("`wGTServer V0.0.1``", 0, DialogBuilder::LEFT, DialogBuilder::BIG)
                        ->add_spacer();
                    if (parser.contain("extra"))
                        db.add_textbox(parser.get("extra"))->add_spacer();
                    db.add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any device. Your`` name`` will be shown to other players!")
                        ->add_text_input("name", "GrowID:", parser.get("name"), 18)
                        ->add_spacer()
                        ->add_textbox("Your `wpassword`` must contain`` 8 to 18 characters, 1 letter, 1 number`` and`` 1 special character: @#!$^&*.,``")
                        ->add_text_input_password("password", "Password:", parser.get("password"), 18)
                        ->add_text_input_password("verify_password", "Verify Password:", parser.get("verify_password"), 18)
                        ->add_spacer()
                        ->add_textbox("Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.")
                        ->add_text_input("email", "Email:", parser.get("email"), 25)
                        ->add_textbox("Your `wDiscord ID `owill be used for secondary verification if you lost access to your `wemail address`o! Please enter in such format: `wdiscordname#tag`o. Your `wDiscord Tag `ocan be found in your `wDiscord account settings`o.")
                        ->add_text_input("discord", "Discord:", parser.get("discord"), 25)
                        ->add_textbox("We will never ask you for your password, email or discord, never share it with anyone!")
                        ->add_spacer()
                        ->end_dialog("growid", "Disconnect", "Create!");
                    v_sender.OnDialogRequest(db.get(), 0);
                    break;
                }
                default:
                    break;
            }
        }
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
    };
}