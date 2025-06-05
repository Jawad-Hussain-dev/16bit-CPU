#include "Position.h"
#include <cmath>

Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
}

Position Position::operator-(const Position& other) const {
    return Position(x - other.x, y - other.y);
}

Position& Position::operator+=(const Position& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Position& Position::operator-=(const Position& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

double Position::distanceTo(const Position& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

int Position::manhattanDistance(const Position& other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}

