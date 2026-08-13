#pragma once

#include <vector>

#include "../models/Tile.h"
#include "../graphics/TileSprite.h"

class Board
{
private:
    int rows;
    int columns;
    int totalMines;

    std::vector<std::vector<Tile>> logicTiles;
    std::vector<std::vector<TileSprite>> visualTiles;

    bool showMines;

public:
    Board(int rows, int columns, int totalMines);

    void initializeMines(int safeRow, int safeCol);

    void syncTiles();

    void toggleMinesVisibility();
    void unToggleMinesVisibility();

    Tile& getTile(int row, int column);
    TileSprite& getTileSprite(int row, int column);

    int getRows();
    int getColumns();
};