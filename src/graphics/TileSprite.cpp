#include 'TileSprite.h'

class TileSprite: public sf::Sprite{
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

    void setMineHiddenTexture() {
        mineSprite.setTexture(hidden); // Set mine sprite texture to hidden texture
    }

public:
    TileSprite(){
        hidden.loadFromFile("files/images/tile_hidden.png");
        revealed.loadFromFile("files/images/tile_revealed.png");
        mine.loadFromFile("files/images/mine.png");
        flagged.loadFromFile("files/images/flag.png");

        isRevealed = false;
        isPaused = false;
        isFlagged = false;

        adjacentMines = 0;

    }

    void setAdjacentMines(int count, sf::RenderWindow& window) {
        adjacentMines = count;
    }

    void setHidden(){
        sf::Sprite::setTexture(hidden);
    }

    void setMineHiddenTexture(const sf::Texture& texture) {
        mineSprite.setTexture(texture);
    }

    void setMineTexture(const sf::Texture& texture) {
        mineSprite.setTexture(texture);
    }

    bool getRevealed(){
        return isRevealed;
    }

    void setPaused(bool boolean){
        isPaused = boolean;
    }

    bool getPaused(){
        return isPaused;
    }

    void setRevealed(bool reveal) {
        isRevealed = reveal;
        if (isRevealed) {
            sf::Sprite::setTexture(revealed);
        } else {
            sf::Sprite::setTexture(hidden);
            // Reset mine sprite when tile is not revealed
            setMineHiddenTexture();
        }
    }

    void setMine(bool hasMine) {
        this->hasMine = hasMine;
        if (hasMine && isRevealed) { // Show mine if it's revealed
            mineSprite.setTexture(mine);
        } else {
            // Otherwise, keep it hidden
            mineSprite.setTexture(hidden);
        }
    }

    void setPosition(float x, float y){
        sf::Sprite::setPosition(x, y);
    }

    void setFlagged(bool flagged) {
        isFlagged = flagged;
    }

    void setTextureBasedOnState() {
        if (isPaused){
            sf::Sprite::setTexture(revealed);
            digitSprite.setTexture(revealed);
            flagSprite.setTexture(revealed);
            mineSprite.setTexture(revealed);
        } else {
            if (isRevealed) {
                if (hasMine) {
                    mineSprite.setPosition(getPosition());
                    mineSprite.setTexture(mine);
                } else {
                    sf::Sprite::setTexture(revealed);
                    if (adjacentMines > 0) {
                        std::string filePath = "files/images/number_" + std::to_string(adjacentMines) + ".png";
                        digitTexture.loadFromFile(filePath);
                        digitSprite.setPosition(getPosition());
                        digitSprite.setTexture(digitTexture);
                    }
                }
            } else {
                if (isFlagged) {
                    flagSprite.setPosition(getPosition());
                    flagSprite.setTexture(flagged);
                } else {
                    // If not revealed and not flagged, show the hidden texture
                    sf::Sprite::setTexture(hidden);
                    // Reset other sprites to hidden texture
                    mineSprite.setTexture(hidden);
                    digitSprite.setTexture(hidden);
                    flagSprite.setTexture(hidden);
                }
            }
        }
    }

    void Draw(sf::RenderWindow& window) {
        // Set the texture based on the tile state
        setTextureBasedOnState();

        // Draw the tile sprite first
        window.draw(*this);

        // Draw the flag sprite on top of the tile sprite if it's flagged
        if (isFlagged) {
            window.draw(flagSprite);
        }

        // Draw the mine sprite on top of the flag sprite if it's revealed and has a mine
        if (isRevealed && hasMine) {
            window.draw(mineSprite);
        }

        // Draw the digit sprite
        window.draw(digitSprite);

    }

};