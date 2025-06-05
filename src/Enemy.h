#pragma once
#include "GameEntity.h"
#include <memory>
#include <vector>

// Forward declarations
class Player;
class Item;

/**
 * Base Enemy class demonstrating inheritance and polymorphism
 */
class Enemy : public GameEntity {
protected:
    int attackPower;
    int experienceReward;
    std::vector<std::shared_ptr<Item>> loot;
    float attackCooldown;
    float currentCooldown;

public:
    Enemy(const std::string& name, int health, int attack, int expReward, const Position& pos = Position(0, 0));
    virtual ~Enemy() = default;

    // Pure virtual functions for different enemy types
    virtual std::string getType() const override = 0;
    virtual void performAI(const Player& player) = 0;
    virtual int attack() = 0;

    // Implement Entity interface
    void update(float deltaTime) override;
    void takeDamage(int damage) override;

    // Enemy-specific methods
    void addLoot(std::shared_ptr<Item> item);
    std::vector<std::shared_ptr<Item>> dropLoot();
    bool canAttack() const;
    void resetCooldown();

    // Getters
    int getAttackPower() const { return attackPower; }
    int getExperienceReward() const { return experienceReward; }
    const std::vector<std::shared_ptr<Item>>& getLoot() const { return loot; }

protected:
    virtual void onDeath() {}
};

/**
 * Goblin class - basic enemy
 */
class Goblin : public Enemy {
private:
    bool isAngry;

public:
    Goblin(const std::string& name, const Position& pos = Position(0, 0));

    std::string getType() const override { return "Goblin"; }
    void performAI(const Player& player) override;
    int attack() override;

protected:
    void onDeath() override;
};

/**
 * Witch class - magical enemy
 */
class Witch : public Enemy {
private:
    int mana;
    int maxMana;
    bool canCastSpell;

public:
    Witch(const std::string& name, const Position& pos = Position(0, 0));

    std::string getType() const override { return "Witch"; }
    void performAI(const Player& player) override;
    int attack() override;

    void castSpell();
    bool hasMana() const { return mana >= 20; }

protected:
    void onDeath() override;
};

/**
 * Boss class - powerful enemy with multiple phases
 */
class Boss : public Enemy {
private:
    int phase;
    int maxPhases;
    std::vector<int> phaseHealthThresholds;
    bool isEnraged;

public:
    Boss(const std::string& name, const Position& pos = Position(0, 0));

    std::string getType() const override { return "Boss"; }
    void performAI(const Player& player) override;
    int attack() override;

    void takeDamage(int damage) override;
    void changePhase();
    int getCurrentPhase() const { return phase; }

protected:
    void onDeath() override;
};

/**
 * Dragon class - flying enemy with special abilities
 */
class Dragon : public Enemy {
private:
    bool isFlying;
    int fireBreathCooldown;
    int currentFireCooldown;

public:
    Dragon(const std::string& name, const Position& pos = Position(0, 0));

    std::string getType() const override { return "Dragon"; }
    void performAI(const Player& player) override;
    int attack() override;

    void breatheFire();
    void toggleFlight();
    bool isInFlight() const { return isFlying; }

protected:
    void onDeath() override;
};

