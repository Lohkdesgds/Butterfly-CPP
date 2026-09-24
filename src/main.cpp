#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <cstdlib>

int main() {
    std::cout << "Starting bot..." << std::endl;
    const auto token = std::getenv("BOT_TOKEN");
    if (!token) {
        std::cout << "Invalid token. Set env variable BOT_TOKEN and try again." << std::endl;
        return 1;
    }

	dpp::cluster bot(token);

    bot.set_websocket_protocol(dpp::ws_etf);

    std::cout << "Setting up log hook..." << std::endl;
    bot.on_log(dpp::utility::cout_logger());

    std::cout << "Setting up slash command hook..." << std::endl;
	bot.on_slashcommand([](auto event) {
		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}
	});

    std::cout << "Setting up guild create hook..." << std::endl;
    bot.on_guild_create([&bot](dpp::guild_create_t event) {
        std::cout << "Joined server " << event.created.name << std::endl;
    });

    std::cout << "Setting up on ready hook..." << std::endl;
	bot.on_ready([&bot](dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(
				dpp::slashcommand("ping", "Ping pong!", bot.me.id)
			);
		}
	});

    std::cout << "Start!" << std::endl;
	bot.start(dpp::st_wait);
	return 0;
}