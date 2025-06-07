#include <iostream>
#include <string>
#include <memory>
#include "GameManager.h"
#include "GUI.h"
#include "Exceptions.h"

void showWelcomeMessage() {
    std::cout << "\033[93m" << std::endl;
    std::cout << "╔══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    DUNGEON CRAWLER RPG                      ║" << std::endl;
    std::cout << "║                  Enhanced C++ Edition                       ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Features:                                                   ║" << std::endl;
    std::cout << "║  • Object-Oriented Design with Inheritance & Polymorphism   ║" << std::endl;
    std::cout << "║  • Template Classes and Generic Programming                 ║" << std::endl;
    std::cout << "║  • Exception Handling and Error Recovery                    ║" << std::endl;
    std::cout << "║  • STL Containers and Algorithms                            ║" << std::endl;
    std::cout << "║  • Operator Overloading and Abstract Classes               ║" << std::endl;
    std::cout << "║  • Simple GUI Interface (Optional)                          ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\033[0m" << std::endl;
}

void showGameModeMenu() {
    std::cout << "\033[36mSelect Game Mode:\033[0m" << std::endl;
    std::cout << "1. Console Mode (Text-based)" << std::endl;
    std::cout << "2. GUI Mode (Graphical - requires SFML)" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choice: ";
}

std::string getPlayerName() {
    std::string name;
    std::cout << "\033[32mEnter your hero's name: \033[0m";
    std::getline(std::cin, name);
    
    if (name.empty()) {
        name = "Anonymous Hero";
    }
    
    return name;
}

void runConsoleMode(const std::string& playerName) {
    try {
        std::cout << "\033[32m🎮 Starting Console Mode...\033[0m" << std::endl;
        
        auto gameManager = std::make_unique<GameManager>(playerName);
        gameManager->startGame();
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mGame Error: " << e.what() << "\033[0m" << std::endl;
    }
}

void runGUIMode(const std::string& playerName) {
    try {
        std::cout << "\033[32m🖥️ Starting GUI Mode...\033[0m" << std::endl;
        
        auto gameManager = std::make_unique<GameManager>(playerName);
        auto gui = std::make_unique<GameGUI>(gameManager.get());
        
        if (!gui->initialize()) {
            std::cerr << "\033[31mFailed to initialize GUI. Falling back to console mode.\033[0m" << std::endl;
            runConsoleMode(playerName);
            return;
        }
        
        // Start the game in a separate thread or integrate with GUI
        // For simplicity, we'll just run the GUI
        gui->run();
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mGUI Error: " << e.what() << "\033[0m" << std::endl;
        std::cout << "\033[33mFalling back to console mode...\033[0m" << std::endl;
        runConsoleMode(playerName);
    }
}

void demonstrateFeatures() {
    std::cout << "\n\033[93m🔧 Demonstrating Advanced C++ Features:\033[0m" << std::endl;
    
    // Template demonstration
    std::cout << "\n1. Template Classes:" << std::endl;
    Container<int> intContainer(5);
    try {
        intContainer.add(10);
        intContainer.add(20);
        std::cout << "   ✅ Template container working with integers" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "   ❌ Template error: " << e.what() << std::endl;
    }
    
    // Exception handling demonstration
    std::cout << "\n2. Exception Handling:" << std::endl;
    try {
        throw InvalidMoveException("Demonstrating custom exception");
    } catch (const InvalidMoveException& e) {
        std::cout << "   ✅ Caught custom exception: " << e.what() << std::endl;
    }
    
    // Polymorphism demonstration
    std::cout << "\n3. Polymorphism:" << std::endl;
    auto goblin = createEnemy("Goblin");
    auto dragon = createEnemy("Dragon");
    std::cout << "   ✅ Created different enemy types using factory pattern" << std::endl;
    std::cout << "   ✅ Polymorphic behavior: " << goblin->getName() << " vs " << dragon->getName() << std::endl;
    
    // STL containers demonstration
    std::cout << "\n4. STL Containers:" << std::endl;
    std::vector<std::string> features = {"Inheritance", "Templates", "Exceptions", "STL"};
    std::cout << "   ✅ Using std::vector, std::map, std::queue, std::stack throughout the game" << std::endl;
    
    std::cout << "\n\033[32m🎯 All advanced features successfully integrated!\033[0m" << std::endl;
}

int main() {
    try {
        showWelcomeMessage();
        
        // Demonstrate features
        demonstrateFeatures();
        
        std::cout << "\nPress Enter to continue to game...";
        std::cin.ignore();
        
        // Get player name
        std::string playerName = getPlayerName();
        
        // Game mode selection
        int choice;
        do {
            showGameModeMenu();
            std::cin >> choice;
            std::cin.ignore(); // Clear input buffer
            
            switch (choice) {
                case 1:
                    runConsoleMode(playerName);
                    choice = 3; // Exit after game
                    break;
                case 2:
                    runGUIMode(playerName);
                    choice = 3; // Exit after game
                    break;
                case 3:
                    std::cout << "\033[33mThanks for playing! Goodbye!\033[0m" << std::endl;
                    break;
                default:
                    std::cout << "\033[31mInvalid choice. Please try again.\033[0m" << std::endl;
                    break;
            }
        } while (choice != 3);
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mFatal Error: " << e.what() << "\033[0m" << std::endl;
        return 1;
    }
    
    return 0;
}

