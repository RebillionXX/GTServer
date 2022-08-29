#pragma once 
#include <enet/enet.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <proton/utils/text_scanner.h>
#include <utils/color.h>
#include <player/objects/enums.h>
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
        void OnConsoleMessage(const std::string& message, uint32_t delay = 0) {
            this->send_varlist({ 
                "OnConsoleMessage",
                message
            }, delay);
        }
        void OnRequestWorldSelectMenu(const std::string& data, int32_t delay = 0) {
            this->send_varlist({
                "OnRequestWorldSelectMenu",
                data
            }, delay);
        }
        void OnFailedToEnterWorld(const bool& val) {
            this->send_varlist({
                "OnFailedToEnterWorld",
                val ? 1 : 0
            });
        }
        void OnSpawn(const TextScanner& data) {
            this->send_varlist({
                "OnSpawn",
                data.get_all_raw()
            }, -1);
        }
        void OnSetClothing(std::array<uint16_t, NUM_BODY_PARTS> cloth, const Color& color, const bool& sound, const uint32_t& net_id, const int32_t& delay = 0) {
            this->send_varlist({
                "OnSetClothing",
                CL_Vec3f{ cloth[CLOTHTYPE_HAIR], cloth[CLOTHTYPE_SHIRT], cloth[CLOTHTYPE_PANTS] },
                CL_Vec3f{ cloth[CLOTHTYPE_FEET], cloth[CLOTHTYPE_FACE], cloth[CLOTHTYPE_HAND] },
                CL_Vec3f{ cloth[CLOTHTYPE_BACK], cloth[CLOTHTYPE_MASK], cloth[CLOTHTYPE_NECKLACE] },
                (int)color.get_uint(),
                CL_Vec3f{ cloth[CLOTHTYPE_ANCES], sound, 0 }
            }, delay, net_id);
        }
    private:
        ENetPeer* m_peer;
    };
}