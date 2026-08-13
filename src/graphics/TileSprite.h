#pragma once

#include <SFML/Graphics.hpp>

class TileSprite : public sf::Sprite
{
private:
    sf::Sprite digitSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;

    sf::Texture hidden;
    sf::Texture revealed;
    sf::Texture flagged;
    sf::Texture mine;
    sf::Texture digitTexture;

    bool isRevealed;
    bool isFlagged;
    bool hasMine;
    bool isPaused;

    int adjacentMines;

    void setMineHiddenTexture();

public:
    TileSprite();

    void setAdjacentMines(int count, sf::RenderWindow& window);

    void setHidden();
    void setMineHiddenTexture(const sf::Texture& texture);
    void setMineTexture(const sf::Texture& texture);

    bool getRevealed();

    void setPaused(bool boolean);
    bool getPaused();

    void setRevealed(bool reveal);
    void setMine(bool hasMine);
    void setPosition(float x, float y);
    void setFlagged(bool flagged);

    void setTextureBasedOnState();

    void Draw(sf::RenderWindow& window);
};