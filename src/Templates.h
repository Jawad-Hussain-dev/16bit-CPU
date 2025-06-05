#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>

/**
 * Template classes demonstrating generic programming
 */

// Generic inventory container template
template<typename T>
class Inventory {
private:
    std::vector<T> items;
    size_t maxCapacity;

public:
    explicit Inventory(size_t capacity = 10) : maxCapacity(capacity) {
        items.reserve(capacity);
    }

    // Add item to inventory
    bool addItem(const T& item) {
        if (items.size() >= maxCapacity) {
            return false;
        }
        items.push_back(item);
        return true;
    }

    // Remove item by index
    bool removeItem(size_t index) {
        if (index >= items.size()) {
            return false;
        }
        items.erase(items.begin() + index);
        return true;
    }

    // Remove item by value
    bool removeItem(const T& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }

    // Get item by index with bounds checking
    T& getItem(size_t index) {
        if (index >= items.size()) {
            throw std::out_of_range("Inventory index out of range");
        }
        return items[index];
    }

    const T& getItem(size_t index) const {
        if (index >= items.size()) {
            throw std::out_of_range("Inventory index out of range");
        }
        return items[index];
    }

    // Sort items using custom comparator
    template<typename Compare>
    void sortItems(Compare comp) {
        std::sort(items.begin(), items.end(), comp);
    }

    // Find items using predicate
    template<typename Predicate>
    std::vector<T> findItems(Predicate pred) const {
        std::vector<T> result;
        std::copy_if(items.begin(), items.end(), std::back_inserter(result), pred);
        return result;
    }

    // Apply function to all items
    template<typename Function>
    void forEach(Function func) {
        std::for_each(items.begin(), items.end(), func);
    }

    // Getters
    size_t size() const { return items.size(); }
    size_t capacity() const { return maxCapacity; }
    bool empty() const { return items.empty(); }
    bool full() const { return items.size() >= maxCapacity; }

    // Iterator support
    typename std::vector<T>::iterator begin() { return items.begin(); }
    typename std::vector<T>::iterator end() { return items.end(); }
    typename std::vector<T>::const_iterator begin() const { return items.begin(); }
    typename std::vector<T>::const_iterator end() const { return items.end(); }
};

// Generic priority queue wrapper template
template<typename T, typename Compare = std::less<T>>
class PriorityContainer {
private:
    std::vector<T> heap;
    Compare comp;

public:
    explicit PriorityContainer(const Compare& c = Compare()) : comp(c) {}

    void push(const T& item) {
        heap.push_back(item);
        std::push_heap(heap.begin(), heap.end(), comp);
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("Cannot pop from empty priority container");
        }
        std::pop_heap(heap.begin(), heap.end(), comp);
        T result = heap.back();
        heap.pop_back();
        return result;
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Cannot access top of empty priority container");
        }
        return heap.front();
    }

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
    void clear() { heap.clear(); }
};

// Template function for generic sorting
template<typename Iterator, typename Compare>
void mergeSort(Iterator first, Iterator last, Compare comp) {
    auto distance = std::distance(first, last);
    if (distance <= 1) return;

    auto middle = first;
    std::advance(middle, distance / 2);

    mergeSort(first, middle, comp);
    mergeSort(middle, last, comp);
    std::inplace_merge(first, middle, last, comp);
}

