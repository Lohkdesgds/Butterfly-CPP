#pragma once

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>

class GuildConfiguration {
public:
    dpp::task<void> load_from(dpp::cluster& ref, dpp::channel* channel);

    dpp::task<bool> save_guild_by_id(dpp::cluster& ref, const dpp::snowflake& guild_id);
private:

    // semi shared user data
    struct user_info {
        double last_message_creation_time{};
        uint64_t local_points = 0;
        uint64_t global_points = 0;
    };

    // A guild may contain only its data and their user's data.
    // The latest guild to have the most up to date data of the members set the global users data.
    struct guild_data {
        dpp::channel* store_channel{};
        double last_message_creation_time{};
        std::list<dpp::snowflake> registered_members;
    };

    struct stored_data {
        std::unordered_map<dpp::snowflake, user_info> users; // semi global list
        std::unordered_map<dpp::snowflake, guild_data> guilds; // per guild list
    };

    stored_data m_data;
};