#ifndef COMBATABLE_H
#define COMBATABLE_H

// Abstract interface for combatable entities
class Combatable {
public:
    virtual ~Combatable() = default;
    virtual int attack() = 0;  // Pure virtual function
    virtual void takeDamage(int damage) = 0;  // Pure virtual function
    virtual bool isAlive() const = 0;  // Pure virtual function
    virtual int getHealth() const = 0;  // Pure virtual function
};

#endif // COMBATABLE_H

