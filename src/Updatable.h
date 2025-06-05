#pragma once

/**
 * Abstract interface for updatable objects
 * Demonstrates interface segregation principle
 */
class Updatable {
public:
    virtual ~Updatable() = default;
    
    // Pure virtual function for updating game logic
    virtual void update(float deltaTime) = 0;
};

