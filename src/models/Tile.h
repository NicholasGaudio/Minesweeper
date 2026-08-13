#pragma once

class Tile
{
private:
    bool isMine;
    bool isFlagged;
    bool isRevealed;
    int adjacentMines;

public:
    Tile();

    void setMine(bool mine);
    void setRevealed(bool revealed);
    void setFlagged(bool flagged);
    void setAdjacentMines(int count);

    bool getMine();
    bool getRevealed();
    bool getFlagged();
    int getMines();
};