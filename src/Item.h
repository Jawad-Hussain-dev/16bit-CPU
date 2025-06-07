#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
#include "Renderable.h"
#include "Usable.h"

// Forward declaration
class Player;

// Base Item class implementing Renderable
class Item : public Renderable {
protected:
    std::string name;
    std::string description;
    int value;

public:
    Item(const std::string& itemName, const std::string& desc, int itemValue);
    virtual ~Item() = default;

    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getValue() const { return value; }

    // Renderable interface implementation
    void render() const override;
    std::string getDisplayInfo() const override;

    // Operator overloading
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

// Weapon class - inherits from Item and implements Usable
class Weapon : public Item, public Usable {
private:
    int damage;
    int durability;

public:
    Weapon(const std::string& name, const std::string& desc, int value, int dmg, int dur);

    // Usable interface implementation
    void use(Player& player) override;
    std::string getDescription() const override;
    bool isConsumable() const override { return false; }

    // Weapon-specific methods
    int getDamage() const { return damage; }
    int getDurability() const { return durability; }
    void reduceDurability(int amount = 1);
    bool isBroken() const { return durability <= 0; }
};

// Potion class - inherits from Item and implements Usable
class Potion : public Item, public Usable {
private:
    int healingAmount;

public:
    Potion(const std::string& name, const std::string& desc, int value, int healing);

    // Usable interface implementation
    void use(Player& player) override;
    std::string getDescription() const override;
    bool isConsumable() const override { return true; }

    // Potion-specific methods
    int getHealingAmount() const { return healingAmount; }
};

// Treasure class - simple valuable item
class Treasure : public Item {
public:
    Treasure(const std::string& name, const std::string& desc, int value);
    
    // Override render for special treasure display
    void render() const override;
};

#endif // ITEM_H

