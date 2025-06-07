#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <memory>
#include <queue>
#include <stack>
#include <vector>
#include <map>
#include <functional>
#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Leaderboard.h"
#include "Container.h"

class GameManager {
private:
    std::unique_ptr<Player> player;
    std::shared_ptr<Room> currentRoom;
    std::map<int, std::shared_ptr<Room>> allRooms;
    std::queue<std::shared_ptr<Enemy>> combatQueue;
    std::stack<std::shared_ptr<Room>> roomHistory;
    Leaderboard leaderboard;
    
    // Game state
    bool gameRunning;
    bool gameWon;
    int totalScore;
    
    // Event system using lambdas
    std::map<std::string, std::function<void()>> eventHandlers;

public:
    explicit GameManager(const std::string& playerName);
    ~GameManager() = default;

    // Game flow
    void startGame();
    void gameLoop();
    void endGame();
    
    // Room management
    void addRoom(std::shared_ptr<Room> room);
    void moveToRoom(std::shared_ptr<Room> room);
    void moveInDirection(const std::string& direction);
    void handleRoomInteraction();
    
    // Combat system
    void initiateCombat(std::shared_ptr<Enemy> enemy);
    void handleCombat();
    bool processCombatTurn(std::shared_ptr<Enemy> enemy);
    void onEnemyDefeated(std::shared_ptr<Enemy> enemy);
    
    // Player actions
    void handlePlayerAction();
    void showGameMenu();
    void handleInventoryAction();
    void handleMovementAction();
    void handleCombatAction();
    
    // Game utilities
    void displayGameState() const;
    void displayRoomInfo() const;
    void calculateFinalScore();
    void saveHighScore();
    
    // Event system
    void registerEventHandler(const std::string& event, std::function<void()> handler);
    void triggerEvent(const std::string& event);
    
    // Getters
    Player* getPlayer() const { return player.get(); }
    std::shared_ptr<Room> getCurrentRoom() const { return currentRoom; }
    bool isGameRunning() const { return gameRunning; }
    bool isGameWon() const { return gameWon; }
    int getScore() const { return totalScore; }

private:
    void initializeGame();
    void createDungeon();
    void setupEventHandlers();
    void processInput(int choice);
    bool checkWinCondition() const;
    bool checkLoseCondition() const;
    void showHelp() const;
    
    // Lambda functions for common operations
    std::function<void(const std::string&)> displayMessage;
    std::function<bool(const std::string&)> confirmAction;
};

#endif // GAMEMANAGER_H

