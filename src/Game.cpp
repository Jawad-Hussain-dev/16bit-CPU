#include "Game.h"
#include "Goblin.h"
#include "Witch.h"
#include "Boss.h"
#include "Warrior.h"
#include "Mage.h"
#include "Rogue.h"
#include "Item.h"
#include "GameExceptions.h"
#include <iostream>
#include <random>
#include <fstream>

Game::Game() 
    : currentState(GameState::MainMenu), previousState(GameState::MainMenu),
      deltaTime(0.0f), totalTime(0.0f), mapWidth(20), mapHeight(15),
      playerPosition(1, 1), enemiesKilled(0), itemsCollected(0), 
      roomsExplored(0), gameTime(0.0f), worldItems(50) {
}

bool Game::initialize() {
    try {
        // Initialize renderer
        renderer = std::make_unique<Renderer>(1024, 768, "Enhanced Dungeon Crawler");
        if (!renderer->initialize()) {
            throw GameException("Failed to initialize renderer");
        }
        
        // Initialize input system
        inputManager = std::make_unique<InputManager>();
        contextManager = std::make_unique<InputContextManager>(inputManager.get());
        
        // Initialize combat system
        combatSystem = std::make_unique<CombatSystem>();
        
        // Initialize pathfinding
        pathFinder = std::make_unique<PathFinder>(mapWidth, mapHeight);
        
        // Load resources
        loadResources();
        
        // Initialize game world
        initializeWorld();
        initializePlayer();
        initializeEnemies();
        
        // Setup input callbacks
        setupInputCallbacks();
        
        // Initialize UI
        initializeUI();
        
        // Setup input contexts
        contextManager->addContext("MainMenu");
        contextManager->addContext("Gameplay");
        contextManager->addContext("Inventory");
        contextManager->addContext("Combat");
        
        contextManager->pushContext("MainMenu");
        
        lastFrameTime = std::chrono::steady_clock::now();
        
        std::cout << "Game initialized successfully!\n";
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize game: " << e.what() << std::endl;
        return false;
    }
}

