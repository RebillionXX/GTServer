#pragma once 
#include <enet/enet.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <player/objects/packet_sender.h>

namespace GTServer {
    class VariantListSender : public PacketSender{
    public:
        VariantListSender(ENetPeer* peer) : 
            m_peer(peer),
            PacketSender{ peer } {
        }
        ~VariantListSender() = default;

        void OnSuperMainStart(
            const uint32_t& items_dat, 
            const std::string& cdn_host, 
            const std::string& cdn_cache, 
            const std::string& blocked, 
            const std::string& settings, 
            const uint32_t& player_tribute)
        {
            this->send_varlist({ 
                "OnSuperMainStartAcceptLogonHrdxs47254722215a",
                items_dat,
                cdn_host,
                cdn_cache,
                blocked,
                settings,
                player_tribute
            });
        }
        void OnDialogRequest(const std::string& data, int32_t delay = 0) {
            this->send_varlist({
                "OnDialogRequest",
                data
            }, delay);
        }
        void OnRequestWorldSelectMenu(const std::string& data, int32_t delay = 0) {
            this->send_varlist({
                "OnRequestWorldSelectMenu",
                data
            }, delay);
        }
        void OnFailedToEnterWorld(const bool& failed) {
            this->send_varlist({
                "OnFailedToEnterWorld",
                failed ? 1 : 0
            });
        }
        void OnSpawn(const std::string& data) {
            this->send_varlist({
                "OnSpawn",
                data
            });
        }
    private:
        ENetPeer* m_peer;
    };
}