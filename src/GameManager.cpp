#include "GameManager.h"
#include "Exceptions.h"
#include <iostream>
#include <algorithm>
#include <ctime>

GameManager::GameManager(const std::string& playerName)
    : player(std::make_unique<Player>(playerName)), currentRoom(nullptr),
      leaderboard("leaderboard.txt"), gameRunning(false), gameWon(false), totalScore(0) {
    
    setupEventHandlers();
    initializeGame();
}

void GameManager::startGame() {
    std::cout << "\033[93m🎮 ===== DUNGEON CRAWLER RPG ===== 🎮\033[0m" << std::endl;
    std::cout << "\033[36mWelcome, " << player->getName() << "!\033[0m" << std::endl;
    std::cout << "\033[33mYour quest: Explore the dungeon, defeat enemies, and find the exit!\033[0m" << std::endl;
    
    createDungeon();
    gameRunning = true;
    
    // Start in the first room
    if (!allRooms.empty()) {
        moveToRoom(allRooms.begin()->second);
    }
    
    triggerEvent("game_start");
    gameLoop();
}

void GameManager::gameLoop() {
    while (gameRunning && player->isAlive()) {
        try {
            displayGameState();
            handleRoomInteraction();
            handleCombat();
            
            if (checkWinCondition()) {
                gameWon = true;
                triggerEvent("game_won");
                break;
            }
            
            if (checkLoseCondition()) {
                triggerEvent("game_lost");
                break;
            }
            
            if (player->hasMoves()) {
                handlePlayerAction();
            } else {
                std::cout << "\033[31m⏰ You've run out of moves! Game Over!\033[0m" << std::endl;
                break;
            }
            
        } catch (const std::exception& e) {
            std::cout << "\033[31mError: " << e.what() << "\033[0m" << std::endl;
            std::cout << "Press Enter to continue..." << std::endl;
            std::cin.ignore();
            std::cin.get();
        }
    }
    
    endGame();
}

void GameManager::endGame() {
    gameRunning = false;
    calculateFinalScore();
    
    std::cout << "\n\033[93m🎯 === GAME OVER === 🎯\033[0m" << std::endl;
    
    if (gameWon) {
        std::cout << "\033[32m🎉 Congratulations! You've completed the dungeon!\033[0m" << std::endl;
    } else {
        std::cout << "\033[31m💀 Better luck next time!\033[0m" << std::endl;
    }
    
    std::cout << "\033[36mFinal Score: " << totalScore << "\033[0m" << std::endl;
    player->displayStatistics();
    
    saveHighScore();
    leaderboard.display();
}

void GameManager::addRoom(std::shared_ptr<Room> room) {
    allRooms[room->getRoomID()] = room;
}

void GameManager::moveToRoom(std::shared_ptr<Room> room) {
    if (!room) {
        throw InvalidMoveException("Cannot move to null room");
    }
    
    if (!room->canEnter(*player)) {
        throw InvalidMoveException("Cannot enter this room");
    }
    
    if (currentRoom) {
        currentRoom->onExit(*player);
        roomHistory.push(currentRoom);
    }
    
    currentRoom = room;
    currentRoom->onEnter(*player);
    
    triggerEvent("room_entered");
}

void GameManager::moveInDirection(const std::string& direction) {
    if (!currentRoom) {
        throw GameStateException("No current room");
    }
    
    auto nextRoom = currentRoom->getConnectedRoom(direction);
    if (!nextRoom) {
        throw InvalidMoveException("No room in that direction");
    }
    
    player->useMove();
    moveToRoom(nextRoom);
}

void GameManager::handleRoomInteraction() {
    if (!currentRoom) return;
    
    // Add enemies to combat queue
    for (auto& enemy : currentRoom->getEnemies()) {
        if (enemy->isAlive()) {
            combatQueue.push(enemy);
        }
    }
}

void GameManager::initiateCombat(std::shared_ptr<Enemy> enemy) {
    if (enemy && enemy->isAlive()) {
        combatQueue.push(enemy);
    }
}

