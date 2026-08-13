#pragma once

#include "../game/Board.h"
#include <SFML/Graphics.hpp>

int countAdjacentMines(
    Board& board,
    int row,
    int col,
    sf::RenderWindow& window
);