#include "Algorithms.h"
#include <queue>
#include <stack>
#include <unordered_set>

// BFS implementation
std::vector<int> PathfindingAlgorithms::bfs(const std::vector<std::vector<int>>& graph, int start, int target) {
    if (start == target) return {start};
    
    std::queue<int> queue;
    std::unordered_set<int> visited;
    std::vector<int> parent(graph.size(), -1);
    
    queue.push(start);
    visited.insert(start);
    
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        
        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                queue.push(neighbor);
                
                if (neighbor == target) {
                    return reconstructPath(parent, start, target);
                }
            }
        }
    }
    
    return {};  // No path found
}

// DFS implementation
std::vector<int> PathfindingAlgorithms::dfs(const std::vector<std::vector<int>>& graph, int start, int target) {
    if (start == target) return {start};
    
    std::stack<int> stack;
    std::unordered_set<int> visited;
    std::vector<int> parent(graph.size(), -1);
    
    stack.push(start);
    
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        
        if (visited.find(current) != visited.end()) continue;
        visited.insert(current);
        
        if (current == target) {
            return reconstructPath(parent, start, target);
        }
        
        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                parent[neighbor] = current;
                stack.push(neighbor);
            }
        }
    }
    
    return {};  // No path found
}

// Check if path exists
bool PathfindingAlgorithms::hasPath(const std::vector<std::vector<int>>& graph, int start, int target) {
    return !bfs(graph, start, target).empty();
}

// Reconstruct path from parent array
std::vector<int> PathfindingAlgorithms::reconstructPath(const std::vector<int>& parent, int start, int target) {
    std::vector<int> path;
    int current = target;
    
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    
    std::reverse(path.begin(), path.end());
    
    // Verify path starts with start node
    if (!path.empty() && path[0] == start) {
        return path;
    }
    
    return {};  // Invalid path
}

