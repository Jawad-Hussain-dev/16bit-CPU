#include "Item.h"
#include "Player.h"
#include <iostream>

// Item base class implementation
Item::Item(const std::string& itemName, const std::string& desc, int itemValue)
    : name(itemName), description(desc), value(itemValue) {}

void Item::render() const {
    std::cout << "\033[33m[" << name << "]\033[0m - " << description << std::endl;
}

std::string Item::getDisplayInfo() const {
    return name + " (" + description + ") - Value: " + std::to_string(value);
}

bool Item::operator==(const Item& other) const {
    return name == other.name && value == other.value;
}

bool Item::operator!=(const Item& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.getDisplayInfo();
    return os;
}

// Weapon class implementation
Weapon::Weapon(const std::string& name, const std::string& desc, int value, int dmg, int dur)
    : Item(name, desc, value), damage(dmg), durability(dur) {}

void Weapon::use(Player& player) {
    // Suppress unused parameter warning
    (void)player;
    
    if (isBroken()) {
        std::cout << "\033[31m" << name << " is broken and cannot be used!\033[0m" << std::endl;
        return;
    }
    
    std::cout << "\033[32mYou equip " << name << " (+" << damage << " damage)\033[0m" << std::endl;
    reduceDurability();
}

std::string Weapon::getDescription() const {
    return description + " | Damage: " + std::to_string(damage) + 
           " | Durability: " + std::to_string(durability);
}

void Weapon::reduceDurability(int amount) {
    durability = std::max(0, durability - amount);
    if (isBroken()) {
        std::cout << "\033[31m" << name << " has broken!\033[0m" << std::endl;
    }
}

// Potion class implementation
Potion::Potion(const std::string& name, const std::string& desc, int value, int healing)
    : Item(name, desc, value), healingAmount(healing) {}

void Potion::use(Player& player) {
    std::cout << "\033[32mYou drink " << name << " and recover " << healingAmount << " health!\033[0m" << std::endl;
    player.heal(healingAmount);
}

std::string Potion::getDescription() const {
    return description + " | Healing: " + std::to_string(healingAmount);
}

// Treasure class implementation
Treasure::Treasure(const std::string& name, const std::string& desc, int value)
    : Item(name, desc, value) {}

void Treasure::render() const {
    std::cout << "\033[93m✨ [" << name << "] ✨\033[0m - " << description 
              << " (Value: " << value << " gold)" << std::endl;
}
