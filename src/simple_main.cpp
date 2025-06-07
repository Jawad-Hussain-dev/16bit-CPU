#include <iostream>
#include <string>
#include <memory>
#include "GameManager.h"
#include "Exceptions.h"

// Simple console-only version for testing without SFML dependency
int main() {
    try {
        std::cout << "\033[93m" << std::endl;
        std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                    DUNGEON CRAWLER RPG                      ║" << std::endl;
        std::cout << "║                  Enhanced C++ Edition                       ║" << std::endl;
        std::cout << "║                   Console Mode Only                         ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << "\033[0m" << std::endl;
        
        // Get player name
        std::string playerName;
        std::cout << "\033[32mEnter your hero's name: \033[0m";
        std::getline(std::cin, playerName);
        
        if (playerName.empty()) {
            playerName = "Anonymous Hero";
        }
        
        // Start the game
        std::cout << "\033[32m🎮 Starting game...\033[0m" << std::endl;
        
        auto gameManager = std::make_unique<GameManager>(playerName);
        gameManager->startGame();
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mGame Error: " << e.what() << "\033[0m" << std::endl;
        return 1;
    }
    
    return 0;
}

