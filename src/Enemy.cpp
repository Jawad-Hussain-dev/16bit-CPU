#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

// Base Enemy implementation
Enemy::Enemy(const std::string& name, int health, int attack, int expReward, const Position& pos)
    : GameEntity(name, health, pos), attackPower(attack), experienceReward(expReward),
      attackCooldown(2.0f), currentCooldown(0.0f) {
}

void Enemy::update(float deltaTime) {
    updateAnimation(deltaTime);
    
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
        if (currentCooldown < 0) {
            currentCooldown = 0;
        }
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        alive = false;
        onDeath();
        std::cout << name << " has been defeated!\n";
    }
}

void Enemy::addLoot(std::shared_ptr<Item> item) {
    loot.push_back(item);
}

std::vector<std::shared_ptr<Item>> Enemy::dropLoot() {
    std::vector<std::shared_ptr<Item>> droppedLoot = loot;
    loot.clear();
    return droppedLoot;
}

bool Enemy::canAttack() const {
    return currentCooldown <= 0 && isAlive();
}

void Enemy::resetCooldown() {
    currentCooldown = attackCooldown;
}

// Goblin implementation
Goblin::Goblin(const std::string& name, const Position& pos)
    : Enemy(name, 40, 8, 25, pos), isAngry(false) {
    loadTexture("assets/textures/goblin.png");
    
    // Add some loot
    addLoot(std::make_shared<Treasure>("Goblin Tooth", "A sharp goblin tooth", 5, 5));
    if (rand() % 3 == 0) { // 33% chance
        addLoot(std::make_shared<Weapon>("Rusty Dagger", "A worn goblin dagger", 15, 6, 20));
    }
}

void Goblin::performAI(const Player& player) {
    // Simple AI: get angry when health is low
    if (health < maxHealth / 2 && !isAngry) {
        isAngry = true;
        attackPower += 3;
        std::cout << name << " becomes enraged!\n";
    }
    
    // Move towards player (simplified)
    Position playerPos = player.getPosition();
    if (position.manhattanDistance(playerPos) > 1) {
        // Move closer to player
        Position direction = playerPos - position;
        if (abs(direction.getX()) > abs(direction.getY())) {
            position.setX(position.getX() + (direction.getX() > 0 ? 1 : -1));
        } else {
            position.setY(position.getY() + (direction.getY() > 0 ? 1 : -1));
        }
        updateSpritePosition();
    }
}

int Goblin::attack() {
    if (!canAttack()) return 0;
    
    resetCooldown();
    std::cout << name << " swipes with claws!\n";
    return attackPower + (rand() % 3); // Random variation
}

void Goblin::onDeath() {
    std::cout << name << " drops some items!\n";
}

// Witch implementation
Witch::Witch(const std::string& name, const Position& pos)
    : Enemy(name, 60, 12, 50, pos), mana(80), maxMana(80), canCastSpell(true) {
    loadTexture("assets/textures/witch.png");
    
    // Add magical loot
    addLoot(std::make_shared<Potion>("Mana Potion", "Restores magical energy", 25, 30));
    addLoot(std::make_shared<Treasure>("Crystal Orb", "A mystical crystal orb", 50, 15));
}

void Witch::performAI(const Player& player) {
    // Witch prefers to keep distance and cast spells
    Position playerPos = player.getPosition();
    int distance = position.manhattanDistance(playerPos);
    
    if (distance < 2) {
        // Too close, move away
        Position direction = position - playerPos;
        if (direction.getX() != 0) {
            position.setX(position.getX() + (direction.getX() > 0 ? 1 : -1));
        } else if (direction.getY() != 0) {
            position.setY(position.getY() + (direction.getY() > 0 ? 1 : -1));
        }
        updateSpritePosition();
    }
    
    // Restore mana over time
    if (mana < maxMana) {
        mana = std::min(mana + 2, maxMana);
    }
}

int Witch::attack() {
    if (!canAttack()) return 0;
    
    resetCooldown();
    
    if (hasMana() && rand() % 2 == 0) {
        castSpell();
        return attackPower + 8; // Spell damage bonus
    } else {
        std::cout << name << " strikes with her staff!\n";
        return attackPower;
    }
}

void Witch::castSpell() {
    mana -= 20;
    std::cout << name << " casts a dark spell!\n";
}

void Witch::onDeath() {
    std::cout << name << " dissolves into shadow, leaving magical items!\n";
}

