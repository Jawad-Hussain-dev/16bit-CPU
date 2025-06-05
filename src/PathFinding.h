#pragma once
#include "Position.h"
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <functional>

/**
 * PathFinding algorithms demonstrating BFS, DFS, and A*
 */

// Hash function for Position to use in unordered containers
struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        return std::hash<int>()(pos.getX()) ^ (std::hash<int>()(pos.getY()) << 1);
    }
};

class PathFinder {
private:
    std::vector<std::vector<bool>> grid;
    int width, height;

public:
    PathFinder(int w, int h);
    
    // Set obstacles in the grid
    void setObstacle(int x, int y, bool isObstacle = true);
    void setObstacle(const Position& pos, bool isObstacle = true);
    bool isObstacle(int x, int y) const;
    bool isObstacle(const Position& pos) const;
    bool isValid(const Position& pos) const;
    
    // Pathfinding algorithms
    std::vector<Position> findPathBFS(const Position& start, const Position& goal);
    std::vector<Position> findPathDFS(const Position& start, const Position& goal);
    std::vector<Position> findPathAStar(const Position& start, const Position& goal);
    
    // Utility functions
    std::vector<Position> getNeighbors(const Position& pos) const;
    std::vector<Position> reconstructPath(
        const std::unordered_map<Position, Position, PositionHash>& cameFrom,
        const Position& start, const Position& goal) const;
    
    // Heuristic function for A*
    int heuristic(const Position& a, const Position& b) const;
    
    // Grid manipulation
    void clearGrid();
    void printGrid() const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

/**
 * Node structure for A* algorithm
 */
struct AStarNode {
    Position position;
    int gCost; // Distance from start
    int hCost; // Heuristic distance to goal
    int fCost; // Total cost (g + h)
    
    AStarNode(const Position& pos, int g, int h) 
        : position(pos), gCost(g), hCost(h), fCost(g + h) {}
    
    // Comparison for priority queue (min-heap)
    bool operator>(const AStarNode& other) const {
        return fCost > other.fCost;
    }
};

/**
 * Maze generation using recursive algorithms
 */
class MazeGenerator {
private:
    std::vector<std::vector<bool>> maze;
    int width, height;
    
    void recursiveBacktracker(int x, int y);
    void recursiveDivision(int x, int y, int w, int h);
    std::vector<Position> getUnvisitedNeighbors(int x, int y, 
        const std::vector<std::vector<bool>>& visited) const;

public:
    MazeGenerator(int w, int h);
    
    // Different maze generation algorithms
    std::vector<std::vector<bool>> generateMazeBacktracker();
    std::vector<std::vector<bool>> generateMazeDivision();
    std::vector<std::vector<bool>> generateMazePrim();
    
    // Utility functions
    void printMaze(const std::vector<std::vector<bool>>& maze) const;
    bool isValidMazePosition(int x, int y) const;
};

