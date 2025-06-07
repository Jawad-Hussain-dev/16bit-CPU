#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <string>

// Abstract interface for renderable objects
class Renderable {
public:
    virtual ~Renderable() = default;
    virtual void render() const = 0;  // Pure virtual function
    virtual std::string getDisplayInfo() const = 0;  // Pure virtual function
};

#endif // RENDERABLE_H

