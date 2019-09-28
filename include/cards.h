#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>
#include <tgbot/tgbot.h>

using namespace TgBot;

int capture_cards(Message::Ptr message);
std::string create_card_image(std::string str);
void create_all_card_image();

#endif