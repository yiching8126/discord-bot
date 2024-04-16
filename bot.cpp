#include <dpp/dpp.h>
#include <string>
#include <random>
#include <fstream>
#include <stdio.h>
#include <limits>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
namespace fs = std::filesystem;
std::vector <std::string> memes;
std::vector <int> memes_rating;
//for memes feature

using namespace std;


const std::string    BOT_TOKEN = "token";
int janken_rounds = INT16_MAX;
int janken_attack;
int bot_won_janken, player_won_janken;
//for janken feature
int main() {
    int guess_ans = rand() % 100;

    //21點
    srand(time(NULL));
    bool gameStarted = false; //遊戲狀態
    std::pair <int,int>card_value = {}; //玩家卡牌點數
    //card_value.first 除了Ａ以外的卡牌
    //card_value.second 卡牌Ａ
    std::pair <int,int>dealer_card_value = {}; //莊家卡牌點數
    //dealer_card_value.first 除了Ａ以外的卡牌
    //dealer_card_value.second 卡牌Ａ
    int card = 0; //玩家卡牌數
    vector<int>type = {}; //卡牌的出現次數
    //type[1]為點數1的卡牌數，type[2]為點數2的卡牌數...

    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand greeting("greeting", "Say hello to the given username", bot.me.id);
            greeting.add_option(
                dpp::command_option(dpp::co_string, "username", "Please enter the username", true)
            );
            bot.global_command_create(greeting);
            dpp::slashcommand add("add", "Add two given integers", bot.me.id);
            add.add_option(
                dpp::command_option(dpp::co_string, "number_1", "Please enter an integer", true)
            );
            add.add_option(
                dpp::command_option(dpp::co_string, "number_2", "Please enter an integer", true)
            );
            bot.global_command_create(add);

            dpp::slashcommand sub("sub", "Subtract two given integers", bot.me.id);
            sub.add_option(
                dpp::command_option(dpp::co_string, "number_1", "Please enter an integer", true)
            );
            sub.add_option(
                dpp::command_option(dpp::co_string, "number_2", "Please enter an integer", true)
            );
            bot.global_command_create(sub);

            dpp::slashcommand mul("mul", "Multiply two given integers", bot.me.id);
            mul.add_option(
                dpp::command_option(dpp::co_string, "number_1", "Please enter an integer", true)
            );
            mul.add_option(
                dpp::command_option(dpp::co_string, "number_2", "Please enter an integer", true)
            );
            bot.global_command_create(mul);

            dpp::slashcommand reset("reset", "Randomly generate an integer between 1 to 100", bot.me.id);
            bot.global_command_create(reset);

            dpp::slashcommand guess("guess", "Guess a number between 1 to 100", bot.me.id);
            guess.add_option(
                dpp::command_option(dpp::co_string, "guess_num", "Please guess a number between 1 to 100", true)
            );
            bot.global_command_create(guess);

            dpp::slashcommand janken_init("janken_init", "tell the bot to play paper scissors rock", bot.me.id);
            janken_init.add_option(
                dpp::command_option(dpp::co_string, "rounds", "How many rounds you want to win", true)
            );
            bot.global_command_create(janken_init);

            dpp::slashcommand janken("janken", "play janken with the bot", bot.me.id);
            dpp::command_option attack(dpp::co_string, "attack", "The thing you want to play", true);
            dpp::command_option_choice sci("scissors", "sci");
            dpp::command_option_choice roc("rock", "roc");
            dpp::command_option_choice pap("paper", "pap");
            attack.add_choice(sci);
            attack.add_choice(roc);
            attack.add_choice(pap);
            janken.add_option(attack);
            bot.global_command_create(janken);


            bot.global_command_create(janken);
            dpp::slashcommand janken_argue("janken_argue", "you are not good at janken, being a sore loser, you...", bot.me.id);
            bot.global_command_create(janken_argue);

            dpp::slashcommand meme_share("meme_share", "share a meme", bot.me.id);
            meme_share.add_option(
                dpp::command_option(dpp::co_string, "link", "link of the meme", true)
            );
            bot.global_command_create(meme_share);
            dpp::slashcommand get_rand_meme("get_rand_meme", "Get a random meme shared by this discord server", bot.me.id);
            bot.global_command_create(get_rand_meme);
            dpp::slashcommand meme_rate("meme_rate", "rate a meme", bot.me.id);
            meme_rate.add_option(
                dpp::command_option(dpp::co_string, "id", "id of the meme", true)
            );
            meme_rate.add_option(
                dpp::command_option(dpp::co_string, "rating", "Rating you give", true)
            );
            bot.global_command_create(meme_rate);
            dpp::slashcommand get_specific_meme("get_specific_meme", "get a specific meme", bot.me.id);
            get_specific_meme.add_option(
                dpp::command_option(dpp::co_string, "id", "id of the meme", true)
            );
            bot.global_command_create(get_specific_meme);
            dpp::slashcommand write("write", "write a diary", bot.me.id);
            bot.global_command_create(write);

            dpp::slashcommand read("read", "read a diary", bot.me.id);
            read.add_option(
                dpp::command_option(dpp::co_string, "date", "Please enter diary date", true)
            );
            bot.global_command_create(read);

            dpp::slashcommand remove("remove", "remove a dirary from documents", bot.me.id);
            remove.add_option(
                dpp::command_option(dpp::co_string, "date", "The date of the diary to remove", true)
            );
            bot.global_command_create(remove);

            bot.global_command_create(dpp::slashcommand("start_game", "play Blackjack", bot.me.id));

            bot.global_command_create(dpp::slashcommand("hit", "add card", bot.me.id));

            bot.global_command_create(dpp::slashcommand("stand", "end game", bot.me.id));

        }
        });

    bot.on_form_submit([&](const dpp::form_submit_t& event) {
    
    std::string date = std::get<std::string>(event.components[0].components[0].value);
    std::string title = std::get<std::string>(event.components[1].components[0].value);
    std::string content = std::get<std::string>(event.components[2].components[0].value);

    dpp::message m;
    std::string res;
    res = "Date: " + date + '\n' + "Title: " + title + '\n' + "Content: " + '\n' + content;
    m.set_content(res).set_flags(dpp::m_ephemeral);
    event.reply(m);
    std::ofstream out("C:\\Users\\USER\\Documents\\Diary " + date + ".txt");
    out << res;
    out.close();
        });




    bot.on_slashcommand([&bot, &card_value, &gameStarted, &dealer_card_value, &card, &guess_ans](const dpp::slashcommand_t& event) {
        std::string com = event.command.get_command_name();
    //指定功能start
    if (com == "greeting") {

        std::string username = std::get<std::string>(event.get_parameter("username"));
        event.reply("Hello, " + username);
    }
    else if (com == "add") {
        std::string n1 = std::get<std::string>(event.get_parameter("number_1"));
        std::string n2 = std::get<std::string>(event.get_parameter("number_2"));

        int num1, num2;
        num1 = std::stoi(n1); num2 = std::stoi(n2);
        std::string ans = std::to_string(num1 + num2);
        event.reply("[Add] The result is " + n1 + " + " + n2 + " = " + ans);
    }
    else if (com == "sub") {
        std::string n1 = std::get<std::string>(event.get_parameter("number_1"));
        std::string n2 = std::get<std::string>(event.get_parameter("number_2"));

        int num1, num2;
        num1 = std::stoi(n1); num2 = std::stoi(n2);
        std::string ans = std::to_string(num1 - num2);
        event.reply("[Sub] The result is " + n1 + " - " + n2 + " = " + ans);
    }
    else if (com == "mul") {
        std::string n1 = std::get<std::string>(event.get_parameter("number_1"));
        std::string n2 = std::get<std::string>(event.get_parameter("number_2"));

        int num1, num2;
        num1 = std::stoi(n1); num2 = std::stoi(n2);
        std::string ans = std::to_string(num1 * num2);
        event.reply("[Mul] The result is " + n1 + " * " + n2 + " = " + ans);
    }
    else if (com == "reset") {
        guess_ans = rand() % 100;
        event.reply("Reset successful!");
    }
    else if (com == "guess") {
        std::string guessing = std::get<std::string>(event.get_parameter("guess_num"));
        int gue = std::stoi(guessing);
        if (gue == guess_ans) {
            event.reply("Bingo!");
        }
        else if (gue < guess_ans) {
            event.reply("Guess a bigger number!");
        }
        else {// gue > guess_ans
            event.reply("Guess a smaller number!");
        }
    }

    if (event.command.get_command_name() == "write") {
        //  Instantiate an interaction_modal_response object 
        dpp::interaction_modal_response modal("my_modal", "Please enter stuff");
        //  Add a text component 
        modal.add_component(
            dpp::component().
            set_label("DATE (IN FORMS OF YYYYMMDD)").
            set_id("date").
            set_type(dpp::cot_text).

            set_min_length(5).
            set_max_length(50).
            set_text_style(dpp::text_short)
        );
        // Add another text component in the next row, as required by Discord 
        modal.add_row();

        modal.add_component(
            dpp::component().
            set_label("TITLE").
            set_id("title").
            set_type(dpp::cot_text).

            set_min_length(5).
            set_max_length(50).
            set_text_style(dpp::text_short)
        );
        modal.add_row();

        modal.add_component(
            dpp::component().
            set_label("YOUR DIARY").
            set_id("text").
            set_type(dpp::cot_text).

            set_min_length(1).
            set_max_length(2000).
            set_text_style(dpp::text_paragraph)
        );
        // Trigger the dialog box. All dialog boxes are ephemeral 
        event.dialog(modal);
    }
    else if (com == "read") {
        std::string date = std::get<std::string>(event.get_parameter("date"));
        std::ifstream in("C:\\Users\\USER\\Documents\\Diary " + date + ".txt");
        if (in) {
            std::string title, line, contents = "";
            std::string ignore;
            in >> ignore >> ignore >> ignore;
            std::getline(in, title);
            in >> ignore;
            while (!in.eof()) {
                std::getline(in, line);
                contents += line;
                contents += '\n';
            }


            dpp::embed embed = dpp::embed().

                set_color(dpp::colors::sti_blue).
                set_title(title).
                add_field(
                    "Date",
                    date
                ).
                add_field(
                    "Content",
                    contents
                ).
                set_footer(dpp::embed_footer().set_text("My Diary at " + date)).
                set_timestamp(time(0));

            in.close();

            dpp::message m;
            m.add_embed(embed);
            //so the message is just an embed
            event.reply(m);
        }
        else {

            event.reply("Diary on " + date + " not found!!!!");
        }
    }
    else if (com == "remove") {
        //to do
        std::string date = std::get<std::string>(event.get_parameter("date"));
        std::string filename = "C:\\Users\\USER\\Documents\\Diary " + date + ".txt";
        if (std::remove(filename.c_str()) == 0) {//std::string cannot be in remove()
            event.reply("Successfully removed diary at " + date);
        }
        else {
            event.reply("Failed to remove diary at " + date);
        }

    }
    //指定功能end
    //blackjack
    else if (event.command.get_command_name() == "start_game") {//initialize blackjack game
        gameStarted = true;
        int get_card = rand() % 13 + 1;
        int dealer_visible_card = 0;
        card = 0;
        card_value = {};
        dealer_card_value = {};
        std::string dealer_visible_card_value;
        for (int i = 0; i < 2;i++) {
            card++;
            if (get_card == 1) {
                card_value.second++;
            }
            else if (get_card > 10) {
                card_value.first += 10;
            }
            else {
                card_value.first += get_card;
            }
            get_card = rand() % 13 + 1;
        }
        for (int i = 0; i < 2;i++) {
            dealer_visible_card = get_card;
            if (get_card == 1) {
                //A可為11點或1點
                dealer_card_value.second++;
                dealer_visible_card_value = "A";
            }
            else if (get_card > 10) {
                dealer_card_value.first += 10;
            }
            else {
                dealer_card_value.first += get_card;
            }
            get_card = rand() % 13 + 1;
        }
        if (dealer_visible_card == 11) {
            dealer_visible_card_value = "J";
        }
        else if (dealer_visible_card == 12) {
            dealer_visible_card_value = "Q";
        }
        else if (dealer_visible_card == 13) {
            dealer_visible_card_value = "K";
        }
        dpp::message m;
        m.set_content("Now you have " + std::to_string(card) + " cards, and sum of points is " +
            std::to_string(card_value.first) + "+" + std::to_string(card_value.second) + "A" + ".\n" +
            "Now one of dealer's card is " + std::to_string(dealer_visible_card) + ".\n" +
            "You have two choice, hit or stand ?");

        event.reply(m);
    }
    else if (event.command.get_command_name() == "hit") {//use a card
        dpp::message m;
        int get_card = rand() % 13 + 1;
        int card_sum = 0;

        if (gameStarted) {
            card++;

            if (get_card == 1) {
                card_value.second++;
            }
            else if (get_card > 10) {
                card_value.first += 10;
            }
            else {
                card_value.first += get_card;
            }

            if ((card_value.first + card_value.second * 11) < 21) {
                card_sum = (card_value.first + card_value.second * 11);
            }
            else {
                card_sum = (card_value.first + card_value.second);
            }

            if (card_sum > 21) {
                m.set_content("Sum of points is " + std::to_string(card_sum) + ". You lose, haha!!!");
                gameStarted = false;
                event.reply(m);
            }
            else {
                m.set_content("Now you have " + std::to_string(card) + " cards, and the sum of points is " +
                    std::to_string(card_value.first) + "+" + std::to_string(card_value.second) + "A" + ".\n" +
                    "You have two choices: hit or stand?");
                event.reply(m);
            }
        }
        else {
            m.set_content("Please start the game!");
            event.reply(m);
        }
    }
    else if (event.command.get_command_name() == "stand") {//not use a card
        dpp::message m;

        if (gameStarted) {
            int card_sum = 0;
            int dealer_card_sum = 0;
            int get_card = rand() % 13 + 1;

            // Player
            if ((card_value.first + card_value.second * 11) < 21) {
                card_sum = (card_value.first + card_value.second * 11);
            }
            else {
                card_sum = (card_value.first + card_value.second);
            }

            // Dealer
            if ((dealer_card_value.first + dealer_card_value.second * 11) < 21) {
                dealer_card_sum = (dealer_card_value.first + dealer_card_value.second * 11);
            }
            else {
                dealer_card_sum = (dealer_card_value.first + dealer_card_value.second);
            }

            m.set_content("Dealer's sum of points is " + std::to_string(dealer_card_sum));

            while (dealer_card_sum < 17) {
                if (get_card == 1) {
                    dealer_card_value.second++;
                }
                else if (get_card > 10) {
                    dealer_card_value.first += 10;
                }
                else {
                    dealer_card_value.first += get_card;
                }

                get_card = rand() % 13 + 1;

                if ((dealer_card_value.first + dealer_card_value.second * 11) < 21) {
                    dealer_card_sum = (dealer_card_value.first + dealer_card_value.second * 11);
                }
                else {
                    dealer_card_sum = (dealer_card_value.first + dealer_card_value.second);
                }
            }

            if (dealer_card_sum > 21) {
                m.set_content("Dealer's sum of points is " + std::to_string(dealer_card_sum) + ". You win! :)");
                gameStarted = false;
                event.reply(m);
            }
            else {
                if (card_sum > dealer_card_sum) {
                    m.set_content("Your sum of points is " + std::to_string(card_sum) + ".\nDealer's sum of points is " +
                        std::to_string(dealer_card_sum) + ". You win! :)");
                    gameStarted = false;
                    event.reply(m);
                }
                else if (card_sum < dealer_card_sum) {
                    m.set_content("Your sum of points is " + std::to_string(card_sum) + ".\nDealer's sum of points is " +
                        std::to_string(dealer_card_sum) + ". You lose, haha!!!");
                    gameStarted = false;
                    event.reply(m);
                }
                else {
                    m.set_content("Your sum of points is " + std::to_string(card_sum) + ".\nDealer's sum of points is " +
                        std::to_string(dealer_card_sum) + ". Push!");
                    gameStarted = false;
                    event.reply(m);
                }
            }
        }
        else {
            m.set_content("Please start the game!");
            event.reply(m);
        }
    }

    //janken
    else if (com == "janken_init") {//set number of points required to win
        srand(rand());
        std::string rounds = std::get<std::string>(event.get_parameter("rounds"));
        janken_rounds = std::stoi(rounds);
        player_won_janken = 0;
        bot_won_janken = 0;
        srand(rand() + janken_rounds);
        event.reply("Cool, lets play janken. Try winning " + std::to_string(janken_rounds) + " rounds.");
    }
    else if (com == "janken") {//play a game of janken

        int bot_attack = (rand() % 2);
        int player_attack;
        std::string attack = std::get<std::string>(event.get_parameter("attack"));
        if (!(attack == "sci" || attack == "pap" || attack == "roc")) {
            if (!(attack == "scissors" || attack == "paper" || attack == "rock")) {
                event.reply("invalid");
            }
        }

        std::string repl;
        if (rand() % 3 == 0) {//0, 1, 2
            repl += "You win! ";
            player_won_janken++;
        }
        else if (rand() % 3 == 1) {
            repl += "You lost! ";
            bot_won_janken++;
        }
        else {
            repl += "Draw. ";
        }
        repl += (std::to_string(player_won_janken) + " - " + std::to_string(bot_won_janken) + ". ");

        if (player_won_janken == janken_rounds) {
            repl += "You're really good at this. Now reinitialize janken.";
            janken_rounds = INT16_MAX;
            player_won_janken = 0;
            bot_won_janken = 0;
        }
        else if (bot_won_janken == janken_rounds) {
            repl += "HA! Game over! Now reinitialize janken.";
            janken_rounds = INT16_MAX;
            player_won_janken = 0;
            bot_won_janken = 0;
        }
        event.reply(repl);
    }
    else if (com == "janken_argue") {//one more chance to win
        janken_rounds++;
        event.reply("More rounds! Try winning " + std::to_string(janken_rounds) + " rounds in total.");
    }
    //meme
    else if (com == "meme_share") {//share a meme to memes (a vector<string>)
        std::string link = std::get<std::string>(event.get_parameter("link"));
        memes.push_back(link);
        memes_rating.push_back(0);
        event.reply("You added a meme, there are currently " + to_string(memes.size()) + " of them here.");
    }
    else if (com == "get_rand_meme") {//show a random meme from memes
        if (memes.size() == 0) {
            event.reply("There are no memes here, upload some.");
        }
        else {
            int id = rand() % memes.size();
            event.reply("meme id : " + to_string(id) + ", Rating " + to_string(memes_rating[id]) + "\n" + memes[id]);
        }

    }
    else if (com == "meme_rate") {//modify rating[id]
        std::string id = std::get<std::string>(event.get_parameter("id"));
        std::string rating = std::get<std::string>(event.get_parameter("rating"));
        int idi = stoi(id);
        int r = stoi(rating);

        if (r > 0) {
            memes_rating[idi]++;
            event.reply("Rating of meme no. " + id + " increased.");
        }
        else if (r < 0) {
            memes_rating[idi]--;
            event.reply("Rating of meme no. " + id + " decreased.");
        }
        else {
            event.reply("You effectively did nothing.");
        }

    }
    else if (com == "get_specific_meme") {//show meme[id]
        int id = stoi(std::get<std::string>(event.get_parameter("id")));
        if (memes.size() == 0) {
            event.reply("There are no memes here yet, go add some");
        }
        else if (id >= memes.size()) {
            event.reply("The biggest id here is " + std::to_string(memes.size() - 1));
        }
        else {
            event.reply("meme id : " + std::to_string(id) + ", Rating " + std::to_string(memes_rating[id]) + "\n" + memes[id]);
        }
    }


        });
    /* Start bot */
    bot.start(dpp::st_wait);
    return 0;
}