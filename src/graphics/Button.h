#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button : public sf::Sprite
{
private:
    sf::FloatRect bounds;
    sf::Texture texture;

public:
    Button(const std::string& filename,
           const sf::Vector2f& position);

    bool isClicked(const sf::Vector2i& mousePosition);

    void draw(sf::RenderWindow& window);

    void setTexture(const std::string& filename);
};