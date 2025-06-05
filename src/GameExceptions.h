#pragma once
#include <stdexcept>
#include <string>

/**
 * Custom exception classes for game-specific error handling
 */

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error("Game Error: " + message) {}
};

class InvalidMoveException : public GameException {
public:
    explicit InvalidMoveException(const std::string& message)
        : GameException("Invalid Move: " + message) {}
};

class ResourceLoadException : public GameException {
public:
    explicit ResourceLoadException(const std::string& resource)
        : GameException("Failed to load resource: " + resource) {}
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& operation)
        : GameException("Save/Load error: " + operation) {}
};

class CombatException : public GameException {
public:
    explicit CombatException(const std::string& message)
        : GameException("Combat error: " + message) {}
};

class InventoryException : public GameException {
public:
    explicit InventoryException(const std::string& message)
        : GameException("Inventory error: " + message) {}
};

class NetworkException : public GameException {
public:
    explicit NetworkException(const std::string& message)
        : GameException("Network error: " + message) {}
};

