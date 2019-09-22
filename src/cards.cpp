#include "../include/cards.h"
#include <iterator>
#include <algorithm>
#include <fstream>

int capture_cards(std::string message, std::vector<std::string>& black_cards, std::vector<std::string> &white_cards)
{
    // No queremos modificar el 'case' de la cadena original, por lo que especificamos 
	// todos los casos en las keywords.
	std::vector<std::string> white_keywords = {"Blanca: ", "blanca: "};
	std::vector<std::string> black_keywords = {"Negra: ", "negra: "};

    // Comprobamos si es carta negra
    for (auto it = black_keywords.begin(); it != black_keywords.end(); ++it)
    {
        if (size_t pos = (message.find(*it)) != std::string::npos)
        {
            if(message.find("<blank>") == std::string::npos)
            {
                return 1;
            }

            // Eliminamos el keyword de la frase.
            message.replace(0, it->length(), "");

            black_cards.push_back(message);

            return 0;
        }
    }

    // Comprobamos si es carta blanca
    for (auto it = white_keywords.begin(); it != white_keywords.end(); ++it)
    {
        if (size_t pos = (message.find(*it)) != std::string::npos)
        {
            // Eliminamos el keyword de la frase.
            message.replace(0, it->length(), "");

            white_cards.push_back(message);

            return 0;
        }
    }

    return -1;
}