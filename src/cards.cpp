#include "../include/cards.h"
#include <iterator>
#include <algorithm>
#include <fstream>


int capture_cards(Message::Ptr message)
{
    std::ofstream white_cards, black_cards;

    // No queremos modificar el 'case' de la cadena original, por lo que especificamos 
	// todos los casos en las keywords.
	std::vector<std::string> white_keywords = {"Blanca: ", "blanca: "};
	std::vector<std::string> black_keywords = {"Negra: ", "negra: "};

    std::string black_path("../data/" + std::to_string(message->chat->id) + "/black.txt");
    std::string white_path("../data/" + std::to_string(message->chat->id) + "/white.txt");

    // Comprobamos si es carta negra
    for (auto it = black_keywords.begin(); it != black_keywords.end(); ++it)
    {
        if (size_t pos = (message->text.find(*it)) != std::string::npos)
        {
            if(message->text.find("<blank>") == std::string::npos)
            {
                return 1;
            }

            // Eliminamos el keyword de la frase.
            message->text.replace(0, it->length(), "");

            black_cards.open("black.txt", std::ios::app);
            black_cards << message->text << std::endl;
            black_cards.close();

            return 0;
        }
    }

    // Comprobamos si es carta blanca
    for (auto it = white_keywords.begin(); it != white_keywords.end(); ++it)
    {
        if (size_t pos = (message->text.find(*it)) != std::string::npos)
        {
            // Eliminamos el keyword de la frase.
            message->text.replace(0, it->length(), "");

            white_cards.open("white.txt", std::ios::app);
            white_cards << message->text << std::endl;
            white_cards.close();

            return 0;
        }
    }

    return -1;
}