void Game::run() {
    while (renderer->isOpen()) {
        calculateDeltaTime();
        
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::shutdown() {
    if (renderer) {
        renderer->shutdown();
    }
    
    std::cout << "Game shut down successfully\n";
}

void Game::handleEvents() {
    sf::Event event;
    while (renderer->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            renderer->close();
            return;
        }
        
        inputManager->handleEvent(event);
        
        // Handle state-specific events
        switch (currentState) {
            case GameState::MainMenu:
                handleMainMenuInput();
                break;
            case GameState::Playing:
                handleGameplayInput();
                break;
            case GameState::Inventory:
                handleInventoryInput();
                break;
            case GameState::Combat:
                handleCombatInput();
                break;
            default:
                break;
        }
    }
    
    inputManager->update();
}

void Game::update(float dt) {
    totalTime += dt;
    gameTime += dt;
    
    switch (currentState) {
        case GameState::Playing:
            updatePlayer(dt);
            updateEnemies(dt);
            updateWorld(dt);
            checkCollisions();
            break;
            
        case GameState::Combat:
            combatSystem->processActions(dt);
            break;
            
        default:
            break;
    }
    
    updateUI();
}

void Game::render() {
    renderer->clear();
    
    switch (currentState) {
        case GameState::MainMenu:
            renderMainMenu();
            break;
            
        case GameState::Playing:
        case GameState::Combat:
            renderGame();
            break;
            
        case GameState::Inventory:
            renderGame(); // Render game in background
            renderInventory();
            break;
            
        case GameState::GameOver:
            renderGameOver();
            break;
    }
    
    renderUI();
    renderer->display();
}

void Game::changeState(GameState newState) {
    previousState = currentState;
    currentState = newState;
    
    // Handle state transitions
    switch (newState) {
        case GameState::MainMenu:
            contextManager->clearContexts();
            contextManager->pushContext("MainMenu");
            break;
            
        case GameState::Playing:
            contextManager->clearContexts();
            contextManager->pushContext("Gameplay");
            break;
            
        case GameState::Inventory:
            contextManager->pushContext("Inventory");
            break;
            
        case GameState::Combat:
            contextManager->clearContexts();
            contextManager->pushContext("Combat");
            break;
            
        case GameState::GameOver:
            contextManager->clearContexts();
            printGameStatistics();
            break;
    }
}

void Game::initializePlayer() {
    // Create a warrior player (could be made selectable)
    player = std::make_shared<Warrior>("Hero");
    player->setPosition(Position(playerPosition.x, playerPosition.y));
    player->loadTexture("assets/textures/warrior.png");
    
    // Give player some starting items
    player->addItem(std::make_shared<Potion>("Health Potion", "Restores 50 HP", 25, 50));
    player->addItem(std::make_shared<Weapon>("Iron Sword", "A sturdy iron sword", 50, 15, 30));
    
    std::cout << "Player initialized: " << player->getName() << " (" << player->getType() << ")\n";
}

void Game::initializeEnemies() {
    createSampleEnemies();
}

void Game::initializeWorld() {
    setupGameMap();
    createSampleItems();
}

void Game::setupInputCallbacks() {
    // Movement callbacks using lambdas
    inputManager->onKeyPressed(sf::Keyboard::W, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) movePlayer(0, -1);
    });
    
    inputManager->onKeyPressed(sf::Keyboard::S, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) movePlayer(0, 1);
    });
    
    inputManager->onKeyPressed(sf::Keyboard::A, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) movePlayer(-1, 0);
    });
    
    inputManager->onKeyPressed(sf::Keyboard::D, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) movePlayer(1, 0);
    });
    
    // Action callbacks
    inputManager->onKeyPressed(sf::Keyboard::Space, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) playerAttack();
    });
    
    inputManager->onKeyPressed(sf::Keyboard::I, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) openInventory();
        else if (currentState == GameState::Inventory) closeInventory();
    });
    
    inputManager->onKeyPressed(sf::Keyboard::Escape, [this](sf::Keyboard::Key) {
        if (currentState == GameState::Playing) changeState(GameState::MainMenu);
        else if (currentState == GameState::Inventory) closeInventory();
    });
    
    inputManager->onKeyPressed(sf::Keyboard::Enter, [this](sf::Keyboard::Key) {
        if (currentState == GameState::MainMenu) changeState(GameState::Playing);
    });
}

void Game::loadResources() {
    // Load textures
    renderer->loadTexture("warrior", "assets/textures/warrior.png");
    renderer->loadTexture("mage", "assets/textures/mage.png");
    renderer->loadTexture("rogue", "assets/textures/rogue.png");
    renderer->loadTexture("goblin", "assets/textures/goblin.png");
    renderer->loadTexture("witch", "assets/textures/witch.png");
    renderer->loadTexture("boss", "assets/textures/boss.png");
    renderer->loadTexture("wall", "assets/textures/wall.png");
    renderer->loadTexture("floor", "assets/textures/floor.png");
    
    // Load fonts
    renderer->loadFont("main", "assets/fonts/arial.ttf");
}

void Game::updatePlayer(float dt) {
    if (player && player->isAlive()) {
        player->update(dt);
        
        // Update camera to follow player
        Position playerPos = player->getPosition();
        renderer->setCameraPosition(playerPos.getX() * 32.0f, playerPos.getY() * 32.0f);
    }
}

void Game::updateEnemies(float dt) {
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->update(dt);
            enemy->performAI(*player);
        }
    }
    
    // Remove dead enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::shared_ptr<Enemy>& enemy) {
                return !enemy || !enemy->isAlive();
            }),
        enemies.end()
    );
}

void Game::updateWorld(float dt) {
    // Update world systems, environmental effects, etc.
}

void Game::checkCollisions() {
    if (!player || !player->isAlive()) return;
    
    Position playerPos = player->getPosition();
    
    // Check enemy collisions
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && enemy->getPosition() == playerPos) {
            // Start combat
            std::vector<std::shared_ptr<Player>> players = {player};
            std::vector<std::shared_ptr<Enemy>> combatEnemies = {enemy};
            combatSystem->startCombat(players, combatEnemies);
            changeState(GameState::Combat);
            break;
        }
    }
}

void Game::processPlayerMovement() {
    // This is handled by input callbacks
}

