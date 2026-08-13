#include "Tile.h"

Tile::Tile()
{
    isMine = false;
    isFlagged = false;
    isRevealed = false;
    adjacentMines = 0;
}

void Tile::setMine(bool mine)
{
    isMine = mine;
}

void Tile::setRevealed(bool revealed)
{
    isRevealed = revealed;
}

void Tile::setFlagged(bool flagged)
{
    isFlagged = flagged;
}

void Tile::setAdjacentMines(int count)
{
    adjacentMines = count;
}

bool Tile::getMine()
{
    return isMine;
}

bool Tile::getRevealed()
{
    return isRevealed;
}

bool Tile::getFlagged()
{
    return isFlagged;
}

int Tile::getMines()
{
    return adjacentMines;
}