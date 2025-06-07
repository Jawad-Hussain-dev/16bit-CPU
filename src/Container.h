#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <algorithm>
#include <functional>
#include "Exceptions.h"

// Template class for generic container management
template<typename T>
class Container {
private:
    std::vector<T> items;
    size_t maxCapacity;

public:
    explicit Container(size_t capacity = 100) : maxCapacity(capacity) {}

    // Add item to container
    void add(const T& item) {
        if (items.size() >= maxCapacity) {
            throw InventoryException("Container is full");
        }
        items.push_back(item);
    }

    // Remove item by index
    void remove(size_t index) {
        if (index >= items.size()) {
            throw InventoryException("Invalid index");
        }
        items.erase(items.begin() + index);
    }

    // Get item by index
    T& get(size_t index) {
        if (index >= items.size()) {
            throw InventoryException("Invalid index");
        }
        return items[index];
    }

    const T& get(size_t index) const {
        if (index >= items.size()) {
            throw InventoryException("Invalid index");
        }
        return items[index];
    }

    // Get size
    size_t size() const { return items.size(); }
    
    // Check if empty
    bool empty() const { return items.empty(); }
    
    // Get capacity
    size_t capacity() const { return maxCapacity; }

    // Clear all items
    void clear() { items.clear(); }

    // Find item using predicate
    template<typename Predicate>
    auto find_if(Predicate pred) -> decltype(items.begin()) {
        return std::find_if(items.begin(), items.end(), pred);
    }

    // Sort items using custom comparator
    template<typename Compare>
    void sort(Compare comp) {
        std::sort(items.begin(), items.end(), comp);
    }

    // Iterator support
    auto begin() -> decltype(items.begin()) { return items.begin(); }
    auto end() -> decltype(items.end()) { return items.end(); }
    auto begin() const -> decltype(items.begin()) { return items.begin(); }
    auto end() const -> decltype(items.end()) { return items.end(); }
};

#endif // CONTAINER_H

