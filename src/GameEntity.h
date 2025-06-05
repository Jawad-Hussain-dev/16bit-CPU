#pragma once
#include "Entity.h"
#include "Drawable.h"
#include "Updatable.h"
#include <SFML/Graphics.hpp>

/**
 * GameEntity combines Entity with rendering capabilities
 * Demonstrates multiple inheritance and interface implementation
 */
class GameEntity : public Entity, public Drawable, public Updatable {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    bool textureLoaded;

public:
    GameEntity(const std::string& name, int health, const Position& pos = Position(0, 0));
    virtual ~GameEntity() = default;

    // Implement Drawable interface
    void draw(sf::RenderWindow& window) override;
    void loadTexture(const std::string& texturePath) override;
    void setSprite(const sf::Sprite& newSprite) override;
    sf::Sprite& getSprite() override { return sprite; }

    // Update sprite position when entity position changes
    void setPosition(const Position& pos) override;
    
    // Virtual function for animation updates
    virtual void updateAnimation(float deltaTime) {}

protected:
    void updateSpritePosition();
};

