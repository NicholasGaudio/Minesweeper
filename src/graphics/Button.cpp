#include 'Button.h'

class Button: public sf::Sprite {
private:
    sf::FloatRect bounds;
    sf::Texture texture;

public:
    Button(const std::string& filename, const sf::Vector2f& position) {
        if (!texture.loadFromFile(filename)) {
            cout << "Failed to load texture from file: " << filename << endl;
            return;
        }

        sf::Sprite::setTexture(texture);
        sf::Sprite::setPosition(position);
        bounds = sf::Sprite::getGlobalBounds();
    }

    bool isClicked(const sf::Vector2i& mousePosition) {
        return bounds.contains(sf::Vector2f(mousePosition));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(*this);
    }

    void setTexture(const string &filename){
        if (!texture.loadFromFile(filename)) {
            cout << "Failed to load texture from file: " << filename << endl;
            return;
        }
        sf::Sprite::setTexture(texture);
    }

};