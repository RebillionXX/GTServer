#pragma once
#include <atomic>
#include <format>
#include <string>
#include <enet/enet.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <player/login_information.h>
#include <server/server.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    class Player {
    public:
        enum : int32_t {
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
    public:
        explicit Player(ENetPeer* peer);
        ~Player() = default;

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }

        void disconnect(const enet_uint32& data) {
            enet_peer_disconnect_later(this->get_peer(), data);
        }

        bool update_internet_protocol() {
            return enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16) < 0 ? false : true;
        }

        void send_packet(TankUpdatePacket tank_packet, uintmax_t data_size);
        void send_packet(TankUpdatePacket* tank_packet, uintmax_t data_size);
        void send_packet(int32_t type, const void* data, uintmax_t data_size);
        void send_variant(const variantlist_t& var, int32_t delay = 0, int32_t net_id = -1);
        
        template <typename... Args>
        void send_log(const std::string& format, Args&&... args) {
            const auto& data = fmt::format("action|log\nmsg|{}", std::vformat(format, std::make_format_args(args...)));
            this->send_packet(NET_MESSAGE_GAME_MESSAGE, data.data(), data.size());
        }

        [[nodiscord]] std::shared_ptr<LoginInformation> get_login_info() { return m_login_info; }
    public:
        enum class dialog_type {
            REGISTRATION
        };
        void send_dialog(const dialog_type& type, text_scanner parser) {
            using namespace proton::utils;
            switch (type) {
                case dialog_type::REGISTRATION: {
                    dialog_builder db{};
                    db.set_default_color('o')
                        ->add_label_with_icon("`wGTServer V0.0.1``", 0, dialog_builder::LEFT, dialog_builder::BIG)
                        ->add_spacer();
                    if (parser.contain("extra"))
                        db.add_textbox(parser.get("extra"))->add_spacer();
                    db.add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any devide. Your`` name`` will be shown to other players!")
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
                    this->send_variant({ "OnDialogRequest", db.get() });
                    break;
                }
                default:
                    break;
            }
        }
    public:
        int32_t m_platform{ PLATFORM_ID_UNKNOWN };
        std::shared_ptr<LoginInformation> m_login_info;

        std::atomic<bool> m_logged_on;
    private:
        ENetPeer* m_peer;

        std::string m_ip_address;
    };
}