void GameManager::handleCombat() {
    while (!combatQueue.empty() && player->isAlive()) {
        auto enemy = combatQueue.front();
        
        if (!enemy->isAlive()) {
            combatQueue.pop();
            continue;
        }
        
        std::cout << "\n\033[91m⚔️ Combat with " << enemy->getName() << "!\033[0m" << std::endl;
        enemy->render();
        
        bool combatContinues = processCombatTurn(enemy);
        
        if (!combatContinues) {
            combatQueue.pop();
            if (!enemy->isAlive()) {
                onEnemyDefeated(enemy);
            }
        }
    }
}

bool GameManager::processCombatTurn(std::shared_ptr<Enemy> enemy) {
    if (!enemy->isAlive() || !player->isAlive()) {
        return false;
    }
    
    std::cout << "\n\033[32mCombat Options:\033[0m" << std::endl;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Use Item" << std::endl;
    std::cout << "3. Try to Flee" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            int damage = player->attack();
            if (damage > 0) {
                enemy->takeDamage(damage);
                std::cout << "\033[32mYou deal " << damage << " damage!\033[0m" << std::endl;
            }
            break;
        }
        case 2:
            handleInventoryAction();
            break;
        case 3:
            std::cout << "\033[33mYou attempt to flee...\033[0m" << std::endl;
            if (rand() % 2 == 0) {
                std::cout << "\033[32mYou successfully escaped!\033[0m" << std::endl;
                return false;
            } else {
                std::cout << "\033[31mYou couldn't escape!\033[0m" << std::endl;
            }
            break;
        default:
            std::cout << "\033[31mInvalid choice!\033[0m" << std::endl;
            return true;
    }
    
    // Enemy attacks back if still alive
    if (enemy->isAlive()) {
        int enemyDamage = enemy->attack();
        if (enemyDamage > 0) {
            player->takeDamage(enemyDamage);
        }
    }
    
    return enemy->isAlive() && player->isAlive();
}

void GameManager::onEnemyDefeated(std::shared_ptr<Enemy> enemy) {
    std::cout << "\033[32m🏆 " << enemy->getName() << " defeated!\033[0m" << std::endl;
    
    // Award experience and gold
    int expReward = enemy->getMaxHealth() / 2;
    int goldReward = enemy->getAttackPower() * 2;
    
    player->gainExperience(expReward);
    player->addGold(goldReward);
    player->updateStatistic("enemies_defeated", 1);
    
    std::cout << "\033[93m💰 +" << goldReward << " gold, +" << expReward << " experience\033[0m" << std::endl;
    
    triggerEvent("enemy_defeated");
}

void GameManager::handlePlayerAction() {
    showGameMenu();
    
    int choice;
    std::cin >> choice;
    
    processInput(choice);
}

void GameManager::showGameMenu() {
    std::cout << "\n\033[32m🎮 What would you like to do?\033[0m" << std::endl;
    std::cout << "1. Move" << std::endl;
    std::cout << "2. Check Inventory" << std::endl;
    std::cout << "3. Use Item" << std::endl;
    std::cout << "4. Pick up Item" << std::endl;
    std::cout << "5. View Statistics" << std::endl;
    std::cout << "6. View Leaderboard" << std::endl;
    std::cout << "7. Help" << std::endl;
    std::cout << "8. Quit Game" << std::endl;
    std::cout << "Choice: ";
}

void GameManager::handleInventoryAction() {
    player->showInventory();
    
    if (player->getInventorySize() == 0) {
        return;
    }
    
    std::cout << "Enter item number to use (0 to cancel): ";
    int itemChoice;
    std::cin >> itemChoice;
    
    if (itemChoice > 0 && itemChoice <= static_cast<int>(player->getInventorySize())) {
        player->useItem(itemChoice - 1);
    }
}

