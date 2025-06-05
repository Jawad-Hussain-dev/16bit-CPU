#include "Item.h"
#include <iostream>
#include <algorithm>

// Base Item implementation
Item::Item(const std::string& itemName, const std::string& desc, int itemValue, bool isConsumable)
    : name(itemName), description(desc), value(itemValue), consumable(isConsumable) {
}

bool Item::operator==(const Item& other) const {
    return name == other.name && value == other.value;
}

bool Item::operator!=(const Item& other) const {
    return !(*this == other);
}

// Weapon implementation
Weapon::Weapon(const std::string& name, const std::string& desc, int value, 
               int weaponDamage, int weaponDurability)
    : Item(name, desc, value, false), damage(weaponDamage), 
      durability(weaponDurability), maxDurability(weaponDurability) {
}

void Weapon::use() {
    if (durability > 0) {
        durability--;
        std::cout << name << " used. Durability: " << durability << "/" << maxDurability << "\n";
        
        if (durability <= 0) {
            std::cout << name << " has broken!\n";
        }
    } else {
        std::cout << name << " is broken and cannot be used!\n";
    }
}

void Weapon::repair(int amount) {
    durability = std::min(durability + amount, maxDurability);
    std::cout << name << " repaired. Durability: " << durability << "/" << maxDurability << "\n";
}

// Potion implementation
Potion::Potion(const std::string& name, const std::string& desc, int value, int healing)
    : Item(name, desc, value, true), healAmount(healing) {
}

void Potion::use() {
    std::cout << "Used " << name << " and restored " << healAmount << " health!\n";
    // Actual healing would be handled by the player/game system
}

// Treasure implementation
Treasure::Treasure(const std::string& name, const std::string& desc, int value, int expBonus)
    : Item(name, desc, value, false), experienceBonus(expBonus) {
}

void Treasure::use() {
    std::cout << "Examined " << name << ". " << description << "\n";
    if (experienceBonus > 0) {
        std::cout << "Gained " << experienceBonus << " experience!\n";
    }
}

// Key implementation
Key::Key(const std::string& name, const std::string& desc, int value, const std::string& door)
    : Item(name, desc, value, true), doorId(door) {
}

void Key::use() {
    std::cout << "Used " << name << " to unlock door: " << doorId << "\n";
}

