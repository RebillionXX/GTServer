#pragma once
#include <regex>
#include <event/event_pool.h>
#include <proton/utils/text_scanner.h>
#include <utils/random.h>
#include <player/objects/enums.h>
#include <player/objects/login_information.h>
#include <database/database.h>
#include <database/table/player_table.h>

namespace GTServer::events {
    void requested_name(EventContext& ctx) {
        if (!ctx.m_parser.try_get<int32_t>("platformID", ctx.m_player->m_platform) ||
            ctx.m_parser.get("meta", 1).empty() || 
            ctx.m_parser.get("rid", 1).empty() || 
            ctx.m_parser.get("mac", 1).empty() ||
            ctx.m_parser.get("requestedName", 1).empty() ||
            ctx.m_player->is_flag_on(PLAYER_FLAG_LOGGED_ON)) {
            ctx.m_player->disconnect(0U);
            return;
        }
        ctx.m_player->set_raw_name(ctx.m_parser.get("requestedName", 1));

        std::regex regex{ "^[a-zA-Z0-9]+$" };
        if (!std::regex_match(ctx.m_player->get_raw_name(), regex)) {
            ctx.m_player->send_log("`4Oops! `oYour name is including invalid characters, please try again.``");
            ctx.m_player->disconnect(0U);
            return;
        }
        auto login_info{ ctx.m_player->get_login_info() };
        login_info->m_mac = ctx.m_parser.get("mac", 1);
        login_info->m_rid = ctx.m_parser.get("rid", 1);

        switch (ctx.m_player->m_platform) {
            case PLATFORM_ID_WINDOWS: {
                if (!(
                    ctx.m_parser.try_get("fz", login_info->m_fz) &&
                    ctx.m_parser.try_get("wk", login_info->m_wk) &&
                    ctx.m_parser.try_get("zf", login_info->m_zf)
                )) {
                    ctx.m_player->disconnect(0U);    
                    return;
                }
                break;
            }
            case PLATFORM_ID_IOS: {
                if (!(
                    ctx.m_parser.try_get("vid", login_info->m_vid)
                )) {
                    ctx.m_player->disconnect(0U);    
                    return;
                }
                break;
            }
            default: { //todo more platform checks
                ctx.m_player->disconnect(0U);    
                return;
            }
        }
        
        ctx.m_player->set_raw_name(ctx.m_parser.get("requestedName", 1) + "_" + std::to_string(utils::random::get_generator_static().uniform(100, 999)));
        
        PlayerTable* db = (PlayerTable*)ctx.m_database->get_table(Database::DATABASE_PLAYER_TABLE);
        if (!db->load(ctx.m_player, true)) {
            uint32_t uid{ db->insert(ctx.m_player) };
            if (uid == 0) {
                ctx.m_player->disconnect(0U);
                return;
            }
            ctx.m_player->set_user_id(uid);
        }

        ctx.m_player->set_flag(PLAYER_FLAG_LOGGED_ON);
        fmt::print("{} has logged on.\n", ctx.m_player->get_raw_name());
        ctx.m_player->v_sender.OnSuperMainStart(
            ctx.m_items->get_hash(),
            "ubistatic-a.akamaihd.net", 
            "0098/75414/cache/", 
            "cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
            "proto=172|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|wing_week_day=0|ubi_week_day=0|server_tick=263203319|clash_active=0|drop_lavacheck_faster=1|isPayingUser=0|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|",
            ctx.m_database->get_tribute()->get_hash()
        );
    }
}