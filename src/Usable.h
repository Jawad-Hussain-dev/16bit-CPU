#ifndef USABLE_H
#define USABLE_H

#include <string>

// Forward declaration
class Player;

// Abstract interface for usable items
class Usable {
public:
    virtual ~Usable() = default;
    virtual void use(Player& player) = 0;  // Pure virtual function
    virtual std::string getDescription() const = 0;  // Pure virtual function
    virtual bool isConsumable() const = 0;  // Pure virtual function
};

#endif // USABLE_H