void GameManager::handleMovementAction() {
    if (!currentRoom) return;
    
    auto directions = currentRoom->getAvailableDirections();
    if (directions.empty()) {
        std::cout << "\033[31mNo exits available!\033[0m" << std::endl;
        return;
    }
    
    std::cout << "\033[32mAvailable directions:\033[0m" << std::endl;
    for (size_t i = 0; i < directions.size(); ++i) {
        std::cout << (i + 1) << ". " << directions[i] << std::endl;
    }
    std::cout << "0. Cancel" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(directions.size())) {
        try {
            moveInDirection(directions[choice - 1]);
        } catch (const std::exception& e) {
            std::cout << "\033[31m" << e.what() << "\033[0m" << std::endl;
        }
    }
}

void GameManager::displayGameState() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    player->render();
    std::cout << std::string(60, '=') << std::endl;
    
    if (currentRoom) {
        currentRoom->render();
    }
}

void GameManager::displayRoomInfo() const {
    if (currentRoom) {
        std::cout << currentRoom->getDisplayInfo() << std::endl;
    }
}

void GameManager::calculateFinalScore() {
    totalScore = 0;
    
    // Base score from player stats
    totalScore += player->getLevel() * 100;
    totalScore += player->getGold();
    totalScore += player->getStatistic("enemies_defeated") * 50;
    totalScore += player->getStatistic("rooms_explored") * 25;
    
    // Bonus for completing the game
    if (gameWon) {
        totalScore += 1000;
    }
    
    // Bonus for efficiency (moves remaining)
    totalScore += player->getMovesLeft() * 10;
}

void GameManager::saveHighScore() {
    if (leaderboard.isHighScore(totalScore)) {
        std::cout << "\033[93m🏆 NEW HIGH SCORE! 🏆\033[0m" << std::endl;
    }
    
    PlayerScore score(
        player->getName(),
        totalScore,
        player->getLevel(),
        player->getStatistic("enemies_defeated"),
        player->getStatistic("rooms_explored"),
        "Today"  // Simplified timestamp
    );
    
    leaderboard.addScore(score);
}

void GameManager::registerEventHandler(const std::string& event, std::function<void()> handler) {
    eventHandlers[event] = handler;
}

void GameManager::triggerEvent(const std::string& event) {
    auto it = eventHandlers.find(event);
    if (it != eventHandlers.end()) {
        it->second();
    }
}

void GameManager::initializeGame() {
    // Initialize lambda functions
    displayMessage = [](const std::string& msg) {
        std::cout << "\033[36m" << msg << "\033[0m" << std::endl;
    };
    
    confirmAction = [](const std::string& prompt) -> bool {
        std::cout << prompt << " (y/n): ";
        char response;
        std::cin >> response;
        return (response == 'y' || response == 'Y');
    };
}

void GameManager::createDungeon() {
    // Create entrance
    auto entrance = std::make_shared<Room>(1, "Grand entrance hall with flickering torches");
    addRoom(entrance);
    
    // Create warning room
    auto warningRoom = std::make_shared<Room>(2, "A quiet chamber with an ominous feeling");
    addRoom(warningRoom);
    entrance->connectRoom("north", warningRoom);
    
    // Create shop
    auto shop = std::make_shared<ShopRoom>(3, "A mysterious merchant's shop");
    shop->addShopItem(std::make_shared<Weapon>("Iron Sword", "A sturdy iron blade", 50, 15, 10), 100);
    shop->addShopItem(std::make_shared<Potion>("Health Potion", "Restores 30 health", 25, 30), 50);
    addRoom(shop);
    warningRoom->connectRoom("east", shop);
    
    // Create trap room
    auto trapRoom = std::make_shared<TrapRoom>(4, "A dark corridor with suspicious floor tiles", 
                                               "Spikes shoot from the floor", 15);
    trapRoom->addItem(std::make_shared<Treasure>("Silver Ring", "A valuable silver ring", 75));
    addRoom(trapRoom);
    warningRoom->connectRoom("north", trapRoom);
    
    // Create combat room
    auto combatRoom = std::make_shared<Room>(5, "A damp cave with strange sounds");
    combatRoom->addEnemy(createEnemy("Goblin"));
    combatRoom->addItem(std::make_shared<Weapon>("Rusty Dagger", "An old but sharp dagger", 30, 8, 5));
    addRoom(combatRoom);
    trapRoom->connectRoom("north", combatRoom);
    
    // Create treasure room
    auto treasureRoom = std::make_shared<TreasureRoom>(6, "A glittering treasure chamber", 200);
    treasureRoom->addItem(std::make_shared<Treasure>("Golden Crown", "A magnificent golden crown", 500));
    addRoom(treasureRoom);
    combatRoom->connectRoom("east", treasureRoom);
    
    // Create boss room
    auto bossRoom = std::make_shared<BossRoom>(7, "The dragon's lair - final chamber", createEnemy("Dragon"));
    addRoom(bossRoom);
    combatRoom->connectRoom("north", bossRoom);
    
    // Create exit
    auto exitRoom = std::make_shared<Room>(8, "The dungeon exit - freedom awaits!");
    addRoom(exitRoom);
    bossRoom->connectRoom("north", exitRoom);
}

