#pragma once
#include <string>
#include <memory>

/**
 * Base Item class demonstrating inheritance hierarchy
 */
class Item {
protected:
    std::string name;
    std::string description;
    int value;
    bool consumable;

public:
    Item(const std::string& itemName, const std::string& desc, int itemValue, bool isConsumable = false);
    virtual ~Item() = default;

    // Pure virtual function for item usage
    virtual void use() = 0;
    virtual std::string getType() const = 0;

    // Getters
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getValue() const { return value; }
    bool isConsumable() const { return consumable; }

    // Operator overloading for item comparison
    bool operator==(const Item& other) const;
    bool operator!=(const Item& other) const;
};

/**
 * Weapon class - inherits from Item
 */
class Weapon : public Item {
private:
    int damage;
    int durability;
    int maxDurability;

public:
    Weapon(const std::string& name, const std::string& desc, int value, 
           int weaponDamage, int weaponDurability);

    void use() override;
    std::string getType() const override { return "Weapon"; }

    int getDamage() const { return damage; }
    int getDurability() const { return durability; }
    int getMaxDurability() const { return maxDurability; }
    bool isBroken() const { return durability <= 0; }

    void repair(int amount);
};

/**
 * Potion class - inherits from Item
 */
class Potion : public Item {
private:
    int healAmount;

public:
    Potion(const std::string& name, const std::string& desc, int value, int healing);

    void use() override;
    std::string getType() const override { return "Potion"; }

    int getHealAmount() const { return healAmount; }
};

/**
 * Treasure class - inherits from Item
 */
class Treasure : public Item {
private:
    int experienceBonus;

public:
    Treasure(const std::string& name, const std::string& desc, int value, int expBonus = 0);

    void use() override;
    std::string getType() const override { return "Treasure"; }

    int getExperienceBonus() const { return experienceBonus; }
};

/**
 * Key class - inherits from Item
 */
class Key : public Item {
private:
    std::string doorId;

public:
    Key(const std::string& name, const std::string& desc, int value, const std::string& door);

    void use() override;
    std::string getType() const override { return "Key"; }

    const std::string& getDoorId() const { return doorId; }
};

