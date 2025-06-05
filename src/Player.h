#pragma once
#include "GameEntity.h"
#include "Templates.h"
#include <stack>
#include <string>
#include <memory>

// Forward declarations
class Item;
class Weapon;

/**
 * Enhanced Player class with inheritance and polymorphism
 */
class Player : public GameEntity {
protected:
    int movesLeft;
    int maxMoves;
    int experience;
    int level;
    Inventory<std::shared_ptr<Item>> inventory;
    std::stack<Position> pathHistory;
    std::shared_ptr<Weapon> equippedWeapon;

public:
    Player(const std::string& name, int health = 100, int moves = 10);
    virtual ~Player() = default;

    // Pure virtual functions for different player types
    virtual std::string getType() const override = 0;
    virtual int getAttackPower() const = 0;
    virtual void levelUp() = 0;

    // Implement Entity interface
    void update(float deltaTime) override;
    void takeDamage(int damage) override;

    // Player-specific methods
    virtual int attack();
    virtual void useSpecialAbility() = 0;
    void addItem(std::shared_ptr<Item> item);
    void useMove();
    void gainExperience(int exp);
    void recordPosition();
    Position getLastPosition();
    
    // Equipment management
    void equipWeapon(std::shared_ptr<Weapon> weapon);
    std::shared_ptr<Weapon> getEquippedWeapon() const { return equippedWeapon; }

    // Getters
    int getMovesLeft() const { return movesLeft; }
    int getMaxMoves() const { return maxMoves; }
    int getExperience() const { return experience; }
    int getLevel() const { return level; }
    const Inventory<std::shared_ptr<Item>>& getInventory() const { return inventory; }
    bool hasMoves() const { return movesLeft > 0; }

    // Operator overloading for player comparison
    bool operator>(const Player& other) const;
    bool operator<(const Player& other) const;
};

/**
 * Warrior class - demonstrates inheritance
 */
class Warrior : public Player {
private:
    int rage;
    int maxRage;

public:
    Warrior(const std::string& name);
    
    std::string getType() const override { return "Warrior"; }
    int getAttackPower() const override;
    void levelUp() override;
    void useSpecialAbility() override; // Berserker rage
    
    void buildRage(int amount);
    int getRage() const { return rage; }
};

/**
 * Mage class - demonstrates inheritance
 */
class Mage : public Player {
private:
    int mana;
    int maxMana;

public:
    Mage(const std::string& name);
    
    std::string getType() const override { return "Mage"; }
    int getAttackPower() const override;
    void levelUp() override;
    void useSpecialAbility() override; // Magic missile
    
    void useMana(int amount);
    void restoreMana(int amount);
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; }
};

/**
 * Rogue class - demonstrates inheritance
 */
class Rogue : public Player {
private:
    int stealth;
    bool isHidden;

public:
    Rogue(const std::string& name);
    
    std::string getType() const override { return "Rogue"; }
    int getAttackPower() const override;
    void levelUp() override;
    void useSpecialAbility() override; // Stealth attack
    
    void enterStealth();
    void exitStealth();
    bool isStealthed() const { return isHidden; }
};

