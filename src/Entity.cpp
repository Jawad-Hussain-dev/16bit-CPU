#include "Entity.h"
#include <iostream>
#include <algorithm>

Entity::Entity(const std::string& entityName, int hp, const Position& pos)
    : name(entityName), health(hp), maxHealth(hp), position(pos), alive(true) {
}

void Entity::heal(int amount) {
    if (amount > 0) {
        health = std::min(health + amount, maxHealth);
    }
}

bool Entity::isAlive() const {
    return alive && health > 0;
}

void Entity::setPosition(const Position& pos) {
    position = pos;
}

bool Entity::operator==(const Entity& other) const {
    return name == other.name && position == other.position;
}

bool Entity::operator!=(const Entity& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Entity& entity) {
    os << entity.name << " [HP: " << entity.health << "/" << entity.maxHealth 
       << ", Pos: " << entity.position << "]";
    return os;
}

