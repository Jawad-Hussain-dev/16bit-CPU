#include "Enemy.h"
#include <iostream>
#include <random>

// Base Enemy class implementation
Enemy::Enemy(const std::string& enemyName, int hp, int attack, int def, const std::string& desc)
    : name(enemyName), health(hp), maxHealth(hp), attackPower(attack), defense(def), description(desc) {}

int Enemy::attack() {
    if (!isAlive()) return 0;
    
    std::cout << getAttackMessage() << std::endl;
    
    // 10% chance for special attack
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    
    if (dis(gen) == 1 && hasSpecialAbility()) {
        return getSpecialAttackDamage();
    }
    
    return attackPower;
}

void Enemy::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defense);
    health -= actualDamage;
    if (health < 0) health = 0;
    
    std::cout << "\033[31m" << name << " takes " << actualDamage << " damage! ";
    std::cout << "Health: " << health << "/" << maxHealth << "\033[0m" << std::endl;
}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::getHealth() const {
    return health;
}

void Enemy::render() const {
    std::cout << "\033[91m👹 " << name << "\033[0m" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Attack: " << attackPower << " | Defense: " << defense << std::endl;
    if (!description.empty()) {
        std::cout << description << std::endl;
    }
}

std::string Enemy::getDisplayInfo() const {
    return name + " (HP: " + std::to_string(health) + "/" + std::to_string(maxHealth) + 
           ", ATK: " + std::to_string(attackPower) + ", DEF: " + std::to_string(defense) + ")";
}

std::string Enemy::getAttackMessage() const {
    return "\033[91m" + name + " attacks for " + std::to_string(attackPower) + " damage!\033[0m";
}

int Enemy::getSpecialAttackDamage() {
    return attackPower * 2;  // Default special attack is double damage
}

bool Enemy::operator==(const Enemy& other) const {
    return name == other.name && maxHealth == other.maxHealth;
}

bool Enemy::operator!=(const Enemy& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << enemy.getDisplayInfo();
    return os;
}

// Goblin implementation
Goblin::Goblin() : Enemy("Goblin", 30, 8, 1, "A small, sneaky creature with sharp claws") {}

std::string Goblin::getAttackMessage() const {
    return "\033[91m🗡️ " + name + " slashes with its claws for " + std::to_string(attackPower) + " damage!\033[0m";
}

int Goblin::getSpecialAttackDamage() {
    std::cout << "\033[93m⚡ " << name << " performs a quick strike!\033[0m" << std::endl;
    return attackPower + 5;  // Quick strike bonus
}

// Orc implementation
Orc::Orc() : Enemy("Orc", 60, 15, 3, "A large, brutish warrior with immense strength"), isEnraged(false) {}

std::string Orc::getAttackMessage() const {
    if (isEnraged) {
        return "\033[91m💢 Enraged " + name + " smashes for " + std::to_string(attackPower + 10) + " damage!\033[0m";
    }
    return "\033[91m🔨 " + name + " swings its club for " + std::to_string(attackPower) + " damage!\033[0m";
}

int Orc::getSpecialAttackDamage() {
    if (!isEnraged) {
        enrage();
        return attackPower;
    }
    std::cout << "\033[93m💥 " << name << " performs a devastating blow!\033[0m" << std::endl;
    return attackPower + 20;
}

void Orc::enrage() {
    isEnraged = true;
    attackPower += 10;
    std::cout << "\033[91m💢 " << name << " becomes enraged! Attack power increased!\033[0m" << std::endl;
}

// Dragon implementation
Dragon::Dragon() : Enemy("Ancient Dragon", 150, 25, 8, "A massive, fire-breathing dragon with scales like armor"), 
                   canBreatheFire(true), fireBreathCooldown(0) {}

std::string Dragon::getAttackMessage() const {
    return "\033[91m🐉 " + name + " attacks with claws and fangs for " + std::to_string(attackPower) + " damage!\033[0m";
}

int Dragon::getSpecialAttackDamage() {
    if (canBreatheFire && fireBreathCooldown == 0) {
        std::cout << "\033[93m🔥 " << name << " breathes fire!\033[0m" << std::endl;
        fireBreathCooldown = 3;  // Cooldown for fire breath
        return attackPower * 2;
    }
    
    if (fireBreathCooldown > 0) {
        fireBreathCooldown--;
    }
    
    std::cout << "\033[93m⚡ " << name << " performs a tail swipe!\033[0m" << std::endl;
    return attackPower + 10;
}

void Dragon::render() const {
    std::cout << "\033[91m🐉 ===== " << name << " ===== 🐉\033[0m" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Attack: " << attackPower << " | Defense: " << defense << std::endl;
    std::cout << "\033[93m" << description << "\033[0m" << std::endl;
    if (canBreatheFire) {
        std::cout << "\033[91m🔥 Fire Breath Available\033[0m" << std::endl;
    }
}

// Witch implementation
Witch::Witch() : Enemy("Dark Witch", 45, 12, 2, "A mysterious spellcaster with dark magic"), manaPoints(3) {}

std::string Witch::getAttackMessage() const {
    return "\033[91m🔮 " + name + " casts a dark spell for " + std::to_string(attackPower) + " damage!\033[0m";
}

int Witch::getSpecialAttackDamage() {
    if (manaPoints > 0) {
        castSpell();
        return attackPower + 15;
    }
    return attackPower;
}

void Witch::castSpell() {
    if (manaPoints > 0) {
        manaPoints--;
        std::cout << "\033[93m✨ " << name << " casts a powerful spell! (Mana: " << manaPoints << ")\033[0m" << std::endl;
    }
}

// Factory function implementation
std::unique_ptr<Enemy> createEnemy(const std::string& type) {
    if (type == "Goblin") {
        return std::make_unique<Goblin>();
    } else if (type == "Orc") {
        return std::make_unique<Orc>();
    } else if (type == "Dragon") {
        return std::make_unique<Dragon>();
    } else if (type == "Witch") {
        return std::make_unique<Witch>();
    }
    
    // Default to Goblin if unknown type
    return std::make_unique<Goblin>();
}

