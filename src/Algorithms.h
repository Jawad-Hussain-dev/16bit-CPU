#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <functional>
#include <queue>
#include <stack>

// Template sorting algorithms
template<typename T>
class SortingAlgorithms {
public:
    // Merge Sort implementation
    static void mergeSort(std::vector<T>& arr, std::function<bool(const T&, const T&)> compare) {
        if (arr.size() <= 1) return;
        mergeSortHelper(arr, 0, arr.size() - 1, compare);
    }

    // Quick Sort implementation
    static void quickSort(std::vector<T>& arr, std::function<bool(const T&, const T&)> compare) {
        if (arr.size() <= 1) return;
        quickSortHelper(arr, 0, arr.size() - 1, compare);
    }

    // Binary Search (requires sorted array)
    static int binarySearch(const std::vector<T>& arr, const T& target, 
                           std::function<bool(const T&, const T&)> compare) {
        int left = 0, right = arr.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (!compare(arr[mid], target) && !compare(target, arr[mid])) {
                return mid;  // Found
            }
            
            if (compare(arr[mid], target)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return -1;  // Not found
    }

private:
    static void mergeSortHelper(std::vector<T>& arr, int left, int right, 
                               std::function<bool(const T&, const T&)> compare) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, compare);
        mergeSortHelper(arr, mid + 1, right, compare);
        merge(arr, left, mid, right, compare);
    }

    static void merge(std::vector<T>& arr, int left, int mid, int right,
                     std::function<bool(const T&, const T&)> compare) {
        std::vector<T> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (compare(arr[i], arr[j])) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (int i = 0; i < k; i++) {
            arr[left + i] = temp[i];
        }
    }

    static void quickSortHelper(std::vector<T>& arr, int low, int high,
                               std::function<bool(const T&, const T&)> compare) {
        if (low < high) {
            int pi = partition(arr, low, high, compare);
            quickSortHelper(arr, low, pi - 1, compare);
            quickSortHelper(arr, pi + 1, high, compare);
        }
    }

    static int partition(std::vector<T>& arr, int low, int high,
                        std::function<bool(const T&, const T&)> compare) {
        T pivot = arr[high];
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (compare(arr[j], pivot)) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }
};

// Pathfinding algorithms for dungeon navigation
class PathfindingAlgorithms {
public:
    // Breadth-First Search for shortest path
    static std::vector<int> bfs(const std::vector<std::vector<int>>& graph, int start, int target);
    
    // Depth-First Search for path finding
    static std::vector<int> dfs(const std::vector<std::vector<int>>& graph, int start, int target);
    
    // Check if path exists between two nodes
    static bool hasPath(const std::vector<std::vector<int>>& graph, int start, int target);

private:
    static std::vector<int> reconstructPath(const std::vector<int>& parent, int start, int target);
};

#endif // ALGORITHMS_H

