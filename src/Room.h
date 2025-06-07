#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Renderable.h"
#include "Enemy.h"
#include "Item.h"

// Forward declarations
class Player;

// Base Room class
class Room : public Renderable {
protected:
    int roomID;
    std::string description;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Item>> items;
    std::map<std::string, std::shared_ptr<Room>> connections;  // Direction -> Room
    bool visited;
    std::string roomType;

public:
    Room(int id, const std::string& desc, const std::string& type = "normal");
    virtual ~Room() = default;

    // Renderable interface
    void render() const override;
    std::string getDisplayInfo() const override;

    // Room management
    void addEnemy(std::shared_ptr<Enemy> enemy);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(size_t index);
    void connectRoom(const std::string& direction, std::shared_ptr<Room> room);
    
    // Virtual methods for different room behaviors
    virtual void onEnter(Player& player);
    virtual void onExit(Player& player);
    virtual bool canEnter(const Player& player) const;

    // Getters
    int getRoomID() const { return roomID; }
    const std::string& getDescription() const { return description; }
    const std::string& getRoomType() const { return roomType; }
    bool isVisited() const { return visited; }
    bool hasEnemies() const { return !enemies.empty(); }
    bool hasItems() const { return !items.empty(); }
    
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::shared_ptr<Item>>& getItems() const { return items; }
    const std::map<std::string, std::shared_ptr<Room>>& getConnections() const { return connections; }
    
    std::shared_ptr<Room> getConnectedRoom(const std::string& direction) const;
    std::vector<std::string> getAvailableDirections() const;

    // Mark as visited
    void setVisited(bool vis = true) { visited = vis; }

    // Operator overloading
    bool operator==(const Room& other) const;
    bool operator!=(const Room& other) const;
};

// Specialized room types
class TrapRoom : public Room {
private:
    std::string trapMessage;
    int trapDamage;
    bool trapTriggered;

public:
    TrapRoom(int id, const std::string& desc, const std::string& trapMsg, int damage);
    
    void onEnter(Player& player) override;
    void render() const override;
    
    bool isTrapTriggered() const { return trapTriggered; }
    void resetTrap() { trapTriggered = false; }
};

class TreasureRoom : public Room {
private:
    int goldAmount;
    bool treasureLooted;

public:
    TreasureRoom(int id, const std::string& desc, int gold);
    
    void onEnter(Player& player) override;
    void render() const override;
    bool canEnter(const Player& player) const override;
    
    bool isTreasureLooted() const { return treasureLooted; }
};

class BossRoom : public Room {
private:
    bool bossDefeated;
    std::shared_ptr<Enemy> boss;

public:
    BossRoom(int id, const std::string& desc, std::shared_ptr<Enemy> bossEnemy);
    
    void onEnter(Player& player) override;
    void render() const override;
    bool canEnter(const Player& player) const override;
    
    bool isBossDefeated() const { return bossDefeated; }
    std::shared_ptr<Enemy> getBoss() const { return boss; }
};

class ShopRoom : public Room {
private:
    std::vector<std::pair<std::shared_ptr<Item>, int>> shopItems;  // Item, Price
    
public:
    ShopRoom(int id, const std::string& desc);
    
    void onEnter(Player& player) override;
    void render() const override;
    
    void addShopItem(std::shared_ptr<Item> item, int price);
    void displayShop() const;
    bool buyItem(Player& player, size_t itemIndex);
};

#endif // ROOM_H