void Game::updateUI() {
    if (!player) return;
    
    // Update health text
    healthText.setString("HP: " + std::to_string(player->getHealth()) + "/" + 
                        std::to_string(player->getMaxHealth()));
    
    // Update level text
    levelText.setString("Level: " + std::to_string(player->getLevel()) + 
                       " XP: " + std::to_string(player->getExperience()));
    
    // Update health bar
    float healthPercent = static_cast<float>(player->getHealth()) / player->getMaxHealth();
    healthBar.setSize(sf::Vector2f(200.0f * healthPercent, 20.0f));
}

void Game::renderUI() {
    renderer->setUIView();
    
    if (currentState == GameState::Playing || currentState == GameState::Combat) {
        renderer->drawText(healthText);
        renderer->drawText(levelText);
        renderer->drawShape(healthBarBackground);
        renderer->drawShape(healthBar);
    }
}

void Game::renderMainMenu() {
    renderer->setUIView();
    
    sf::Text titleText;
    titleText.setFont(renderer->getFont("main"));
    titleText.setString("Enhanced Dungeon Crawler");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(200, 200);
    
    sf::Text instructionText;
    instructionText.setFont(renderer->getFont("main"));
    instructionText.setString("Press ENTER to start\nPress ESC to quit");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(300, 350);
    
    renderer->drawText(titleText);
    renderer->drawText(instructionText);
}

void Game::renderInventory() {
    renderer->setUIView();
    
    // Draw semi-transparent background
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(renderer->getWidth(), renderer->getHeight()));
    background.setFillColor(sf::Color(0, 0, 0, 128));
    renderer->drawShape(background);
    
    // Draw inventory window
    sf::RectangleShape window;
    window.setSize(sf::Vector2f(400, 300));
    window.setPosition(312, 234);
    window.setFillColor(sf::Color(50, 50, 50));
    window.setOutlineColor(sf::Color::White);
    window.setOutlineThickness(2);
    renderer->drawShape(window);
    
    // Draw inventory title
    sf::Text titleText;
    titleText.setFont(renderer->getFont("main"));
    titleText.setString("Inventory");
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(450, 250);
    renderer->drawText(titleText);
    
    // Draw inventory items
    if (player) {
        const auto& inventory = player->getInventory();
        for (size_t i = 0; i < inventory.size() && i < 10; ++i) {
            sf::Text itemText;
            itemText.setFont(renderer->getFont("main"));
            itemText.setString(std::to_string(i + 1) + ". " + inventory.getItem(i)->getName());
            itemText.setCharacterSize(16);
            itemText.setFillColor(sf::Color::White);
            itemText.setPosition(330, 290 + i * 20);
            renderer->drawText(itemText);
        }
    }
}

void Game::renderGameOver() {
    renderer->setUIView();
    
    sf::Text gameOverText;
    gameOverText.setFont(renderer->getFont("main"));
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(350, 300);
    
    sf::Text statsText;
    statsText.setFont(renderer->getFont("main"));
    statsText.setString("Enemies Killed: " + std::to_string(enemiesKilled) + "\n" +
                       "Items Collected: " + std::to_string(itemsCollected) + "\n" +
                       "Time Played: " + std::to_string(static_cast<int>(gameTime)) + "s");
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(350, 400);
    
    renderer->drawText(gameOverText);
    renderer->drawText(statsText);
}

void Game::movePlayer(int dx, int dy) {
    if (!player || !player->isAlive()) return;
    
    Position currentPos = player->getPosition();
    Position newPos(currentPos.getX() + dx, currentPos.getY() + dy);
    
    if (isValidPosition(newPos.getX(), newPos.getY())) {
        player->setPosition(newPos);
        player->useMove();
        
        if (!player->hasMoves()) {
            onPlayerDeath();
        }
    }
}

