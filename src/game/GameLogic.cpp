#include 'GameLogic.h'

int countAdjacentMines(Board& board, int row, int col, sf::RenderWindow& window) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            // Skip out-of-bounds tiles
            if (newRow < 0 || newRow >= board.getRows() || newCol < 0 || newCol >= board.getColumns()) {
                continue;
            }

            // Skip the current tile
            if (i == 0 && j == 0) {
                continue;
            }

            // Check if the adjacent tile contains a mine
            if (board.getTile(newRow, newCol).getMine()) {
                count++;
            }
        }
    }

    // Set the number of adjacent mines for the tile at (row, col)
    board.getTile(row, col).setAdjacentMines(count);

    // Set the adjacent mines count for the corresponding TileSprite
    board.getTileSprite(row, col).setAdjacentMines(count, window);

    if (count == 0) {
        // Now recursively reveal adjacent tiles
        int tilesRevealed = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; ++j) {
                int newRow = row + i;
                int newCol = col + j;

                // Skip out-of-bounds and the current tile
                if (newRow < 0 || newRow >= board.getRows() || newCol < 0 || newCol >= board.getColumns() || (i == 0 && j == 0)) {
                    continue;
                }

                // If the adjacent tile is not already revealed or flagged
                if (!board.getTile(newRow, newCol).getRevealed() && !board.getTile(newRow, newCol).getFlagged()) {
                    // Mark the tile as revealed
                    board.getTileSprite(newRow, newCol).setRevealed(true);
                    board.getTile(newRow, newCol).setRevealed(true);

                    // Recursively reveal adjacent tiles and count them
                    tilesRevealed += countAdjacentMines(board, newRow, newCol, window) + 1;
                }
            }
        }
        return tilesRevealed;
    } else {
        return 0;
    }
}