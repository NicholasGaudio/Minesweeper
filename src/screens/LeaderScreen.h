#pragma once

#include <SFML/Graphics.hpp>
#include <string>

bool LeaderScreen(
    std::string username,
    int minutes,
    int seconds,
    int columns,
    int rows,
    sf::Font font
);