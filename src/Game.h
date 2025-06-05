#pragma once
#include "Renderer.h"
#include "InputManager.h"
#include "CombatSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "PathFinding.h"
#include "Templates.h"
#include <memory>
#include <vector>
#include <chrono>

/**
 * Main Game class that integrates all systems
 * Demonstrates composition and system integration
 */
class Game {
private:
    // Core systems
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputManager> inputManager;
    std::unique_ptr<InputContextManager> contextManager;
    std::unique_ptr<CombatSystem> combatSystem;
    std::unique_ptr<PathFinder> pathFinder;
    
    // Game entities
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    Inventory<std::shared_ptr<Item>> worldItems;
    
    // Game state
    enum class GameState {
        MainMenu,
        Playing,
        Inventory,
        Combat,
        Paused,
        GameOver
    };
    
    GameState currentState;
    GameState previousState;
    
    // Timing
    std::chrono::steady_clock::time_point lastFrameTime;
    float deltaTime;
    float totalTime;
    
    // Game world
    std::vector<std::vector<int>> gameMap;
    int mapWidth, mapHeight;
    sf::Vector2i playerPosition;
    
    // UI elements
    sf::Text healthText;
    sf::Text levelText;
    sf::Text inventoryText;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    
    // Game statistics
    int enemiesKilled;
    int itemsCollected;
    int roomsExplored;
    float gameTime;

public:
    Game();
    ~Game() = default;

    // Core game loop
    bool initialize();
    void run();
    void shutdown();
    
    // Game loop components
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    // State management
    void changeState(GameState newState);
    GameState getCurrentState() const { return currentState; }
    
    // Game initialization
    void initializePlayer();
    void initializeEnemies();
    void initializeWorld();
    void setupInputCallbacks();
    void loadResources();
    
    // Game logic
    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateWorld(float deltaTime);
    void checkCollisions();
    void processPlayerMovement();
    
    // UI updates
    void updateUI();
    void renderUI();
    void renderMainMenu();
    void renderInventory();
    void renderGameOver();
    
    // Player actions
    void movePlayer(int dx, int dy);
    void playerAttack();
    void usePlayerItem(int itemIndex);
    void openInventory();
    void closeInventory();
    
    // Game events
    void onPlayerDeath();
    void onEnemyDeath(std::shared_ptr<Enemy> enemy);
    void onItemCollected(std::shared_ptr<Item> item);
    void onRoomEntered(int roomId);
    
    // Utility functions
    bool isValidPosition(int x, int y) const;
    std::vector<std::shared_ptr<Enemy>> getEnemiesAt(int x, int y) const;
    std::shared_ptr<Item> getItemAt(int x, int y) const;
    void spawnEnemy(const std::string& type, int x, int y);
    void spawnItem(std::shared_ptr<Item> item, int x, int y);
    
    // Save/Load system
    void saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    
    // Statistics
    void resetStatistics();
    void printGameStatistics() const;

private:
    void calculateDeltaTime();
    void initializeUI();
    void setupGameMap();
    void createSampleEnemies();
    void createSampleItems();
    
    // Input handling helpers
    void handleMainMenuInput();
    void handleGameplayInput();
    void handleInventoryInput();
    void handleCombatInput();
    
    // Rendering helpers
    void renderGame();
    void renderMap();
    void renderEntities();
    void renderEffects();
};