// Boss implementation
Boss::Boss(const std::string& name, const Position& pos)
    : Enemy(name, 150, 20, 200, pos), phase(1), maxPhases(3), isEnraged(false) {
    loadTexture("assets/textures/boss.png");
    
    // Set phase thresholds
    phaseHealthThresholds = {100, 50, 0}; // Phase changes at 100 HP and 50 HP
    
    // Add valuable loot
    addLoot(std::make_shared<Weapon>("Boss Sword", "A legendary weapon", 100, 25, 50));
    addLoot(std::make_shared<Treasure>("Boss Crown", "Crown of the defeated boss", 200, 100));
    addLoot(std::make_shared<Key>("Master Key", "Opens all doors", 50, "master"));
}

void Boss::performAI(const Player& player) {
    // Boss AI changes based on phase
    switch (phase) {
        case 1:
            // Phase 1: Normal attacks
            break;
        case 2:
            // Phase 2: More aggressive
            attackPower = 25;
            attackCooldown = 1.5f;
            break;
        case 3:
            // Phase 3: Desperate attacks
            if (!isEnraged) {
                isEnraged = true;
                attackPower = 35;
                attackCooldown = 1.0f;
                std::cout << name << " enters a final rage!\n";
            }
            break;
    }
}

int Boss::attack() {
    if (!canAttack()) return 0;
    
    resetCooldown();
    
    switch (phase) {
        case 1:
            std::cout << name << " swings a massive weapon!\n";
            return attackPower;
        case 2:
            std::cout << name << " unleashes a devastating combo!\n";
            return attackPower + (rand() % 10);
        case 3:
            std::cout << name << " attacks with desperate fury!\n";
            return attackPower + (rand() % 15);
        default:
            return attackPower;
    }
}

void Boss::takeDamage(int damage) {
    int oldHealth = health;
    Enemy::takeDamage(damage);
    
    // Check for phase changes
    for (int i = 0; i < phaseHealthThresholds.size(); i++) {
        if (oldHealth > phaseHealthThresholds[i] && health <= phaseHealthThresholds[i]) {
            if (phase < maxPhases) {
                changePhase();
            }
            break;
        }
    }
}

void Boss::changePhase() {
    phase++;
    std::cout << name << " enters phase " << phase << "!\n";
    
    // Heal slightly on phase change
    heal(20);
}

void Boss::onDeath() {
    std::cout << name << " falls with a thunderous crash, dropping legendary treasures!\n";
}

// Dragon implementation
Dragon::Dragon(const std::string& name, const Position& pos)
    : Enemy(name, 200, 30, 300, pos), isFlying(true), fireBreathCooldown(5), currentFireCooldown(0) {
    loadTexture("assets/textures/dragon.png");
    
    // Add dragon-specific loot
    addLoot(std::make_shared<Treasure>("Dragon Scale", "A shimmering dragon scale", 75, 50));
    addLoot(std::make_shared<Weapon>("Dragon Claw", "A razor-sharp claw", 150, 30, 40));
    addLoot(std::make_shared<Treasure>("Dragon Heart", "The heart of an ancient dragon", 500, 200));
}

void Dragon::performAI(const Player& player) {
    // Dragon flies around and uses fire breath
    if (currentFireCooldown > 0) {
        currentFireCooldown--;
    }
    
    // Occasionally land or take off
    if (rand() % 20 == 0) {
        toggleFlight();
    }
    
    // Use fire breath if available
    if (currentFireCooldown <= 0 && rand() % 3 == 0) {
        breatheFire();
    }
}

int Dragon::attack() {
    if (!canAttack()) return 0;
    
    resetCooldown();
    
    if (isFlying) {
        std::cout << name << " swoops down with a devastating aerial attack!\n";
        return attackPower + 10;
    } else {
        std::cout << name << " slashes with massive claws!\n";
        return attackPower;
    }
}

void Dragon::breatheFire() {
    currentFireCooldown = fireBreathCooldown;
    std::cout << name << " breathes a torrent of fire!\n";
}

void Dragon::toggleFlight() {
    isFlying = !isFlying;
    if (isFlying) {
        std::cout << name << " takes to the air!\n";
    } else {
        std::cout << name << " lands with a heavy thud!\n";
    }
}

void Dragon::onDeath() {
    std::cout << name << " crashes to the ground, its treasure hoard scattered!\n";
}

