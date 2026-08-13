#include 'Board.h'

class Board{
private:
    int rows;
    int columns;
    int totalMines;
    vector <vector<Tile>> logicTiles;
    vector <vector<TileSprite>> visualTiles;
    bool showMines;
public:
    Board(int rows, int columns, int totalMines){
        this -> rows = rows;
        this -> columns = columns;
        this -> totalMines = totalMines;
        showMines = false;
        logicTiles.resize(rows, vector<Tile>(columns));
        visualTiles.resize(rows, vector<TileSprite>(columns));
    }

    void initializeMines(int safeRow, int safeCol) {
        srand(static_cast<unsigned int>(time(nullptr)));
        int minesPlaced = 0;

        while (minesPlaced < totalMines) {
            int totalTiles = rows * columns;
            int randomTile = rand() % totalTiles; // Generate a random tile index

            // Convert randomTile to row and column
            int randomRow = randomTile / columns;
            int randomCol = randomTile % columns;

            // Ensure that the random tile and its surrounding tiles are safe
            bool isSafe = abs(randomRow - safeRow) > 1 || abs(randomCol - safeCol) > 1;

            // Check if the random tile is safe and not already containing a mine
            if (isSafe && !logicTiles[randomRow][randomCol].getMine()) {
                // Check if the random tile and its adjacent tiles are not mines
                bool noAdjacentMines = true;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; ++j) {
                        int newRow = randomRow + i;
                        int newCol = randomCol + j;

                        // Skip out-of-bounds tiles and the current tile
                        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= columns || (i == 0 && j == 0)) {
                            continue;
                        }

                        // Check if the adjacent tile contains a mine
                        if (logicTiles[newRow][newCol].getMine()) {
                            noAdjacentMines = false;
                            break;
                        }
                    }
                    if (!noAdjacentMines) {
                        break;
                    }
                }

                // If the random tile and its adjacent tiles are safe, place a mine there
                if (noAdjacentMines) {
                    logicTiles[randomRow][randomCol].setMine(true);
                    minesPlaced++;
                }
            }
        }
    }

    void syncTiles() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                // Update the visibility of tiles based on the showMines flag
                if (showMines && logicTiles[row][col].getMine()) {
                    visualTiles[row][col].setRevealed(true); // Show mines if flag is true
                } else {
                    visualTiles[row][col].setRevealed(logicTiles[row][col].getRevealed());
                }
                visualTiles[row][col].setFlagged(logicTiles[row][col].getFlagged());
                visualTiles[row][col].setMine(logicTiles[row][col].getMine()); // Pass hasMine information
            }
        }
    }

    void toggleMinesVisibility() {
        showMines = !showMines; // Toggle the flag
        syncTiles(); // Update the visibility of tiles accordingly
    }

    void unToggleMinesVisibility() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                if (logicTiles[row][col].getMine()) {
                    visualTiles[row][col].setMine(false); // Hide the mine sprite
                }
                if (!logicTiles[row][col].getRevealed()) {
                    visualTiles[row][col].setRevealed(false); // Hide the tile if it's not revealed
                }
            }
        }
    }


    Tile& getTile(int row, int column) {
        return logicTiles[row][column];
    }

    TileSprite& getTileSprite(int row, int column) {
        return visualTiles[row][column];
    }

    int getRows(){
        return rows;
    }

    int getColumns(){
        return columns;
    }
};