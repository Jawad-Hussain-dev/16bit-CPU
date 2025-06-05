#include "Game.h"
#include "GameExceptions.h"
#include <iostream>
#include <exception>

/**
 * Main entry point for the Enhanced Dungeon Crawler
 * Demonstrates exception handling and proper resource management
 */
int main() {
    try {
        std::cout << "=== Enhanced Dungeon Crawler ===" << std::endl;
        std::cout << "Initializing game..." << std::endl;
        
        // Create and initialize the game
        Game game;
        
        if (!game.initialize()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return -1;
        }
        
        std::cout << "Game initialized successfully!" << std::endl;
        std::cout << "Starting game loop..." << std::endl;
        
        // Run the main game loop
        game.run();
        
        std::cout << "Game loop ended." << std::endl;
        
        // Clean shutdown
        game.shutdown();
        
        std::cout << "Game shut down successfully." << std::endl;
        return 0;
        
    } catch (const GameException& e) {
        std::cerr << "Game Error: " << e.what() << std::endl;
        return -1;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return -1;
        
    } catch (...) {
        std::cerr << "Unknown error occurred!" << std::endl;
        return -1;
    }
}

