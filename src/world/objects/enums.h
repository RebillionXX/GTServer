#pragma once
#include <unordered_map>

namespace GTServer {
    enum eWorldCategory {
        WORLD_CATEGORY_DEFAULT,
        WORLD_CATEGORY_TOP_WORLDS,
        WORLD_CATEGORY_RANDOM,
        WORLD_CATEGORY_YOUR_WORLDS,
        WORLD_CATEGORY_ADVENTURE,
        WORLD_CATEGORY_ART,
        WORLD_CATEGORY_FARM,
        WORLD_CATEGORY_GAME,
        WORLD_CATEGORY_GUILD,
        WORLD_CATEGORY_INFORMATION,
        WORLD_CATEGORY_MUSIC,
        WORLD_CATEGORY_PARKOUR,
        WORLD_CATEGORY_PUZZLE,
        WORLD_CATEGORY_ROLEPLAY,
        WORLD_CATEGORY_SHOP,
        WORLD_CATEGORY_SOCIAL,
        WORLD_CATEGORY_STORAGE,
        WORLD_CATEGORY_STORY,
        WORLD_CATEGORY_TRADE
    };
    
    static std::unordered_map<eWorldCategory, std::string> world_category_as_string = {
        { WORLD_CATEGORY_DEFAULT, "Default" },
        { WORLD_CATEGORY_TOP_WORLDS, "Top Worlds" },
        { WORLD_CATEGORY_RANDOM, "Random" },
        { WORLD_CATEGORY_YOUR_WORLDS, "Your Worlds" },
        { WORLD_CATEGORY_ADVENTURE, "Adventure" },
        { WORLD_CATEGORY_ART, "Art" },
        { WORLD_CATEGORY_FARM, "Farm" },
        { WORLD_CATEGORY_GAME, "Game" },
        { WORLD_CATEGORY_GUILD, "Guild" },
        { WORLD_CATEGORY_INFORMATION, "Information" },
        { WORLD_CATEGORY_MUSIC, "Music" },
        { WORLD_CATEGORY_PARKOUR, "Parkour" },
        { WORLD_CATEGORY_PUZZLE, "Puzzle" },
        { WORLD_CATEGORY_ROLEPLAY, "Roleplay" },
        { WORLD_CATEGORY_SHOP, "Shop" },
        { WORLD_CATEGORY_SOCIAL, "Social" },
        { WORLD_CATEGORY_STORAGE, "Storage" },
        { WORLD_CATEGORY_STORY, "Story" },
        { WORLD_CATEGORY_TRADE, "Trade" }
    };
}