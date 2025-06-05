#pragma once
#include <iostream>

/**
 * Position class demonstrating operator overloading
 * Used for entity positioning and movement calculations
 */
class Position {
private:
    int x, y;

public:
    Position(int x = 0, int y = 0) : x(x), y(y) {}

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    // Setters
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void set(int newX, int newY) { x = newX; y = newY; }

    // Operator overloading for position arithmetic
    Position operator+(const Position& other) const;
    Position operator-(const Position& other) const;
    Position& operator+=(const Position& other);
    Position& operator-=(const Position& other);
    
    // Comparison operators
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
    
    // Distance calculation
    double distanceTo(const Position& other) const;
    int manhattanDistance(const Position& other) const;
    
    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

