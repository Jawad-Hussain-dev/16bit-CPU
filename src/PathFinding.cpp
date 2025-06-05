#include "PathFinding.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

// PathFinder implementation
PathFinder::PathFinder(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<bool>(width, false));
}

void PathFinder::setObstacle(int x, int y, bool isObstacle) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = isObstacle;
    }
}

void PathFinder::setObstacle(const Position& pos, bool isObstacle) {
    setObstacle(pos.getX(), pos.getY(), isObstacle);
}

bool PathFinder::isObstacle(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true; // Out of bounds is considered an obstacle
    }
    return grid[y][x];
}

bool PathFinder::isObstacle(const Position& pos) const {
    return isObstacle(pos.getX(), pos.getY());
}

bool PathFinder::isValid(const Position& pos) const {
    return !isObstacle(pos);
}

std::vector<Position> PathFinder::findPathBFS(const Position& start, const Position& goal) {
    if (isObstacle(start) || isObstacle(goal)) {
        return {}; // No path if start or goal is blocked
    }
    
    std::queue<Position> frontier;
    std::unordered_map<Position, Position, PositionHash> cameFrom;
    std::unordered_set<Position, PositionHash> visited;
    
    frontier.push(start);
    visited.insert(start);
    cameFrom[start] = start;
    
    while (!frontier.empty()) {
        Position current = frontier.front();
        frontier.pop();
        
        if (current == goal) {
            return reconstructPath(cameFrom, start, goal);
        }
        
        for (const Position& neighbor : getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end() && isValid(neighbor)) {
                frontier.push(neighbor);
                visited.insert(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }
    
    return {}; // No path found
}

std::vector<Position> PathFinder::findPathDFS(const Position& start, const Position& goal) {
    if (isObstacle(start) || isObstacle(goal)) {
        return {}; // No path if start or goal is blocked
    }
    
    std::stack<Position> frontier;
    std::unordered_map<Position, Position, PositionHash> cameFrom;
    std::unordered_set<Position, PositionHash> visited;
    
    frontier.push(start);
    cameFrom[start] = start;
    
    while (!frontier.empty()) {
        Position current = frontier.top();
        frontier.pop();
        
        if (visited.find(current) != visited.end()) {
            continue;
        }
        
        visited.insert(current);
        
        if (current == goal) {
            return reconstructPath(cameFrom, start, goal);
        }
        
        for (const Position& neighbor : getNeighbors(current)) {
            if (visited.find(neighbor) == visited.end() && isValid(neighbor)) {
                frontier.push(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }
    
    return {}; // No path found
}

std::vector<Position> PathFinder::findPathAStar(const Position& start, const Position& goal) {
    if (isObstacle(start) || isObstacle(goal)) {
        return {}; // No path if start or goal is blocked
    }
    
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;
    std::unordered_map<Position, Position, PositionHash> cameFrom;
    std::unordered_map<Position, int, PositionHash> gScore;
    std::unordered_set<Position, PositionHash> closedSet;
    
    gScore[start] = 0;
    openSet.emplace(start, 0, heuristic(start, goal));
    
    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();
        
        if (closedSet.find(current.position) != closedSet.end()) {
            continue;
        }
        
        closedSet.insert(current.position);
        
        if (current.position == goal) {
            return reconstructPath(cameFrom, start, goal);
        }
        
        for (const Position& neighbor : getNeighbors(current.position)) {
            if (closedSet.find(neighbor) != closedSet.end() || !isValid(neighbor)) {
                continue;
            }
            
            int tentativeGScore = current.gCost + 1;
            
            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current.position;
                gScore[neighbor] = tentativeGScore;
                int hScore = heuristic(neighbor, goal);
                openSet.emplace(neighbor, tentativeGScore, hScore);
            }
        }
    }
    
    return {}; // No path found
}

std::vector<Position> PathFinder::getNeighbors(const Position& pos) const {
    std::vector<Position> neighbors;
    
    // 4-directional movement (up, down, left, right)
    std::vector<Position> directions = {
        Position(0, -1), // Up
        Position(0, 1),  // Down
        Position(-1, 0), // Left
        Position(1, 0)   // Right
    };
    
    for (const Position& dir : directions) {
        Position neighbor = pos + dir;
        if (neighbor.getX() >= 0 && neighbor.getX() < width &&
            neighbor.getY() >= 0 && neighbor.getY() < height) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

std::vector<Position> PathFinder::reconstructPath(
    const std::unordered_map<Position, Position, PositionHash>& cameFrom,
    const Position& start, const Position& goal) const {
    
    std::vector<Position> path;
    Position current = goal;
    
    while (!(current == start)) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(start);
    
    std::reverse(path.begin(), path.end());
    return path;
}

int PathFinder::heuristic(const Position& a, const Position& b) const {
    return a.manhattanDistance(b);
}

void PathFinder::clearGrid() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), false);
    }
}

void PathFinder::printGrid() const {
    for (const auto& row : grid) {
        for (bool cell : row) {
            std::cout << (cell ? '#' : '.') << ' ';
        }
        std::cout << '\n';
    }
}

// MazeGenerator implementation
MazeGenerator::MazeGenerator(int w, int h) : width(w), height(h) {
    // Ensure odd dimensions for proper maze generation
    if (width % 2 == 0) width++;
    if (height % 2 == 0) height++;
}

std::vector<std::vector<bool>> MazeGenerator::generateMazeBacktracker() {
    maze.assign(height, std::vector<bool>(width, true)); // Start with all walls
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    
    // Start from (1, 1)
    recursiveBacktracker(1, 1);
    
    return maze;
}

void MazeGenerator::recursiveBacktracker(int x, int y) {
    static std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    
    visited[y][x] = true;
    maze[y][x] = false; // Make it a path
    
    // Get all unvisited neighbors
    std::vector<Position> neighbors = getUnvisitedNeighbors(x, y, visited);
    
    // Shuffle neighbors for randomness
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(neighbors.begin(), neighbors.end(), g);
    
    for (const Position& neighbor : neighbors) {
        int nx = neighbor.getX();
        int ny = neighbor.getY();
        
        if (!visited[ny][nx]) {
            // Remove wall between current cell and neighbor
            int wallX = x + (nx - x) / 2;
            int wallY = y + (ny - y) / 2;
            maze[wallY][wallX] = false;
            
            recursiveBacktracker(nx, ny);
        }
    }
}

std::vector<Position> MazeGenerator::getUnvisitedNeighbors(int x, int y, 
    const std::vector<std::vector<bool>>& visited) const {
    
    std::vector<Position> neighbors;
    
    // Check 4 directions, 2 cells away
    std::vector<Position> directions = {
        Position(0, -2), // Up
        Position(0, 2),  // Down
        Position(-2, 0), // Left
        Position(2, 0)   // Right
    };
    
    for (const Position& dir : directions) {
        int nx = x + dir.getX();
        int ny = y + dir.getY();
        
        if (isValidMazePosition(nx, ny) && !visited[ny][nx]) {
            neighbors.emplace_back(nx, ny);
        }
    }
    
    return neighbors;
}

std::vector<std::vector<bool>> MazeGenerator::generateMazeDivision() {
    maze.assign(height, std::vector<bool>(width, false)); // Start with all paths
    
    // Add border walls
    for (int i = 0; i < width; i++) {
        maze[0][i] = true;
        maze[height - 1][i] = true;
    }
    for (int i = 0; i < height; i++) {
        maze[i][0] = true;
        maze[i][width - 1] = true;
    }
    
    recursiveDivision(1, 1, width - 2, height - 2);
    
    return maze;
}

void MazeGenerator::recursiveDivision(int x, int y, int w, int h) {
    if (w < 2 || h < 2) return;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    bool horizontal = (h > w) || (h == w && gen() % 2 == 0);
    
    if (horizontal) {
        // Draw horizontal wall
        int wallY = y + 1 + (gen() % (h - 1));
        if (wallY % 2 == 0) wallY++; // Ensure odd position
        
        for (int i = x; i < x + w; i++) {
            maze[wallY][i] = true;
        }
        
        // Create passage
        int passageX = x + (gen() % w);
        if (passageX % 2 == 1) passageX++; // Ensure even position
        maze[wallY][passageX] = false;
        
        // Recursively divide
        recursiveDivision(x, y, w, wallY - y);
        recursiveDivision(x, wallY + 1, w, h - (wallY - y + 1));
    } else {
        // Draw vertical wall
        int wallX = x + 1 + (gen() % (w - 1));
        if (wallX % 2 == 0) wallX++; // Ensure odd position
        
        for (int i = y; i < y + h; i++) {
            maze[i][wallX] = true;
        }
        
        // Create passage
        int passageY = y + (gen() % h);
        if (passageY % 2 == 1) passageY++; // Ensure even position
        maze[passageY][wallX] = false;
        
        // Recursively divide
        recursiveDivision(x, y, wallX - x, h);
        recursiveDivision(wallX + 1, y, w - (wallX - x + 1), h);
    }
}

std::vector<std::vector<bool>> MazeGenerator::generateMazePrim() {
    maze.assign(height, std::vector<bool>(width, true)); // Start with all walls
    
    // Start with a random cell
    std::random_device rd;
    std::mt19937 gen(rd());
    int startX = 1 + (gen() % (width - 2));
    int startY = 1 + (gen() % (height - 2));
    if (startX % 2 == 0) startX++;
    if (startY % 2 == 0) startY++;
    
    maze[startY][startX] = false;
    
    std::vector<Position> walls;
    
    // Add walls of the starting cell
    for (const Position& dir : {Position(0, -2), Position(0, 2), Position(-2, 0), Position(2, 0)}) {
        int nx = startX + dir.getX();
        int ny = startY + dir.getY();
        if (isValidMazePosition(nx, ny)) {
            walls.emplace_back(startX + dir.getX() / 2, startY + dir.getY() / 2);
        }
    }
    
    while (!walls.empty()) {
        // Pick a random wall
        int wallIndex = gen() % walls.size();
        Position wall = walls[wallIndex];
        walls.erase(walls.begin() + wallIndex);
        
        // Check if wall can be removed
        std::vector<Position> neighbors;
        for (const Position& dir : {Position(0, -1), Position(0, 1), Position(-1, 0), Position(1, 0)}) {
            int nx = wall.getX() + dir.getX();
            int ny = wall.getY() + dir.getY();
            if (isValidMazePosition(nx, ny) && !maze[ny][nx]) {
                neighbors.emplace_back(nx, ny);
            }
        }
        
        if (neighbors.size() == 1) {
            // Remove the wall
            maze[wall.getY()][wall.getX()] = false;
            
            // Add the cell on the other side
            Position newCell = neighbors[0];
            for (const Position& dir : {Position(0, -1), Position(0, 1), Position(-1, 0), Position(1, 0)}) {
                int nx = newCell.getX() + dir.getX();
                int ny = newCell.getY() + dir.getY();
                if (isValidMazePosition(nx, ny) && maze[ny][nx]) {
                    Position otherSide(newCell.getX() - dir.getX(), newCell.getY() - dir.getY());
                    if (isValidMazePosition(otherSide.getX(), otherSide.getY()) && maze[otherSide.getY()][otherSide.getX()]) {
                        walls.emplace_back(nx, ny);
                    }
                }
            }
        }
    }
    
    return maze;
}

void MazeGenerator::printMaze(const std::vector<std::vector<bool>>& maze) const {
    for (const auto& row : maze) {
        for (bool cell : row) {
            std::cout << (cell ? "██" : "  ");
        }
        std::cout << '\n';
    }
}

bool MazeGenerator::isValidMazePosition(int x, int y) const {
    return x > 0 && x < width - 1 && y > 0 && y < height - 1;
}

