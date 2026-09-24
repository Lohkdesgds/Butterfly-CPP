#include <dpp/dpp.h>
#include <cstdlib>

int main() {
	dpp::cluster bot(std::getenv("BOT_TOKEN"));

    bot.set_websocket_protocol(dpp::ws_etf);

    bot.on_log([](const dpp::log_t& log) {
        switch(log.severity) {
        case dpp::loglevel::ll_info:
            std::cout << "[I] " << log.message << std::endl;
            break;
        case dpp::loglevel::ll_warning:
            std::cout << "[W] " << log.message << std::endl;
            break;
        case dpp::loglevel::ll_error:
            std::cout << "[E] " << log.message << std::endl;
            break;
        case dpp::loglevel::ll_critical:
            std::cout << "[C] " << log.message << std::endl;
            break;
        }
    });

	bot.on_slashcommand([](auto event) {
		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}
	});

    bot.on_guild_create([&bot](dpp::guild_create_t event) {
        std::cout << "Joined server " << event.created.name << std::endl;
    });

	bot.on_ready([&bot](dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(
				dpp::slashcommand("ping", "Ping pong!", bot.me.id)
			);
		}
	});

	bot.start(dpp::st_wait);
	return 0;
}