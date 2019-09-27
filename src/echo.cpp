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

int main() {
    string token("");
    Bot bot(token);

    bot.getEvents().onCommand("start", [&bot](Message::Ptr message) 
    {
        bot.getApi().sendMessage(message->chat->id, "Hi! I'm CIM Against Humanity."
                                                    " I'll be taking notes of your cards today.");
    });

    bot.getEvents().onCommand("send_cards", [&bot](Message::Ptr message) 
    {

        bot.getApi().sendMessage(message->chat->id, "Sending files. Please wait.");

        bot.getApi().sendDocument(message->chat->id, InputFile::fromFile("white.txt", "text/plain"), "White cards");
        bot.getApi().sendDocument(message->chat->id, InputFile::fromFile("black.txt", "text/plain"), "Black cards");

    });

    bot.getEvents().onNonCommandMessage([&bot](Message::Ptr message) {
        std::cout << message->text << std::endl;
        int captured;

        captured = capture_cards(message);

        switch (captured)
        { 
            case 0:
                bot.getApi().sendMessage(message->chat->id, "Your card has been saved.");
				create_card_image(message->text);
				bot.getApi().sendDocument(message->chat->id, InputFile::fromFile("../images/black_card.jpg", "text/plain"), "Black cards");
				//send_image
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
