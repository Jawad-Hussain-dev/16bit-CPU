#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include <stack>
#include <map>
#include "Combatable.h"
#include "Renderable.h"
#include "Container.h"
#include "Item.h"

// Forward declarations
class Weapon;
class Potion;

class Player : public Combatable, public Renderable {
private:
    std::string name;
    int health;
    int maxHealth;
    int movesLeft;
    int maxMoves;
    int experience;
    int level;
    int gold;
    
    // Inventory using template container
    Container<std::shared_ptr<Item>> inventory;
    std::shared_ptr<Weapon> equippedWeapon;
    
    // Path tracking for backtracking
    std::stack<int> roomPath;
    
    // Statistics tracking
    std::map<std::string, int> statistics;

public:
    Player(const std::string& playerName, int startHealth = 100, int maxMoves = 15);
    virtual ~Player() = default;

    // Combatable interface implementation
    int attack() override;
    void takeDamage(int damage) override;
    bool isAlive() const override;
    int getHealth() const override;

    // Renderable interface implementation
    void render() const override;
    std::string getDisplayInfo() const override;

    // Inventory management
    void addItem(std::shared_ptr<Item> item);
    void removeItem(size_t index);
    void useItem(size_t index);
    void showInventory() const;
    bool hasItem(const std::string& itemName) const;

    // Equipment management
    void equipWeapon(std::shared_ptr<Weapon> weapon);
    void unequipWeapon();
    std::shared_ptr<Weapon> getEquippedWeapon() const { return equippedWeapon; }

    // Health and movement
    void heal(int amount);
    void useMove();
    bool hasMoves() const;
    void restoreMoves(int amount = -1);  // -1 means restore all

    // Experience and leveling
    void gainExperience(int exp);
    void levelUp();
    int getLevel() const { return level; }
    int getExperience() const { return experience; }

    // Gold management
    void addGold(int amount) { gold += amount; }
    void spendGold(int amount);
    int getGold() const { return gold; }

    // Path tracking
    void recordRoom(int roomID);
    void backtrack();
    bool canBacktrack() const;

    // Statistics
    void updateStatistic(const std::string& stat, int value);
    int getStatistic(const std::string& stat) const;
    void displayStatistics() const;

    // Getters
    const std::string& getName() const { return name; }
    int getMaxHealth() const { return maxHealth; }
    int getMovesLeft() const { return movesLeft; }
    int getMaxMoves() const { return maxMoves; }
    size_t getInventorySize() const { return inventory.size(); }

    // Operator overloading
    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif // PLAYER_H

