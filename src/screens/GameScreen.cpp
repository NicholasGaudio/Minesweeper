#include "GameScreen.h"

#include "../game/Board.h"
#include "../game/GameLogic.h"
#include "../graphics/Button.h"
#include "../graphics/TileSprite.h"
#include "../screens/LeaderboardScreen.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

void GameScreen(string username, int width, int height, int columns, int rows, int mines,  sf::Font font){
    sf::RenderWindow window(sf::VideoMode(width, height), "Game Screen");
    bool win = false;
    bool lose = false;
    bool writeTofile = false;
    int winRevealed = (columns * rows) - mines;
    int revealedTiles = 0;
    int flagCounter = mines;
    vector<sf::Drawable*> drawings;
    vector<Button> buttons;
    bool boardInitialized = false;
    Board board(rows, columns, mines);
    float tileSize = 32.0f;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            float x = col * tileSize;
            float y = row * tileSize;
            board.getTileSprite(row, col).setPosition(x, y);
        }
    }
    bool leaderScreen = false;
    bool debuged;
    sf::Texture hidden;
    hidden.loadFromFile("files/images/tile_hidden.png");


    //Creating Buttons
    Button happyFace("files/images/face_happy.png", sf::Vector2f((columns/2.0 * 32) -32, 32 * (rows + 0.5)));
    buttons.push_back(happyFace);
    Button debug("files/images/debug.png", sf::Vector2f ((columns * 32) - 304, (rows + 0.5) * 32));
    buttons.push_back(debug);
    Button pause("files/images/pause.png", sf::Vector2f ((columns * 32) - 240, (rows + 0.5) * 32));
    buttons.push_back(pause);
    Button leaderboard("files/images/leaderboard.png", sf::Vector2f ((columns * 32) - 176, (rows + 0.5) * 32));
    buttons.push_back(leaderboard);

    //Creating Timer
    sf::Clock clock;
    sf::Texture digitsTexture;
    sf::Time totalPausedTime = sf::Time::Zero;
    sf::Time elapsedTime;
    bool timerStarted = false;
    bool paused = false;
    bool wasPaused = false;

    digitsTexture.loadFromFile("files/images/digits.png");
    sf::Sprite digits[11];
    for (int i = 0; i < 11; i++) {
        digits[i].setTexture(digitsTexture);
        digits[i].setTextureRect(sf::IntRect(21 * i, 0, 21, 32));
    }
    int startX = columns * 32 - 54;
    int startY = 32 * (rows + 0.5) + 16;


    //Window Open/Events
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int mouseCol = mousePos.x/tileSize;
                    int mouseRow = mousePos.y/tileSize;
                    cout << mouseCol << endl;
                    cout << mouseRow << endl;
                    if (mouseCol >= 0 && mouseCol < columns && mouseRow >= 0 && mouseRow < rows) {
                        // Retrieve the corresponding tile and its sprite
                        TileSprite& tileSprite = board.getTileSprite(mouseRow, mouseCol);
                        Tile& tile = board.getTile(mouseRow, mouseCol);


                        if (!timerStarted) { // Start the timer when the user clicks
                            clock.restart(); // Restart the clock
                            timerStarted = true;
                        }
                        if (!boardInitialized) { // Check if the board is not initialized yet
                            board.initializeMines(mouseRow, mouseCol);
                            boardInitialized = true;
                        }

                        if (tile.getMine() && !tile.getFlagged() && !lose){
                            lose = true;
                            tileSprite.setRevealed(true);
                            tile.setRevealed(true);
                            board.toggleMinesVisibility();
                            happyFace.setTexture("files/images/face_lose.png");
                        }
                        // If the tile is not already revealed or flagged
                        if (!tile.getRevealed() && !tile.getFlagged() && !lose) {
                            tileSprite.setRevealed(true);
                            tile.setRevealed(true);
                            board.syncTiles();
                            revealedTiles += countAdjacentMines(board, mouseRow, mouseCol, window)+1;
                            if (revealedTiles == winRevealed){
                                win = true;
                                paused = true;
                                happyFace.setTexture("files/images/face_win.png");
                                for (int row = 0; row < rows; row++) {
                                    for (int col = 0; col < columns; col++) {
                                        if (board.getTile(row, col).getMine() && !board.getTile(row, col).getFlagged()) {
                                            // If the tile contains a mine and is not already flagged, flag it
                                            board.getTile(row, col).setFlagged(true);
                                            // Update the corresponding TileSprite
                                            board.getTileSprite(row, col).setFlagged(true);
                                            flagCounter--; // Decrease the flag counter
                                        }
                                    }
                                }
                            }
                        }


                    }

                    if (happyFace.isClicked(mousePos)){
                        window.close();
                        GameScreen( username,  width,  height,  columns,  rows,  mines,  font);
                    }
                    if (debug.isClicked(mousePos) && !lose && !win){
                        if (!debuged){
                            board.toggleMinesVisibility();
                            board.syncTiles();
                            debuged = true;
                        } else {
                            debuged = false;
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    if (board.getTile(row, col).getMine()) {
                                        board.getTileSprite(row, col).setMineHiddenTexture(hidden);
                                    }
                                }
                            }
                            board.unToggleMinesVisibility();
                            board.syncTiles();
                        }
                    }
                    if (pause.isClicked(mousePos) && !lose && !win){
                        if (!paused) {
                            pause.setTexture("files/images/play.png");
                            paused = true;
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).setPaused(true);
                                }
                            }
                            totalPausedTime += clock.getElapsedTime();
                        } else if (paused) {
                            pause.setTexture("files/images/pause.png");
                            paused = false;
                            wasPaused = true;
                            clock.restart();
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).setPaused(false);
                                }
                            }
                        }
                    }
                    if (leaderboard.isClicked(mousePos)){
                        if (paused){
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).setPaused(true);
                                }
                            }
                            if (!paused) {
                                if (wasPaused) {
                                    totalPausedTime += clock.getElapsedTime();
                                    wasPaused = false;
                                }

                                sf::Time frameElapsed = clock.restart();
                                elapsedTime += frameElapsed;
                            }

                            sf::Time adjustedElapsedTime = elapsedTime - totalPausedTime;

                            int totalSeconds = static_cast<int>(adjustedElapsedTime.asSeconds());
                            int minutes = totalSeconds/60;
                            int seconds = totalSeconds %60;

                            // Get individual digits
                            int digit1 = minutes / 10;
                            int digit2 = minutes % 10;
                            int digit3 = seconds/10;
                            int digit4= seconds % 10;

                            int flagCounterAbs = abs(flagCounter); // Get the absolute value of flagCounter

                            // Get individual digits for flag counter
                            int flagDigit1 = 0;
                            if (flagCounter < 0){
                                flagDigit1 = 10;
                            }
                            int flagDigit2 = (flagCounterAbs % 100) / 10; // Tens place
                            int flagDigit3 = flagCounterAbs % 10; // Ones place


                            //Drawing
                            window.clear(sf::Color::White);


                            // Draw the digits
                            digits[digit1].setPosition(columns * 32 - 97, 32 * (rows + 0.5) + 16);
                            window.draw(digits[digit1]);

                            digits[digit2].setPosition(columns * 32 - 97 + 21, 32 * (rows + 0.5) + 16);
                            window.draw(digits[digit2]);

                            digits[digit3].setPosition(startX,startY);
                            window.draw(digits[digit3]);

                            digits[digit4].setPosition(startX + 21 ,startY);
                            window.draw(digits[digit4]);

                            // Draw the digits for flag counter
                            digits[flagDigit1].setPosition(12, 32 * (rows + 0.5f)+16 ); // Adjust position for first digit
                            window.draw(digits[flagDigit1]);

                            digits[flagDigit2].setPosition(33, 32 * (rows + 0.5f)+16);
                            window.draw(digits[flagDigit2]);

                            digits[flagDigit3].setPosition(33 + 21, 32 * (rows + 0.5f)+16);
                            window.draw(digits[flagDigit3]);

                            //Drawing
                            for (int i =0; i<drawings.size(); i++){
                                window.draw(*drawings.at(i));
                            }
                            for (int i =0; i<buttons.size(); i++){
                                buttons[i].draw(window);
                            }


                            // Draw the updated tiles
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).Draw(window);
                                }
                            }

                            window.display();

                            if (!leaderScreen) {
                                if (!LeaderScreen(username, 999, 999, columns, rows, font)) {
                                    paused = true;
                                    wasPaused = false;
                                }
                            }
                        } else {
                            paused = true;
                            pause.setTexture("files/images/play.png");
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).setPaused(true);
                                }
                            }

                            if (!paused) {
                                if (wasPaused) {
                                    totalPausedTime += clock.getElapsedTime();
                                    wasPaused = false;
                                }

                                sf::Time frameElapsed = clock.restart();
                                elapsedTime += frameElapsed;
                            }

                            sf::Time adjustedElapsedTime = elapsedTime - totalPausedTime;

                            int totalSeconds = static_cast<int>(adjustedElapsedTime.asSeconds());
                            int minutes = totalSeconds/60;
                            int seconds = totalSeconds %60;

                            // Get individual digits
                            int digit1 = minutes / 10;
                            int digit2 = minutes % 10;
                            int digit3 = seconds/10;
                            int digit4= seconds % 10;

                            int flagCounterAbs = abs(flagCounter); // Get the absolute value of flagCounter

                            // Get individual digits for flag counter
                            int flagDigit1 = 0;
                            if (flagCounter < 0){
                                flagDigit1 = 10;
                            }
                            int flagDigit2 = (flagCounterAbs % 100) / 10; // Tens place
                            int flagDigit3 = flagCounterAbs % 10; // Ones place


                            //Drawing
                            window.clear(sf::Color::White);


                            // Draw the digits
                            digits[digit1].setPosition(columns * 32 - 97, 32 * (rows + 0.5) + 16);
                            window.draw(digits[digit1]);

                            digits[digit2].setPosition(columns * 32 - 97 + 21, 32 * (rows + 0.5) + 16);
                            window.draw(digits[digit2]);

                            digits[digit3].setPosition(startX,startY);
                            window.draw(digits[digit3]);

                            digits[digit4].setPosition(startX + 21 ,startY);
                            window.draw(digits[digit4]);

                            // Draw the digits for flag counter
                            digits[flagDigit1].setPosition(12, 32 * (rows + 0.5f)+16 ); // Adjust position for first digit
                            window.draw(digits[flagDigit1]);

                            digits[flagDigit2].setPosition(33, 32 * (rows + 0.5f)+16);
                            window.draw(digits[flagDigit2]);

                            digits[flagDigit3].setPosition(33 + 21, 32 * (rows + 0.5f)+16);
                            window.draw(digits[flagDigit3]);

                            //Drawing
                            for (int i =0; i<drawings.size(); i++){
                                window.draw(*drawings.at(i));
                            }

                            for (int i =0; i<buttons.size(); i++){
                                buttons[i].draw(window);
                            }


                            // Draw the updated tiles
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).Draw(window);
                                }
                            }

                            window.display();

                            if (!LeaderScreen(username, 999, 999, columns, rows, font)) {
                                paused = false;
                                wasPaused = true;
                            }

                            pause.setTexture("files/images/pause.png");
                            for (int row = 0; row < rows; row++) {
                                for (int col = 0; col < columns; col++) {
                                    board.getTileSprite(row, col).setPaused(false);
                                }
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !lose && !win) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int mouseCol = mousePos.x / tileSize;
                int mouseRow = mousePos.y / tileSize;
                if (mouseCol >= 0 && mouseCol < columns && mouseRow >= 0 && mouseRow < rows) {
                    // Retrieve the corresponding tile
                    Tile& tile = board.getTile(mouseRow, mouseCol);
                    // If the tile is flagged, unflag it


                    if (tile.getFlagged()) {
                        tile.setFlagged(false);
                        // Update the corresponding TileSprite
                        board.getTileSprite(mouseRow, mouseCol).setFlagged(false);
                        flagCounter++;
                        board.syncTiles();
                    }
                        // Update the logic tile to handle unflagging only if it was previously flagged
                    else if (!tile.getRevealed()) {
                        // If the tile is not revealed, it should not be flagged
                        tile.setFlagged(true);  // Flag the tile
                        flagCounter --;
                        // Update the corresponding TileSprite
                        board.getTileSprite(mouseRow, mouseCol).setFlagged(true);
                        board.syncTiles();
                    }
                }
            }
        }

        if (lose){
            paused = true;
        }
        if (!timerStarted){
            clock.restart();
        }
        if (!paused) {
            if (wasPaused) {
                totalPausedTime += clock.getElapsedTime();
                wasPaused = false;
            }

            sf::Time frameElapsed = clock.restart();
            elapsedTime += frameElapsed;
        }

        sf::Time adjustedElapsedTime = elapsedTime - totalPausedTime;

        int totalSeconds = static_cast<int>(adjustedElapsedTime.asSeconds());
        int minutes = totalSeconds/60;
        int seconds = totalSeconds %60;

        // Get individual digits
        int digit1 = minutes / 10;
        int digit2 = minutes % 10;
        int digit3 = seconds/10;
        int digit4= seconds % 10;

        int flagCounterAbs = abs(flagCounter); // Get the absolute value of flagCounter

        // Get individual digits for flag counter
        int flagDigit1 = 0;
        if (flagCounter < 0){
            flagDigit1 = 10;
        }
        int flagDigit2 = (flagCounterAbs % 100) / 10; // Tens place
        int flagDigit3 = flagCounterAbs % 10; // Ones place

        if (win && !writeTofile){
            ofstream leaderboardFile("files/leaderboard.txt", ios::app);
            if (!leaderboardFile.is_open()) {
                cout << "Failed to open leaderboard file for writing." << endl;
                return;
            }

            string timeString = to_string(digit1) + to_string(digit2) + ":" + to_string(digit3) + to_string(digit4);

            // Write the new leaderboard entry in the same format
            leaderboardFile << timeString << ", " << username << endl;

            // Close the file
            leaderboardFile.close();
            writeTofile = true;
        }



        //Drawing
        window.clear(sf::Color::White);

        /*if (win) {
            cout << to_string(digit1) << to_string(digit2) << to_string(digit3) << to_string(digit4) << endl;
        }*/

        // Draw the digits
        digits[digit1].setPosition(columns * 32 - 97, 32 * (rows + 0.5) + 16);
        window.draw(digits[digit1]);

        digits[digit2].setPosition(columns * 32 - 97 + 21, 32 * (rows + 0.5) + 16);
        window.draw(digits[digit2]);

        digits[digit3].setPosition(startX,startY);
        window.draw(digits[digit3]);

        digits[digit4].setPosition(startX + 21 ,startY);
        window.draw(digits[digit4]);

        // Draw the digits for flag counter
        digits[flagDigit1].setPosition(12, 32 * (rows + 0.5f)+16 ); // Adjust position for first digit
        window.draw(digits[flagDigit1]);

        digits[flagDigit2].setPosition(33, 32 * (rows + 0.5f)+16);
        window.draw(digits[flagDigit2]);

        digits[flagDigit3].setPosition(33 + 21, 32 * (rows + 0.5f)+16);
        window.draw(digits[flagDigit3]);

        //Drawing
        for (int i =0; i<drawings.size(); i++){
            window.draw(*drawings.at(i));
        }
        for (int i =0; i<buttons.size(); i++){
            buttons[i].draw(window);
        }


// Draw the updated tiles
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                board.getTileSprite(row, col).Draw(window);
            }
        }

        window.display();

    }
}