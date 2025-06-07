#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <memory>
#include "Combatable.h"
#include "Renderable.h"

// Base Enemy class implementing both Combatable and Renderable interfaces
class Enemy : public Combatable, public Renderable {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    std::string description;

public:
    Enemy(const std::string& enemyName, int hp, int attack, int def = 0, const std::string& desc = "");
    virtual ~Enemy() = default;

    // Combatable interface implementation
    int attack() override;
    void takeDamage(int damage) override;
    bool isAlive() const override;
    int getHealth() const override;

    // Renderable interface implementation
    void render() const override;
    std::string getDisplayInfo() const override;

    // Virtual methods for polymorphic behavior
    virtual std::string getAttackMessage() const;
    virtual int getSpecialAttackDamage();
    virtual bool hasSpecialAbility() const { return false; }

    // Getters
    const std::string& getName() const { return name; }
    int getMaxHealth() const { return maxHealth; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defense; }

    // Operator overloading
    bool operator==(const Enemy& other) const;
    bool operator!=(const Enemy& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);
};

// Goblin - weak but fast enemy
class Goblin : public Enemy {
public:
    Goblin();
    std::string getAttackMessage() const override;
    int getSpecialAttackDamage() override;
};

// Orc - strong melee enemy
class Orc : public Enemy {
private:
    bool isEnraged;

public:
    Orc();
    std::string getAttackMessage() const override;
    int getSpecialAttackDamage() override;
    bool hasSpecialAbility() const override { return true; }
    void enrage();
};

// Dragon - powerful boss enemy
class Dragon : public Enemy {
private:
    bool canBreatheFire;
    int fireBreathCooldown;

public:
    Dragon();
    std::string getAttackMessage() const override;
    int getSpecialAttackDamage() override;
    bool hasSpecialAbility() const override { return canBreatheFire; }
    void render() const override;  // Special rendering for dragon
};

// Witch - magical enemy with special abilities
class Witch : public Enemy {
private:
    int manaPoints;

public:
    Witch();
    std::string getAttackMessage() const override;
    int getSpecialAttackDamage() override;
    bool hasSpecialAbility() const override { return manaPoints > 0; }
    void castSpell();
};

// Factory function for creating enemies
std::unique_ptr<Enemy> createEnemy(const std::string& type);

#endif // ENEMY_H

