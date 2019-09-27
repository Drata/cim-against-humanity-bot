#include "../include/cards.h"
#include <iterator>
#include <algorithm>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

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
	Mat image;
    image = imread("../images/CIMah.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(!image.data )                              // Check for invalid input
    {
        std::cout << "Could not open or find the image" << std::endl ;
        return "";
    }

	Point org = cvPoint(image.cols * 0.1, image.rows * 0.2);
	putText(image, message, org, FONT_HERSHEY_DUPLEX, 2, Scalar(255,255,255), 2, CV_AA);

	imwrite("../images/black_card.jpg", image);

	return "../images/black_card.jpg";
}
