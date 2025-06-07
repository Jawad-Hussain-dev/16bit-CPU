#include "Player.h"
#include "Exceptions.h"
#include <iostream>
#include <algorithm>
#include <random>

Player::Player(const std::string& playerName, int startHealth, int maxMoves)
    : name(playerName), health(startHealth), maxHealth(startHealth), 
      movesLeft(maxMoves), maxMoves(maxMoves), experience(0), level(1), gold(0),
      inventory(20), equippedWeapon(nullptr) {
    
    // Initialize statistics
    statistics["enemies_defeated"] = 0;
    statistics["rooms_explored"] = 0;
    statistics["items_collected"] = 0;
    statistics["damage_dealt"] = 0;
    statistics["damage_taken"] = 0;
}

// Combatable interface implementation
int Player::attack() {
    if (!isAlive()) {
        throw CombatException("Dead players cannot attack");
    }

    int baseDamage = 10;  // Base player damage
    int weaponDamage = 0;
    
    if (equippedWeapon && !equippedWeapon->isBroken()) {
        weaponDamage = equippedWeapon->getDamage();
        equippedWeapon->reduceDurability();
        
        if (equippedWeapon->isBroken()) {
            std::cout << "\033[31mYour " << equippedWeapon->getName() << " breaks!\033[0m" << std::endl;
            unequipWeapon();
        }
    }

    // Attack type selection
    std::cout << "\033[32mChoose attack type:\033[0m" << std::endl;
    std::cout << "1. Light Attack (" << (baseDamage + weaponDamage) << " damage, guaranteed)" << std::endl;
    std::cout << "2. Heavy Attack (" << (baseDamage + weaponDamage) * 2 << " damage, 50% chance to miss)" << std::endl;
    
    int choice;
    std::cin >> choice;

    int totalDamage = baseDamage + weaponDamage;

    if (choice == 2) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 2);
        
        if (dis(gen) == 1) {
            std::cout << "\033[31mHeavy attack missed!\033[0m" << std::endl;
            return 0;
        }
        totalDamage *= 2;
        std::cout << "\033[93m💥 Critical hit!\033[0m" << std::endl;
    }

    updateStatistic("damage_dealt", totalDamage);
    return totalDamage;
}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        throw CombatException("Damage cannot be negative");
    }

    health -= damage;
    updateStatistic("damage_taken", damage);
    
    if (health <= 0) {
        health = 0;
        std::cout << "\033[31m💀 " << name << " has died! Game Over!\033[0m" << std::endl;
    } else {
        std::cout << "\033[31m" << name << " takes " << damage << " damage! Health: " << health << "/" << maxHealth << "\033[0m" << std::endl;
    }
}

bool Player::isAlive() const {
    return health > 0;
}

int Player::getHealth() const {
    return health;
}

// Renderable interface implementation
void Player::render() const {
    std::cout << "\033[36m🛡️ === " << name << " === 🛡️\033[0m" << std::endl;
    std::cout << "Level: " << level << " | Experience: " << experience << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Moves Left: " << movesLeft << "/" << maxMoves << std::endl;
    std::cout << "Gold: " << gold << std::endl;
    
    if (equippedWeapon) {
        std::cout << "Equipped: " << equippedWeapon->getName() << std::endl;
    } else {
        std::cout << "No weapon equipped" << std::endl;
    }
}

std::string Player::getDisplayInfo() const {
    return name + " (Lvl " + std::to_string(level) + ", HP: " + std::to_string(health) + 
           "/" + std::to_string(maxHealth) + ", Gold: " + std::to_string(gold) + ")";
}

// Inventory management
void Player::addItem(std::shared_ptr<Item> item) {
    try {
        inventory.add(item);
        updateStatistic("items_collected", 1);
        std::cout << "\033[32m📦 " << item->getName() << " added to inventory!\033[0m" << std::endl;
    } catch (const InventoryException& e) {
        std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
    }
}

void Player::removeItem(size_t index) {
    try {
        inventory.remove(index);
        std::cout << "\033[33mItem removed from inventory.\033[0m" << std::endl;
    } catch (const InventoryException& e) {
        std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
    }
}