void GameManager::setupEventHandlers() {
    registerEventHandler("game_start", [this]() {
        displayMessage("🎮 Adventure begins!");
    });
    
    registerEventHandler("room_entered", [this]() {
        displayMessage("📍 Entered new area");
    });
    
    registerEventHandler("enemy_defeated", [this]() {
        displayMessage("⚔️ Victory in combat!");
    });
    
    registerEventHandler("game_won", [this]() {
        displayMessage("🎉 Quest completed successfully!");
    });
    
    registerEventHandler("game_lost", [this]() {
        displayMessage("💀 Quest failed...");
    });
}

void GameManager::processInput(int choice) {
    switch (choice) {
        case 1:
            handleMovementAction();
            break;
        case 2:
            player->showInventory();
            break;
        case 3:
            handleInventoryAction();
            break;
        case 4:
            // Pick up items from room
            if (currentRoom && currentRoom->hasItems()) {
                const auto& items = currentRoom->getItems();
                std::cout << "Items in room:" << std::endl;
                for (size_t i = 0; i < items.size(); ++i) {
                    std::cout << (i + 1) << ". ";
                    items[i]->render();
                }
                std::cout << "Pick up which item? (0 to cancel): ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice > 0 && itemChoice <= static_cast<int>(items.size())) {
                    player->addItem(items[itemChoice - 1]);
                    currentRoom->removeItem(itemChoice - 1);
                }
            } else {
                std::cout << "\033[31mNo items in this room.\033[0m" << std::endl;
            }
            break;
        case 5:
            player->displayStatistics();
            break;
        case 6:
            leaderboard.display();
            break;
        case 7:
            showHelp();
            break;
        case 8:
            if (confirmAction("Are you sure you want to quit?")) {
                gameRunning = false;
            }
            break;
        default:
            std::cout << "\033[31mInvalid choice!\033[0m" << std::endl;
    }
}

bool GameManager::checkWinCondition() const {
    // Win if player reaches the exit room (room 8)
    return currentRoom && currentRoom->getRoomID() == 8;
}

bool GameManager::checkLoseCondition() const {
    return !player->isAlive() || !player->hasMoves();
}

void GameManager::showHelp() const {
    std::cout << "\n\033[93m📖 === HELP === 📖\033[0m" << std::endl;
    std::cout << "🎯 Goal: Navigate through the dungeon and reach the exit" << std::endl;
    std::cout << "⚔️ Combat: Fight enemies to gain experience and gold" << std::endl;
    std::cout << "💰 Shop: Buy items with gold to help your journey" << std::endl;
    std::cout << "🏃 Movement: Each move costs 1 action point" << std::endl;
    std::cout << "❤️ Health: Use potions to restore health" << std::endl;
    std::cout << "⚠️ Traps: React quickly to avoid damage" << std::endl;
    std::cout << "🏆 Score: Based on level, gold, enemies defeated, and efficiency" << std::endl;
    std::cout << std::endl;
}

