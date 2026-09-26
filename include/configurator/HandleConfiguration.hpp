#pragma once

#include <dpp/dpp.h>

#include "ConfigurationFile.hpp"

// Using Discord as host for stored files
constexpr char configuration_channel_name[] = "butterflybot-config-store";

// This puts and reads config from desired channel
class HandleConfiguration {
public:
    HandleConfiguration(dpp::cluster& ref, GuildConfiguration& conf);

    dpp::task<void> setup_for(dpp::guild& guild);
private:
    dpp::task<std::optional<dpp::snowflake>> create_channel(dpp::snowflake guild_id);

    dpp::cluster& m_ref;
    GuildConfiguration& m_conf;
};