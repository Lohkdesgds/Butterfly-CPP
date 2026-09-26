#include "configurator/ConfigurationFile.hpp"

dpp::task<void> GuildConfiguration::load_from(dpp::cluster& ref, dpp::channel* channel) {
    if (!channel) co_return;

    auto res = co_await ref.co_messages_get(channel->id, 0, 0, 0, 1);
    
    if (res.is_error()) {
        ref.log(dpp::ll_error, std::format("Could not retrieve latest messages for channel {} on guild {}",
            (uint64_t)channel->id, (uint64_t)channel->guild_id));
        co_return;
    }

    dpp::message_map message_map = res.get<dpp::message_map>();

    if (message_map.size() == 0) {
        // insert empty config
        m_data.guilds.insert({ channel->guild_id, guild_data{ .store_channel = channel } });
        co_return;
    }

    dpp::message msg = message_map.begin()->second;

    if (msg.attachments.size() == 0) {// insert empty config too
        m_data.guilds.insert({ channel->guild_id, guild_data{ .store_channel = channel } });
        co_return;
    }

    msg.attachments[0].download([this, channel, &ref](const dpp::http_request_completion_t& res){
        if (res.error != dpp::h_success) {
            ref.log(dpp::ll_error, std::format("Could not retrieve data from message in channel {} on guild {}",
                (uint64_t)channel->id, (uint64_t)channel->guild_id));
            return;
        }

        auto j = nlohmann::json::parse(res.body);

        m_data.guilds.insert({ channel->guild_id, guild_data{
            .store_channel = channel,
            .last_message_creation_time = j["gdat"]["last_msg_t"],
            .registered_members = j["gdat"]["members"]
        } });
    });
}

dpp::task<bool> GuildConfiguration::save_guild_by_id(dpp::cluster& ref, const dpp::snowflake& guild_id) {
    const auto& i = m_data.guilds[guild_id];
    if (i.store_channel == nullptr || i.store_channel->guild_id != guild_id) {
        ref.log(dpp::ll_error, std::format("Could not save guild {} data on itself: invalid channel or mismatch?",
            (uint64_t)guild_id));
        co_return false;
    }

    nlohmann::json j;
    j["gdat"]["last_msg_t"] = i.last_message_creation_time;
    j["gdat"]["members"] = i.registered_members;

    const std::string dumped = j.dump();

    dpp::confirmation_callback_t res = co_await ref.co_message_create(dpp::message()
        .set_content("Configuration file (DO NOT DELETE)")
        .add_file("config.json", dumped, "application/json")
        .set_channel_id(i.store_channel->id)
        .set_guild_id(guild_id));

    if (res.is_error()) {
        ref.log(dpp::ll_error, std::format("Could not create message to save guild {} data.",
            (uint64_t)guild_id));
        co_return false;
    }

    ref.log(dpp::ll_info, std::format("Guild {} saved itself.",
        (uint64_t)guild_id));

    co_return true;
}