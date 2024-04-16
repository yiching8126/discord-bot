#include "MyBot.h"
#include <dpp/dpp.h>
#include <sstream>

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the 
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */
const std::string    BOT_TOKEN    = "MTEyNDYwMjgyMjc4MTA0Njc5NQ.GS28Du.DxU7elabEXoVNDMgusYnDJHpYNGEQRTkzChfps";

int main()
{
    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        if (event.command.get_command_name() == "greeting") {
            const auto& username_param = event.get_parameter("username");
            if (std::holds_alternative<std::string>(username_param)) {
                std::string username = std::get<std::string>(username_param);
                event.reply("Hello, " + username + ", welcome!");
            }
            
        }
        if (event.command.get_command_name() == "add") {
            const auto& number_1_param = event.get_parameter("number_1");
            const auto& number_2_param = event.get_parameter("number_2");

            std::visit([&event](const auto& num1, const auto& num2) {
                using T1 = std::decay_t<decltype(num1)>;
                using T2 = std::decay_t<decltype(num2)>;

                if constexpr (std::is_same_v<T1, int64_t> && std::is_same_v<T2, int64_t>) {
                    int64_t number_1 = num1;
                    int64_t number_2 = num2;

                    int64_t result = number_1 + number_2;

                    std::ostringstream oss;
                    oss << "[Add] The result is " << number_1 << " + " << number_2 << " = " << result;

                    event.reply(oss.str());
                }
                else {
                    event.reply("Please provide both number_1 and number_2 parameters as integers.");
                }
            }, number_1_param, number_2_param);
        }
        if (event.command.get_command_name() == "sub") {
            const auto& number_1_param = event.get_parameter("number_1");
            const auto& number_2_param = event.get_parameter("number_2");

            std::visit([&event](const auto& num1, const auto& num2) {
                using T1 = std::decay_t<decltype(num1)>;
                using T2 = std::decay_t<decltype(num2)>;

                if constexpr (std::is_same_v<T1, int64_t> && std::is_same_v<T2, int64_t>) {
                    int64_t number_1 = num1;
                    int64_t number_2 = num2;

                    int64_t result = number_1 - number_2;

                    std::ostringstream oss;
                    oss << "[Sub] The result is " << number_1 << " - " << number_2 << " = " << result;

                    event.reply(oss.str());
                }
                else {
                    event.reply("Please provide both number_1 and number_2 parameters as integers.");
                }
            }, number_1_param, number_2_param);
        }
        if (event.command.get_command_name() == "mul") {
            const auto& number_1_param = event.get_parameter("number_1");
            const auto& number_2_param = event.get_parameter("number_2");

            std::visit([&event](const auto& num1, const auto& num2) {
                using T1 = std::decay_t<decltype(num1)>;
                using T2 = std::decay_t<decltype(num2)>;

                if constexpr (std::is_same_v<T1, int64_t> && std::is_same_v<T2, int64_t>) {
                    int64_t number_1 = num1;
                    int64_t number_2 = num2;

                    int64_t result = number_1 * number_2;

                    std::ostringstream oss;
                    oss << "[Mul] The result is " << number_1 << " * " << number_2 << " = " << result;

                    event.reply(oss.str());
                }
                else {
                    event.reply("Please provide both number_1 and number_2 parameters as integers.");
                }
                }, number_1_param, number_2_param);
        }
    });

    


    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("greeting", "Greet the user", bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, "username", "User's name", true)));
            bot.global_command_create(
                dpp::slashcommand("add", "Add two numbers", bot.me.id)
                .add_option(dpp::command_option(dpp::co_integer, "number_1", "First number", true))
                .add_option(dpp::command_option(dpp::co_integer, "number_2", "Second number", true)));
            bot.global_command_create(
                dpp::slashcommand("sub", "Sub two numbers", bot.me.id)
                .add_option(dpp::command_option(dpp::co_integer, "number_1", "First number", true))
                .add_option(dpp::command_option(dpp::co_integer, "number_2", "Second number", true)));
            bot.global_command_create(
                dpp::slashcommand("mul", "Sub two numbers", bot.me.id)
                .add_option(dpp::command_option(dpp::co_integer, "number_1", "First number", true))
                .add_option(dpp::command_option(dpp::co_integer, "number_2", "Second number", true)));
        }

    });

    /* Start the bot */
    bot.start(dpp::st_wait);

    return 0;
}

