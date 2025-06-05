#pragma once
#include <SFML/Graphics.hpp>

/**
 * Abstract interface for drawable objects
 * Demonstrates interface segregation principle
 */
class Drawable {
public:
    virtual ~Drawable() = default;
    
    // Pure virtual function for rendering
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Virtual functions for sprite management
    virtual void loadTexture(const std::string& texturePath) {}
    virtual void setSprite(const sf::Sprite& sprite) {}
    virtual sf::Sprite& getSprite() = 0;
};

