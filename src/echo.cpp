#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <tgbot/tgbot.h>
#include <vector>
#include <iterator>

#include "../include/cards.h"

#define PATH_TO_WHITE "white.txt"
#define PATH_TO_BLACK "black.txt"

using namespace std;
using namespace TgBot;

std::ofstream white_cards, black_cards;

int main() {
    string token("TOKEN");

    Bot bot(token);

    std::vector<std::string> white_cards, black_cards;

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) 
    {
        bot.getApi().sendMessage(message->chat->id, "Hi! I'm CIM Against Humanity."
                                                    " I'll be taking notes of your cards today.");
    });

    bot.getEvents().onCommand("generate_expansion", [&bot, &white_cards, &black_cards](Message::Ptr message) 
    {
        std::cout << "generating files" << std::endl;
        bot.getApi().sendMessage(message->chat->id, "Generating files.");

        std::ofstream white_cards_file, black_cards_file;

        white_cards_file.open(PATH_TO_WHITE, std::ios::app);
        black_cards_file.open(PATH_TO_BLACK, std::ios::app);
        
        for(auto it = white_cards.begin(); it != white_cards.end(); ++it)
        {
            white_cards_file << *it << '\n';
        }

        for(auto it = black_cards.begin(); it != black_cards.end(); ++it)
        {
            black_cards_file << *it << '\n';
        }

        white_cards_file.close();
        black_cards_file.close();
    });

    bot.getEvents().onNonCommandMessage([&bot, &white_cards, &black_cards](Message::Ptr message) {
        std::cout << message->text << std::endl;
        int captured;

        captured = capture_cards(message->text, black_cards, white_cards);

        switch (captured)
        { 
            case 0:
                bot.getApi().sendMessage(message->chat->id, "Your card has been saved.");
                break;
            case 1:
                bot.getApi().sendMessage(message->chat->id, "Incorrect format. Use <blank> to write black cards.");
                break;
            default:
                break;
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
