#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <cstdlib>

#include "configurator/ConfigurationFile.hpp"
#include "configurator/HandleConfiguration.hpp"

int main() {
    std::cout << "Starting bot..." << std::endl;
    const auto token = std::getenv("BOT_TOKEN");
    if (!token) {
        std::cout << "Invalid token. Set env variable BOT_TOKEN and try again." << std::endl;
        return 1;
    }

	dpp::cluster bot(token);
    GuildConfiguration configs;
    HandleConfiguration config(bot, configs);

    bot.set_websocket_protocol(dpp::ws_etf);

    std::cout << "Setting up log hook..." << std::endl;
    bot.on_log(dpp::utility::cout_logger());

    std::cout << "Setting up slash command hook..." << std::endl;
	bot.on_slashcommand([&bot,&configs](const dpp::slashcommand_t event) -> dpp::task<void> {
		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}
        if (event.command.get_command_name() == "force-save") {
            event.reply("Working...");
            const bool res = co_await configs.save_guild_by_id(bot, event.command.guild_id);
            if (res) {
                event.edit_response("Success!");
            } else {
                event.edit_response("Failed! Check permissions, maybe?");
            }
        }
	});

    std::cout << "Setting up guild create hook..." << std::endl;
    bot.on_guild_create([&bot,&config](dpp::guild_create_t event) -> dpp::task<void> {
        std::cout << "Joined server " << event.created.name << std::endl;
        co_await config.setup_for(event.created);
    });

    std::cout << "Setting up on ready hook..." << std::endl;
	bot.on_ready([&bot](dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(
				dpp::slashcommand("ping", "Ping pong!", bot.me.id)
			);
            bot.global_command_create(
				dpp::slashcommand("force-save", "Stores latest config in guild", bot.me.id)
			);
		}
	});

    std::cout << "Start!" << std::endl;
	bot.start(dpp::st_wait);
	return 0;
}