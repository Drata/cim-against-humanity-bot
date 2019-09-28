#include "../include/cards.h"

// standard library dependencies
#include <iterator>
#include <algorithm>
#include <fstream>

// opencv dependencies

#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>


using namespace cv;

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

std::string create_card_image(std::string message)
{
	cv::Mat image = cv::imread( "../images/CIMah.png" );
    
    if( image.empty() ){
        cv::waitKey( 0 );
        return "";
    }

    // Create FreeType2
    cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
    
    // Load Font Data
    ft2->loadFontData( "../data/neue-helvetica.otf", 0 );

    // Put Text
    std::vector<std::string> message_splitted;
    int next_end = 17;
    int start = 0;

    while(next_end < message.size()) 
    {
        if(!(message.at(next_end) == ' '  ||
             message.at(next_end - 1) == ' ')) 
        {
            next_end--;
            
            for (auto it = message.begin() + next_end -1; *it != ' '; --it)
            {
                next_end--;
            }
        }

        message_splitted.push_back(std::string(message.begin() + start, 
                                               message.begin() + next_end));
    
        start = next_end;
        next_end += 17;
    }

    message_splitted.push_back(std::string(message.begin() + start, message.end()));

    int i = 0;
    for(auto it = message_splitted.begin(); it != message_splitted.end(); ++it)
    {
        ft2->putText(image, *it, cv::Point( 50, 75 + (50 * i)), 50, cv::Scalar( 255, 255, 255 ), -1, cv::LINE_AA, false );
        i++;
    }   

    cv::imwrite("../images/black_card.jpg", image);

    return "";
}
