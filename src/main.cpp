#include "screens/WelcomeScreen.h"
#include "screens/GameScreen.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("files/config.cfg");

    std::string columns;
    std::string rows;
    std::string mines;

    std::getline(file, columns);
    std::getline(file, rows);
    std::getline(file, mines);

    file.close();

    int width = std::stoi(columns) * 32;
    int height = std::stoi(rows) * 32 + 100;

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    std::string username =
        WelcomeScreen(width, height, font);

    if (!username.empty())
    {
        GameScreen(
            username,
            width,
            height,
            std::stoi(columns),
            std::stoi(rows),
            std::stoi(mines),
            font
        );
    }

    return 0;
}