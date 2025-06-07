#include "Room.h"
#include "Player.h"
#include "Exceptions.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

// Base Room implementation
Room::Room(int id, const std::string& desc, const std::string& type)
    : roomID(id), description(desc), visited(false), roomType(type) {}

void Room::render() const {
    std::cout << "\n\033[34m🏛️ Room " << roomID << ": " << description << "\033[0m" << std::endl;
    
    if (hasEnemies()) {
        std::cout << "\033[91m⚔️ Enemies present: " << enemies.size() << "\033[0m" << std::endl;
    }
    
    if (hasItems()) {
        std::cout << "\033[33m💎 Items here: " << items.size() << "\033[0m" << std::endl;
    }
    
    // Show available directions
    auto directions = getAvailableDirections();
    if (!directions.empty()) {
        std::cout << "\033[32m🧭 Available paths: ";
        for (size_t i = 0; i < directions.size(); ++i) {
            std::cout << directions[i];
            if (i < directions.size() - 1) std::cout << ", ";
        }
        std::cout << "\033[0m" << std::endl;
    }
}

std::string Room::getDisplayInfo() const {
    return "Room " + std::to_string(roomID) + " (" + roomType + "): " + description;
}

void Room::addEnemy(std::shared_ptr<Enemy> enemy) {
    enemies.push_back(enemy);
}

void Room::addItem(std::shared_ptr<Item> item) {
    items.push_back(item);
}

void Room::removeItem(size_t index) {
    if (index < items.size()) {
        items.erase(items.begin() + index);
    }
}

void Room::connectRoom(const std::string& direction, std::shared_ptr<Room> room) {
    connections[direction] = room;
}

void Room::onEnter(Player& player) {
    setVisited(true);
    player.recordRoom(roomID);
    
    // Show items in room
    if (hasItems()) {
        std::cout << "\033[33m💎 Items in this room:\033[0m" << std::endl;
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << "  " << (i + 1) << ". ";
            items[i]->render();
        }
    }
}

void Room::onExit(Player& player) {
    // Default implementation - can be overridden
    // Suppress unused parameter warning
    (void)player;
}

bool Room::canEnter(const Player& player) const {
    // Suppress unused parameter warning
    (void)player;
    return true;  // Default: all rooms can be entered
}

std::shared_ptr<Room> Room::getConnectedRoom(const std::string& direction) const {
    auto it = connections.find(direction);
    return (it != connections.end()) ? it->second : nullptr;
}

std::vector<std::string> Room::getAvailableDirections() const {
    std::vector<std::string> directions;
    for (const auto& connection : connections) {
        directions.push_back(connection.first);
    }
    return directions;
}

bool Room::operator==(const Room& other) const {
    return roomID == other.roomID;
}

bool Room::operator!=(const Room& other) const {
    return !(*this == other);
}

// TrapRoom implementation
TrapRoom::TrapRoom(int id, const std::string& desc, const std::string& trapMsg, int damage)
    : Room(id, desc, "trap"), trapMessage(trapMsg), trapDamage(damage), trapTriggered(false) {}

void TrapRoom::onEnter(Player& player) {
    Room::onEnter(player);  // Call base implementation
    
    if (!trapTriggered) {
        std::cout << "\033[31m⚠️ " << trapMessage << "\033[0m" << std::endl;
        std::cout << "\033[33mQuick! Press '1' to dodge the trap!\033[0m" << std::endl;
        
        auto startTime = std::chrono::steady_clock::now();
        int response;
        std::cin >> response;
        auto endTime = std::chrono::steady_clock::now();
        
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        
        if (response == 1 && elapsed.count() < 3) {
            std::cout << "\033[32m✅ You successfully dodged the trap!\033[0m" << std::endl;
        } else {
            std::cout << "\033[31m💥 Too slow! The trap was triggered!\033[0m" << std::endl;
            player.takeDamage(trapDamage);
        }
        
        trapTriggered = true;
    }
}

