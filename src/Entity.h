#pragma once
#include <string>
#include <memory>
#include "Position.h"

/**
 * Abstract base class for all game entities
 * Demonstrates pure virtual functions and abstract class design
 */
class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    Position position;
    bool alive;

public:
    Entity(const std::string& entityName, int hp, const Position& pos = Position(0, 0));
    virtual ~Entity() = default;

    // Pure virtual functions - makes this an abstract class
    virtual void update(float deltaTime) = 0;
    virtual void takeDamage(int damage) = 0;
    virtual std::string getType() const = 0;

    // Virtual functions that can be overridden
    virtual void heal(int amount);
    virtual bool isAlive() const;
    virtual void setPosition(const Position& pos);

    // Getters
    const std::string& getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    const Position& getPosition() const { return position; }

    // Operator overloading for entity comparison
    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
    
    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);
};