void Game::playerAttack() {
    if (!player || !player->isAlive()) return;
    
    Position playerPos = player->getPosition();
    
    // Find enemies in adjacent positions
    std::vector<Position> attackPositions = {
        Position(playerPos.getX() + 1, playerPos.getY()),
        Position(playerPos.getX() - 1, playerPos.getY()),
        Position(playerPos.getX(), playerPos.getY() + 1),
        Position(playerPos.getX(), playerPos.getY() - 1)
    };
    
    for (const Position& pos : attackPositions) {
        auto enemiesAtPos = getEnemiesAt(pos.getX(), pos.getY());
        if (!enemiesAtPos.empty()) {
            // Attack the first enemy found
            auto enemy = enemiesAtPos[0];
            try {
                int damage = player->attack();
                enemy->takeDamage(damage);
                
                std::cout << player->getName() << " attacks " << enemy->getName() 
                         << " for " << damage << " damage!\n";
                
                if (!enemy->isAlive()) {
                    onEnemyDeath(enemy);
                }
            } catch (const InvalidMoveException& e) {
                std::cout << "Cannot attack: " << e.what() << std::endl;
            }
            break;
        }
    }
}

void Game::usePlayerItem(int itemIndex) {
    if (!player) return;
    
    try {
        auto& inventory = player->getInventory();
        if (itemIndex < inventory.size()) {
            auto item = inventory.getItem(itemIndex);
            item->use();
            
            // Handle item effects based on type
            if (auto potion = std::dynamic_pointer_cast<Potion>(item)) {
                player->heal(potion->getHealAmount());
                inventory.removeItem(itemIndex);
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error using item: " << e.what() << std::endl;
    }
}

void Game::openInventory() {
    changeState(GameState::Inventory);
}

void Game::closeInventory() {
    changeState(GameState::Playing);
}

void Game::onPlayerDeath() {
    std::cout << "Player has died!\n";
    changeState(GameState::GameOver);
}

void Game::onEnemyDeath(std::shared_ptr<Enemy> enemy) {
    enemiesKilled++;
    
    // Award experience
    player->gainExperience(enemy->getExperienceReward());
    
    // Drop loot
    auto loot = enemy->dropLoot();
    for (auto& item : loot) {
        try {
            player->addItem(item);
            itemsCollected++;
        } catch (const InventoryException&) {
            // Inventory full, drop item on ground
            spawnItem(item, enemy->getPosition().getX(), enemy->getPosition().getY());
        }
    }
}

void Game::onItemCollected(std::shared_ptr<Item> item) {
    itemsCollected++;
    std::cout << "Collected: " << item->getName() << std::endl;
}

void Game::onRoomEntered(int roomId) {
    roomsExplored++;
    std::cout << "Entered room " << roomId << std::endl;
}

bool Game::isValidPosition(int x, int y) const {
    return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight && 
           gameMap[y][x] == 0; // 0 = walkable, 1 = wall
}

std::vector<std::shared_ptr<Enemy>> Game::getEnemiesAt(int x, int y) const {
    std::vector<std::shared_ptr<Enemy>> result;
    
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && 
            enemy->getPosition().getX() == x && enemy->getPosition().getY() == y) {
            result.push_back(enemy);
        }
    }
    
    return result;
}

std::shared_ptr<Item> Game::getItemAt(int x, int y) const {
    // Implementation would depend on how world items are stored
    return nullptr;
}

void Game::spawnEnemy(const std::string& type, int x, int y) {
    std::shared_ptr<Enemy> enemy;
    
    if (type == "Goblin") {
        enemy = std::make_shared<Goblin>("Goblin", Position(x, y));
    } else if (type == "Witch") {
        enemy = std::make_shared<Witch>("Witch", Position(x, y));
    } else if (type == "Boss") {
        enemy = std::make_shared<Boss>("Boss", Position(x, y));
    }
    
    if (enemy) {
        enemies.push_back(enemy);
    }
}

void Game::spawnItem(std::shared_ptr<Item> item, int x, int y) {
    // Add item to world items at position
    worldItems.addItem(item);
}

void Game::saveGame(const std::string& filename) const {
    // Implementation for save system
    std::cout << "Game saved to " << filename << std::endl;
}

bool Game::loadGame(const std::string& filename) {
    // Implementation for load system
    std::cout << "Game loaded from " << filename << std::endl;
    return true;
}

void Game::resetStatistics() {
    enemiesKilled = 0;
    itemsCollected = 0;
    roomsExplored = 0;
    gameTime = 0.0f;
}

