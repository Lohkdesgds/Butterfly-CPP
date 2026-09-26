#include "configurator/HandleConfiguration.hpp"

#include <format>

HandleConfiguration::HandleConfiguration(dpp::cluster& ref, GuildConfiguration& conf)
    : m_ref(ref), m_conf(conf)
{
    m_ref.log(dpp::ll_debug, "HandleConfiguration has been successfully created.");
}


dpp::task<std::optional<dpp::snowflake>> HandleConfiguration::create_channel(dpp::snowflake guild_id) {
    dpp::confirmation_callback_t res = co_await m_ref.co_channel_create(dpp::channel()
        .set_name(configuration_channel_name)
        .set_type(dpp::CHANNEL_TEXT)
        .set_guild_id(guild_id)
        .set_topic(
            "This channel is used by Butterfly Bot as storage.\n"
            "If you deny access to it to this channel, it will create another one.\n"
            "All guild and user information may be stored here.\n"
            "User information may not be as up to date.\n"
            "To make the bot work as intended, please, consider not removing it or this channel.\n"
            "You can hide this from others, but don't hide from the bot itself."
        ));


    if (res.is_error()) {
        m_ref.log(dpp::ll_error, std::format("Fatal error on guild {}. Cannot load data!",
            (uint64_t)guild_id));
        co_return std::optional<dpp::snowflake>{};
    }

    dpp::channel ch = res.get<dpp::channel>();

    m_ref.log(dpp::ll_debug, std::format("New setup done at guild {}. Created config channel {}.",
        (uint64_t)guild_id,
        (uint64_t)ch.id
    ));

    res = co_await m_ref.co_message_create(dpp::message()
        .set_content(
            "Hey! This channel was created to store configurations on your server.\n"
            "Please do not remove access of it from me (the bot)!\n\n"
            "More questions, please read the topic of the channel! Thanks."
        )
        .set_channel_id(ch.id)
        .set_guild_id(guild_id));
        
    if (res.is_error()) {
        m_ref.log(dpp::ll_error, std::format("Error writing first message on guild {}, channel {}.",
            (uint64_t)guild_id,
            (uint64_t)ch.id
        ));
    }

    co_return ch.id;
}

dpp::task<void> HandleConfiguration::setup_for(dpp::guild& guild) {
    dpp::channel* channel{};

    for(const auto& id : guild.channels) {
        if (channel = dpp::find_channel(id)) {
            if (channel->name != configuration_channel_name)
                channel = nullptr;
            else
                break;
        }
    }

    if (!channel) {
        std::optional<dpp::snowflake> ch = co_await this->create_channel(guild.id);
        if (!ch.has_value()) co_return;
        const dpp::snowflake ch_id = ch.value();

        channel = dpp::find_channel(ch_id);
        if (!channel) {
            m_ref.log(dpp::ll_error, std::format("Error searching back channel created on guild {}, channel {}.",
                (uint64_t)guild.id,
                (uint64_t)ch_id
            ));
            co_return;
        }
    }

    m_ref.log(dpp::ll_info, std::format("For guild {}, channel is: {} ({})",
        (uint64_t)guild.id,
        channel->name,
        (uint64_t)channel->id));

    co_await m_conf.load_from(m_ref, channel);


    /*m_ref.messages_get(channel->id, 0, 0, 0, 5, [this, id=channel->id, guild_id=guild.id](const dpp::confirmation_callback_t& res){
        if (res.is_error()) {
            m_ref.log(dpp::ll_error, std::format("Could not retrieve latest messages for channel {} on guild {}",
                (uint64_t)id, (uint64_t)guild_id));
            return;
        }

        auto message_map = res.get<dpp::message_map>();

        for (const auto& [mid, msg] : message_map) {
            m_ref.log(dpp::ll_debug, std::format("@{} {}:{} [{}]> {}: {}",
                (uint64_t)mid,
                (uint64_t)guild_id,
                (uint64_t)id,
                msg.attachments.size(),
                msg.author.username,
                msg.content
            ));
        }
    });*/

}