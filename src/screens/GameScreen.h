#pragma once

#include <SFML/Graphics.hpp>
#include <string>

void GameScreen(
    std::string username,
    int width,
    int height,
    int columns,
    int rows,
    int mines,
    sf::Font font
);