void Game::printGameStatistics() const {
    std::cout << "\n=== Game Statistics ===\n";
    std::cout << "Enemies Killed: " << enemiesKilled << "\n";
    std::cout << "Items Collected: " << itemsCollected << "\n";
    std::cout << "Rooms Explored: " << roomsExplored << "\n";
    std::cout << "Time Played: " << gameTime << " seconds\n";
    std::cout << "======================\n\n";
}

void Game::calculateDeltaTime() {
    auto currentTime = std::chrono::steady_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
    lastFrameTime = currentTime;
    
    // Cap delta time to prevent large jumps
    deltaTime = std::min(deltaTime, 0.016f); // Max 16ms (60 FPS)
}

void Game::initializeUI() {
    try {
        // Initialize health text
        healthText.setFont(renderer->getFont("main"));
        healthText.setCharacterSize(20);
        healthText.setFillColor(sf::Color::White);
        healthText.setPosition(10, 10);
        
        // Initialize level text
        levelText.setFont(renderer->getFont("main"));
        levelText.setCharacterSize(16);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(10, 40);
        
        // Initialize health bar
        healthBarBackground.setSize(sf::Vector2f(200, 20));
        healthBarBackground.setPosition(10, 70);
        healthBarBackground.setFillColor(sf::Color::Red);
        
        healthBar.setSize(sf::Vector2f(200, 20));
        healthBar.setPosition(10, 70);
        healthBar.setFillColor(sf::Color::Green);
        
    } catch (const ResourceLoadException& e) {
        std::cout << "Warning: " << e.what() << " - Using default UI\n";
    }
}

void Game::setupGameMap() {
    gameMap.resize(mapHeight, std::vector<int>(mapWidth, 0));
    
    // Create simple map with walls around the border
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (x == 0 || x == mapWidth - 1 || y == 0 || y == mapHeight - 1) {
                gameMap[y][x] = 1; // Wall
                pathFinder->setObstacle(x, y, true);
            }
        }
    }
    
    // Add some internal walls
    for (int i = 5; i < 15; ++i) {
        gameMap[5][i] = 1;
        pathFinder->setObstacle(i, 5, true);
    }
}

void Game::createSampleEnemies() {
    spawnEnemy("Goblin", 10, 10);
    spawnEnemy("Witch", 15, 8);
    spawnEnemy("Goblin", 8, 12);
}

void Game::createSampleItems() {
    // Create some world items
    worldItems.addItem(std::make_shared<Potion>("Health Potion", "Restores health", 25, 50));
    worldItems.addItem(std::make_shared<Weapon>("Magic Sword", "A powerful sword", 100, 20, 40));
    worldItems.addItem(std::make_shared<Treasure>("Gold Coin", "Valuable currency", 10, 5));
}

void Game::handleMainMenuInput() {
    if (inputManager->isActionPressed("attack")) { // Enter key
        changeState(GameState::Playing);
    }
}

void Game::handleGameplayInput() {
    // Movement is handled by callbacks
    // Other gameplay input can be handled here
}

void Game::handleInventoryInput() {
    // Handle inventory-specific input
    for (int i = 1; i <= 9; ++i) {
        if (inputManager->isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i - 1))) {
            usePlayerItem(i - 1);
            break;
        }
    }
}

void Game::handleCombatInput() {
    // Handle combat-specific input
    if (inputManager->isActionPressed("attack")) {
        // Combat attack logic
    }
}

void Game::renderGame() {
    renderer->setGameView();
    renderMap();
    renderEntities();
    renderEffects();
}

void Game::renderMap() {
    // Render the game map
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            sf::RectangleShape tile;
            tile.setSize(sf::Vector2f(32, 32));
            tile.setPosition(x * 32.0f, y * 32.0f);
            
            if (gameMap[y][x] == 1) {
                tile.setFillColor(sf::Color(100, 100, 100)); // Wall
            } else {
                tile.setFillColor(sf::Color(50, 50, 50)); // Floor
            }
            
            renderer->drawShape(tile);
        }
    }
}

void Game::renderEntities() {
    // Render player
    if (player && player->isAlive()) {
        player->draw(renderer->getWindow());
    }
    
    // Render enemies
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->draw(renderer->getWindow());
        }
    }
}

void Game::renderEffects() {
    // Render visual effects, particles, etc.
}

