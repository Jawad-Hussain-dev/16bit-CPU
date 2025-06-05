#include "GameEntity.h"
#include <iostream>

GameEntity::GameEntity(const std::string& name, int health, const Position& pos)
    : Entity(name, health, pos), textureLoaded(false) {
    updateSpritePosition();
}

void GameEntity::draw(sf::RenderWindow& window) {
    if (textureLoaded && isAlive()) {
        window.draw(sprite);
    }
}

void GameEntity::loadTexture(const std::string& texturePath) {
    if (texture.loadFromFile(texturePath)) {
        sprite.setTexture(texture);
        textureLoaded = true;
        updateSpritePosition();
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        textureLoaded = false;
    }
}

void GameEntity::setSprite(const sf::Sprite& newSprite) {
    sprite = newSprite;
    textureLoaded = true;
    updateSpritePosition();
}

void GameEntity::setPosition(const Position& pos) {
    Entity::setPosition(pos);
    updateSpritePosition();
}

void GameEntity::updateSpritePosition() {
    // Convert game coordinates to screen coordinates
    // Assuming each tile is 32x32 pixels
    const float TILE_SIZE = 32.0f;
    sprite.setPosition(position.getX() * TILE_SIZE, position.getY() * TILE_SIZE);
}

