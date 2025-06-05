#include "Player.h"
#include "Item.h"
#include "GameExceptions.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

// Base Player implementation
Player::Player(const std::string& name, int health, int moves)
    : GameEntity(name, health), movesLeft(moves), maxMoves(moves), 
      experience(0), level(1), inventory(20) {
}

void Player::update(float deltaTime) {
    updateAnimation(deltaTime);
    // Update any status effects, cooldowns, etc.
}

void Player::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= damage;
    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << name << " has died! Game Over!\n";
    }
}

int Player::attack() {
    if (!hasMoves()) {
        throw InvalidMoveException("No moves left to attack");
    }
    
    int baseDamage = getAttackPower();
    int weaponDamage = equippedWeapon ? equippedWeapon->getDamage() : 0;
    
    useMove();
    return baseDamage + weaponDamage;
}

void Player::addItem(std::shared_ptr<Item> item) {
    if (!inventory.addItem(item)) {
        throw InventoryException("Inventory is full");
    }
    std::cout << item->getName() << " added to inventory!\n";
}

void Player::useMove() {
    if (movesLeft <= 0) {
        throw InvalidMoveException("No moves left");
    }
    movesLeft--;
}

void Player::gainExperience(int exp) {
    experience += exp;
    // Check for level up (every 100 exp)
    int newLevel = (experience / 100) + 1;
    if (newLevel > level) {
        level = newLevel;
        levelUp();
    }
}

void Player::recordPosition() {
    pathHistory.push(position);
}

Position Player::getLastPosition() {
    if (pathHistory.empty()) {
        throw std::runtime_error("No previous position recorded");
    }
    Position lastPos = pathHistory.top();
    pathHistory.pop();
    return lastPos;
}

void Player::equipWeapon(std::shared_ptr<Weapon> weapon) {
    equippedWeapon = weapon;
    std::cout << "Equipped " << weapon->getName() << "\n";
}

bool Player::operator>(const Player& other) const {
    return level > other.level || (level == other.level && experience > other.experience);
}

bool Player::operator<(const Player& other) const {
    return level < other.level || (level == other.level && experience < other.experience);
}

// Warrior implementation
Warrior::Warrior(const std::string& name) 
    : Player(name, 120, 8), rage(0), maxRage(100) {
    loadTexture("assets/textures/warrior.png");
}

int Warrior::getAttackPower() const {
    int baseAttack = 15 + (level * 3);
    int rageBonus = (rage / 10) * 2; // +2 damage per 10 rage
    return baseAttack + rageBonus;
}

void Warrior::levelUp() {
    maxHealth += 20;
    health = maxHealth;
    maxRage += 10;
    std::cout << name << " leveled up! Now level " << level << "\n";
    std::cout << "Health increased to " << maxHealth << "\n";
}

void Warrior::useSpecialAbility() {
    if (rage >= 50) {
        rage -= 50;
        std::cout << name << " enters a berserker rage!\n";
        // Temporary damage boost would be handled in combat system
    } else {
        std::cout << "Not enough rage! Need 50, have " << rage << "\n";
    }
}

void Warrior::buildRage(int amount) {
    rage = std::min(rage + amount, maxRage);
}

// Mage implementation
Mage::Mage(const std::string& name) 
    : Player(name, 80, 12), mana(100), maxMana(100) {
    loadTexture("assets/textures/mage.png");
}

int Mage::getAttackPower() const {
    return 8 + (level * 2) + (mana / 20); // Mana affects damage
}

void Mage::levelUp() {
    maxHealth += 10;
    health = maxHealth;
    maxMana += 20;
    mana = maxMana;
    std::cout << name << " leveled up! Now level " << level << "\n";
    std::cout << "Mana increased to " << maxMana << "\n";
}

void Mage::useSpecialAbility() {
    if (mana >= 30) {
        useMana(30);
        std::cout << name << " casts Magic Missile!\n";
        // Special attack would be handled in combat system
    } else {
        std::cout << "Not enough mana! Need 30, have " << mana << "\n";
    }
}

void Mage::useMana(int amount) {
    mana = std::max(0, mana - amount);
}

void Mage::restoreMana(int amount) {
    mana = std::min(mana + amount, maxMana);
}

// Rogue implementation
Rogue::Rogue(const std::string& name) 
    : Player(name, 90, 15), stealth(0), isHidden(false) {
    loadTexture("assets/textures/rogue.png");
}

int Rogue::getAttackPower() const {
    int baseAttack = 12 + (level * 2);
    return isHidden ? baseAttack * 2 : baseAttack; // Double damage when stealthed
}

void Rogue::levelUp() {
    maxHealth += 15;
    health = maxHealth;
    maxMoves += 2;
    movesLeft = maxMoves;
    std::cout << name << " leveled up! Now level " << level << "\n";
    std::cout << "Moves increased to " << maxMoves << "\n";
}

void Rogue::useSpecialAbility() {
    if (!isHidden) {
        enterStealth();
    } else {
        std::cout << name << " is already hidden!\n";
    }
}

void Rogue::enterStealth() {
    isHidden = true;
    std::cout << name << " vanishes into the shadows!\n";
}

void Rogue::exitStealth() {
    isHidden = false;
    std::cout << name << " emerges from stealth!\n";
}