void Player::useItem(size_t index) {
    try {
        auto item = inventory.get(index);
        
        // Try to cast to Usable
        auto usableItem = std::dynamic_pointer_cast<Usable>(item);
        if (usableItem) {
            usableItem->use(*this);
            
            // Remove consumable items after use
            if (usableItem->isConsumable()) {
                removeItem(index);
            }
        } else {
            std::cout << "\033[31mThis item cannot be used.\033[0m" << std::endl;
        }
    } catch (const InventoryException& e) {
        std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
    }
}

void Player::showInventory() const {
    std::cout << "\n\033[36m📦 " << name << "'s Inventory (" << inventory.size() << "/" << inventory.capacity() << "):\033[0m" << std::endl;
    
    if (inventory.empty()) {
        std::cout << "  Empty" << std::endl;
        return;
    }

    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        inventory.get(i)->render();
    }
    std::cout << std::endl;
}

bool Player::hasItem(const std::string& itemName) const {
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory.get(i)->getName() == itemName) {
            return true;
        }
    }
    return false;
}

// Equipment management
void Player::equipWeapon(std::shared_ptr<Weapon> weapon) {
    if (equippedWeapon) {
        std::cout << "\033[33mUnequipping " << equippedWeapon->getName() << "\033[0m" << std::endl;
    }
    
    equippedWeapon = weapon;
    std::cout << "\033[32mEquipped " << weapon->getName() << "!\033[0m" << std::endl;
}

void Player::unequipWeapon() {
    if (equippedWeapon) {
        std::cout << "\033[33mUnequipped " << equippedWeapon->getName() << "\033[0m" << std::endl;
        equippedWeapon = nullptr;
    }
}

// Health and movement
void Player::heal(int amount) {
    if (amount < 0) {
        throw GameStateException("Healing amount cannot be negative");
    }

    int oldHealth = health;
    health = std::min(maxHealth, health + amount);
    int actualHealing = health - oldHealth;
    
    if (actualHealing > 0) {
        std::cout << "\033[32m❤️ Healed " << actualHealing << " health points!\033[0m" << std::endl;
    }
}

void Player::useMove() {
    if (movesLeft <= 0) {
        throw InvalidMoveException("No moves remaining");
    }
    movesLeft--;
}

bool Player::hasMoves() const {
    return movesLeft > 0;
}

void Player::restoreMoves(int amount) {
    if (amount == -1) {
        movesLeft = maxMoves;
    } else {
        movesLeft = std::min(maxMoves, movesLeft + amount);
    }
}

// Experience and leveling
void Player::gainExperience(int exp) {
    experience += exp;
    std::cout << "\033[93m⭐ Gained " << exp << " experience!\033[0m" << std::endl;
    
    // Check for level up (100 exp per level)
    int newLevel = (experience / 100) + 1;
    if (newLevel > level) {
        level = newLevel;
        levelUp();
    }
}

void Player::levelUp() {
    std::cout << "\033[93m🎉 LEVEL UP! You are now level " << level << "!\033[0m" << std::endl;
    
    // Increase stats on level up
    int healthIncrease = 20;
    maxHealth += healthIncrease;
    health += healthIncrease;  // Also heal on level up
    maxMoves += 2;
    movesLeft += 2;
    
    std::cout << "\033[32m+ " << healthIncrease << " max health, +2 max moves\033[0m" << std::endl;
}

void Player::spendGold(int amount) {
    if (amount > gold) {
        throw GameStateException("Not enough gold");
    }
    gold -= amount;
}

// Path tracking
void Player::recordRoom(int roomID) {
    roomPath.push(roomID);
    updateStatistic("rooms_explored", 1);
}

void Player::backtrack() {
    if (roomPath.empty()) {
        throw InvalidMoveException("No previous rooms to backtrack to");
    }
    roomPath.pop();
}

bool Player::canBacktrack() const {
    return !roomPath.empty();
}

// Statistics
void Player::updateStatistic(const std::string& stat, int value) {
    statistics[stat] += value;
}

int Player::getStatistic(const std::string& stat) const {
    auto it = statistics.find(stat);
    return (it != statistics.end()) ? it->second : 0;
}

void Player::displayStatistics() const {
    std::cout << "\n\033[36m📊 " << name << "'s Statistics:\033[0m" << std::endl;
    for (const auto& stat : statistics) {
        std::cout << "  " << stat.first << ": " << stat.second << std::endl;
    }
    std::cout << std::endl;
}

// Operator overloading
bool Player::operator==(const Player& other) const {
    return name == other.name && level == other.level;
}

bool Player::operator!=(const Player& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.getDisplayInfo();
    return os;
}

