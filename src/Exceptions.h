#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Custom exception for invalid game moves
class InvalidMoveException : public std::runtime_error {
public:
    explicit InvalidMoveException(const std::string& message) 
        : std::runtime_error("Invalid Move: " + message) {}
};

// Custom exception for inventory operations
class InventoryException : public std::runtime_error {
public:
    explicit InventoryException(const std::string& message) 
        : std::runtime_error("Inventory Error: " + message) {}
};

// Custom exception for combat operations
class CombatException : public std::runtime_error {
public:
    explicit CombatException(const std::string& message) 
        : std::runtime_error("Combat Error: " + message) {}
};

// Custom exception for game state errors
class GameStateException : public std::runtime_error {
public:
    explicit GameStateException(const std::string& message) 
        : std::runtime_error("Game State Error: " + message) {}
};

#endif // EXCEPTIONS_H