void TrapRoom::render() const {
    Room::render();
    if (!trapTriggered) {
        std::cout << "\033[31m⚠️ WARNING: This room contains a trap!\033[0m" << std::endl;
    } else {
        std::cout << "\033[90m⚠️ Trap has been triggered (safe now)\033[0m" << std::endl;
    }
}

// TreasureRoom implementation
TreasureRoom::TreasureRoom(int id, const std::string& desc, int gold)
    : Room(id, desc, "treasure"), goldAmount(gold), treasureLooted(false) {}

void TreasureRoom::onEnter(Player& player) {
    Room::onEnter(player);
    
    if (!treasureLooted) {
        std::cout << "\033[93m💰 You found " << goldAmount << " gold!\033[0m" << std::endl;
        player.addGold(goldAmount);
        treasureLooted = true;
    }
}

void TreasureRoom::render() const {
    Room::render();
    if (!treasureLooted) {
        std::cout << "\033[93m💰 This room contains treasure!\033[0m" << std::endl;
    } else {
        std::cout << "\033[90m💰 Treasure has been looted\033[0m" << std::endl;
    }
}

bool TreasureRoom::canEnter(const Player& player) const {
    return player.isAlive();
}

// BossRoom implementation
BossRoom::BossRoom(int id, const std::string& desc, std::shared_ptr<Enemy> bossEnemy)
    : Room(id, desc, "boss"), bossDefeated(false), boss(bossEnemy) {
    if (boss) {
        addEnemy(boss);
    }
}

void BossRoom::onEnter(Player& player) {
    Room::onEnter(player);
    
    if (!bossDefeated && boss && boss->isAlive()) {
        std::cout << "\033[91m🐉 A powerful boss blocks your path!\033[0m" << std::endl;
        boss->render();
    }
}

void BossRoom::render() const {
    Room::render();
    if (!bossDefeated && boss && boss->isAlive()) {
        std::cout << "\033[91m🐉 BOSS ROOM - Prepare for battle!\033[0m" << std::endl;
    } else {
        std::cout << "\033[32m✅ Boss has been defeated\033[0m" << std::endl;
    }
}

bool BossRoom::canEnter(const Player& player) const {
    return player.isAlive() && player.getLevel() >= 2;  // Require minimum level
}

// ShopRoom implementation
ShopRoom::ShopRoom(int id, const std::string& desc)
    : Room(id, desc, "shop") {}

void ShopRoom::onEnter(Player& player) {
    Room::onEnter(player);
    
    std::cout << "\033[36m🏪 Welcome to the shop! Browse our wares:\033[0m" << std::endl;
    displayShop();
    
    std::cout << "\033[32mYour gold: " << player.getGold() << "\033[0m" << std::endl;
    std::cout << "Enter item number to buy (0 to leave): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(shopItems.size())) {
        buyItem(player, choice - 1);
    }
}

void ShopRoom::render() const {
    Room::render();
    std::cout << "\033[36m🏪 SHOP - Buy items with gold\033[0m" << std::endl;
}

void ShopRoom::addShopItem(std::shared_ptr<Item> item, int price) {
    shopItems.emplace_back(item, price);
}

void ShopRoom::displayShop() const {
    if (shopItems.empty()) {
        std::cout << "  Shop is empty!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < shopItems.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        shopItems[i].first->render();
        std::cout << "     Price: " << shopItems[i].second << " gold" << std::endl;
    }
}

bool ShopRoom::buyItem(Player& player, size_t itemIndex) {
    if (itemIndex >= shopItems.size()) {
        std::cout << "\033[31mInvalid item selection!\033[0m" << std::endl;
        return false;
    }
    
    const auto& shopItem = shopItems[itemIndex];
    
    try {
        player.spendGold(shopItem.second);
        player.addItem(shopItem.first);
        std::cout << "\033[32m✅ Purchased " << shopItem.first->getName() << "!\033[0m" << std::endl;
        
        // Remove item from shop (one-time purchase)
        shopItems.erase(shopItems.begin() + itemIndex);
        return true;
    } catch (const GameStateException& e) {
        std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
        return false;
    }
}
