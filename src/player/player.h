#ifndef PLAYER__PLAYER_H
#define PLAYER__PLAYER_H
#include <string>
#include <enet/enet.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <server/server.h>
#include <constants.h>
#include <database/item/item_database.h>

namespace GTServer {
    class NetAvatar {
    public:
        NetAvatar(ENetPeer* peer, ENetServer* server) 
        : m_peer(peer), m_server(server) {
            if(!m_peer)
                return;
            m_peer->data = this;

            m_ip_address.reserve(16);
            enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
        }
        ~NetAvatar() = default;

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] int get_net() const { return m_net; }

        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }

        void send(TankUpdatePacket tank_packet, uintmax_t data_size) {
            if (!this->get_peer())
                return;
            ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet)
                return;
            std::memcpy(packet->data, &tank_packet, data_size);
            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send(TankUpdatePacket* tank_packet, uintmax_t data_size) {
            if (!this->get_peer())
                return;
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(&tank_packet->data); 
            ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet || !update_packet)
                return;
            std::memcpy(packet->data, &tank_packet->type, 4);
            //DEBUG
            std::vector<char> array;
            for(auto i = 0; i < update_packet->data_size; i++)
                array.push_back(static_cast<char>(reinterpret_cast<uint8_t*>(&update_packet->data)[i]));
            fmt::print("array: {}\n", array);
            std::memcpy(packet->data + 4, update_packet, sizeof(GameUpdatePacket) + update_packet->data_size);

            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send_var(const variantlist_t& var, int32_t delay = 0, int32_t net_id = -1) {
            size_t alloc = 1;
            for(const auto& v : var.get_objects())
                alloc += v.get_memory_allocate() + 1;
            const uint8_t* var_data = var.serialize();

            TankUpdatePacket* tank_packet = (TankUpdatePacket*)std::malloc(sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + alloc);
            std::memset(tank_packet, 0, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + alloc);
            tank_packet->type = NET_MESSAGE_GAME_PACKET;
            tank_packet->data = (char*)std::malloc(sizeof(GameUpdatePacket) + alloc);
            
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(&tank_packet->data);
            update_packet->type = NET_GAME_PACKET_CALL_FUNCTION;
            update_packet->net_id = net_id;
            update_packet->delay = delay;
            update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
            update_packet->data_size = alloc;
            std::memcpy(static_cast<uint8_t*>(&update_packet->data), var_data, alloc);

            this->send(tank_packet, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + update_packet->data_size);
            free(tank_packet);
        }
        void send_log(std::string arg) {
            this->send_var({ "OnConsoleMessage", arg });
        }
        void send_logon_attempt() {
            this->send_var({ "OnSuperMainStartAcceptLogonHrdxs47254722215a",
                0,
                std::string(constants::cdn::url),
                std::string(constants::cdn::cache),
                "cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
                "proto=161|choosemusic=audio/mp3/cumbia.mp3|active_holiday=5|wing_week_day=0|ubi_week_day=0|server_tick=735463|clash_active=0|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|",
                2357275721 });
        }
    public:
        std::string m_ip_address;
        
        std::string m_tank_id_name;
        std::string m_tank_id_pass;
    private:

        int m_net;
        ENetPeer* m_peer;
        ENetServer* m_server;
    };
}

#endif // PLAYER__PLAYER_H