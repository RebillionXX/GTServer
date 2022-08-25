#ifndef PROTON__PACKET_H
#define PROTON__PACKET_H

#include <stdint.h>
#include <enet/enet.h>

namespace GTServer {
    enum : int32_t {
        NET_MESSAGE_UNKNOWN,
        NET_MESSAGE_SERVER_HELLO,
        NET_MESSAGE_GENERIC_TEXT,
        NET_MESSAGE_GAME_MESSAGE,
        NET_MESSAGE_GAME_PACKET,
        NET_MESSAGE_ERROR,
        NET_MESSAGE_TRACK,
        NET_MESSAGE_CLIENT_LOG_REQUEST,
        NET_MESSAGE_CLIENT_LOG_RESPONSE
    };
    enum : uint8_t {
        NET_GAME_PACKET_STATE,
        NET_GAME_PACKET_CALL_FUNCTION,
        NET_GAME_PACKET_UPDATE_STATUS,
        NET_GAME_PACKET_TILE_CHANGE_REQUEST,
        NET_GAME_PACKET_SEND_MAP_DATA,
        NET_GAME_PACKET_SEND_TILE_UPDATE_DATA,
        NET_GAME_PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
        NET_GAME_PACKET_TILE_ACTIVATE_REQUEST,
        NET_GAME_PACKET_TILE_APPLY_DAMAGE,
        NET_GAME_PACKET_SEND_INVENTORY_STATE,
        NET_GAME_PACKET_ITEM_ACTIVATE_REQUEST,
        NET_GAME_PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
        NET_GAME_PACKET_SEND_TILE_TREE_STATE,
        NET_GAME_PACKET_MODIFY_ITEM_INVENTORY,
        NET_GAME_PACKET_ITEM_CHANGE_OBJECT,
        NET_GAME_PACKET_SEND_LOCK,
        NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA,
        NET_GAME_PACKET_SEND_PARTICLE_EFFECT,
        NET_GAME_PACKET_SET_ICON_STATE,
        NET_GAME_PACKET_ITEM_EFFECT,
        NET_GAME_PACKET_SET_CHARACTER_STATE,
        NET_GAME_PACKET_PING_REPLY,
        NET_GAME_PACKET_PING_REQUEST,
        NET_GAME_PACKET_GOT_PUNCHED,
        NET_GAME_PACKET_APP_CHECK_RESPONSE,
        NET_GAME_PACKET_APP_INTEGRITY_FAIL,
        NET_GAME_PACKET_DISCONNECT,
        NET_GAME_PACKET_BATTLE_JOIN,
        NET_GAME_PACKET_BATTLE_EVENT,
        NET_GAME_PACKET_USE_DOOR,
        NET_GAME_PACKET_SEND_PARENTAL,
        NET_GAME_PACKET_GONE_FISHIN,
        NET_GAME_PACKET_STEAM,
        NET_GAME_PACKET_PET_BATTLE,
        NET_GAME_PACKET_NPC,
        NET_GAME_PACKET_SPECIAL,
        NET_GAME_PACKET_SEND_PARTICLE_EFFECT_V2,
        NET_GAME_PACKET_ACTIVE_ARROW_TO_ITEM,
        NET_GAME_PACKET_SELECT_TILE_INDEX,
        NET_GAME_PACKET_SEND_PLAYER_TRIBUTE_DATA
    };
    enum : int32_t {
        NET_GAME_PACKET_FLAGS_FLYING = 1 << 1,
        NET_GAME_PACKET_FLAGS_UPDATE = 1 << 2,
        NET_GAME_PACKET_FLAGS_EXTENDED = 1 << 3,
        NET_GAME_PACKET_FLAGS_FACINGLEFT = 1 << 4
    };

    #pragma pack(push, 1)
    struct TankUpdatePacket {
        int32_t type;
        char* data;
    };
    #pragma pack(pop)
    
    #pragma pack(push, 1)
    struct GameUpdatePacket {
        uint8_t type = 0;

        union
        {
            uint8_t obj_type = 0;
            uint8_t punch_id;
            uint8_t npc_type;
        };

        union
        {
            uint8_t count1 = 0;
            uint8_t jump_count;
            uint8_t build_range;
            uint8_t npc_id;
            uint8_t lost_item_count;
        };

        union
        {
            uint8_t count2 = 0;
            uint8_t animation_type;
            uint8_t punch_range;
            uint8_t npc_action;
            uint8_t particle_id;
            uint8_t gained_item_count;
            uint8_t dice_result;
            uint8_t fruit_count;
        };

        union
        {
            int32_t net_id = 0;
            int32_t owner_id;
            int32_t effect_flags_check;
            int32_t object_change_type;
            int32_t particle_emitter_id;
        };

        union
        {
            int32_t item = 0;
            int32_t ping_hash;
            int32_t item_net_id;
            int32_t pupil_color;
            int32_t target_net_id;
            int32_t tiles_length;
        };

        int32_t flags = 0;

        union
        {
            float float_var = 0;
            float water_speed;
            float obj_alt_count;
        };

        union
        {
            int32_t int_data = 0;
            int32_t ping_item;
            int32_t elapsed_ms;
            int32_t delay;
            int32_t tile_damage;
            int32_t item_id;
            int32_t item_speed;
            int32_t effect_flags;
            int32_t object_id;
            int32_t hash;
            int32_t verify_pos;
            int32_t client_hack_type;
        };

        union
        {
            float vec_x = 0;
            float pos_x;
            float accel;
            float punch_range_in;
        };

        union
        {
            float vec_y = 0;
            float pos_y;
            float build_range_in;
            float punch_strength;
        };

        union
        {
            float vec2_x = 0;
            float dest_x;
            float gravity_in;
            float speed_out;
            float velocity_x;
            float particle_variable;
            float pos2_x;
            int hack_type;
        };

        union
        {
            float vec2_y = 0;
            float dest_y;
            float speed_in;
            float gravity_out;
            float velocity_y;
            float particle_alt_id;
            float pos2_y;
            int hack_type2;
        };

        union
        {
            float particle_rotation = 0;
            float npc_variable;
        };

        union
        {
            uint32_t int_x = 0;
            uint32_t tile_pos_x;
            uint32_t item_id_alt;
            uint32_t hair_color;
        };

        union
        {
            uint32_t int_y = 0;
            uint32_t tile_pos_y;
            uint32_t item_count;
            uint32_t eyecolor;
            uint32_t npc_speed;
            uint32_t particle_size_alt;
        };

        uint32_t data_size = 0;
        uint8_t* data;
    };
    #pragma pack(pop)
}

#endif // PROTON__PACKET_H