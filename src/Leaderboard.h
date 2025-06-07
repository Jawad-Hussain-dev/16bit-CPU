#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <vector>
#include <string>
#include <fstream>
#include "Algorithms.h"

// Structure to represent a player score
struct PlayerScore {
    std::string playerName;
    int score;
    int level;
    int enemiesDefeated;
    int roomsExplored;
    std::string timestamp;

    // Default constructor
    PlayerScore() : playerName(""), score(0), level(1), enemiesDefeated(0), roomsExplored(0), timestamp("") {}

    PlayerScore(const std::string& name, int playerScore, int playerLevel, 
                int enemies, int rooms, const std::string& time)
        : playerName(name), score(playerScore), level(playerLevel),
          enemiesDefeated(enemies), roomsExplored(rooms), timestamp(time) {}

    // Operator overloading for comparison
    bool operator<(const PlayerScore& other) const {
        return score < other.score;
    }

    bool operator>(const PlayerScore& other) const {
        return score > other.score;
    }

    bool operator==(const PlayerScore& other) const {
        return playerName == other.playerName && score == other.score;
    }
};

class Leaderboard {
private:
    std::vector<PlayerScore> scores;
    std::string filename;
    static const size_t MAX_ENTRIES = 10;

public:
    explicit Leaderboard(const std::string& file = "leaderboard.txt");
    
    // Add a new score
    void addScore(const PlayerScore& score);
    
    // Get top scores (sorted)
    std::vector<PlayerScore> getTopScores(size_t count = MAX_ENTRIES) const;
    
    // Display leaderboard
    void display() const;
    
    // Save/Load from file
    void saveToFile() const;
    void loadFromFile();
    
    // Clear all scores
    void clear();
    
    // Get player's best score
    PlayerScore getBestScore(const std::string& playerName) const;
    
    // Check if player has a high score
    bool isHighScore(int score) const;
    
    // Get total number of scores
    size_t size() const { return scores.size(); }
    
    // Sort scores using different algorithms
    void sortByScore();
    void sortByLevel();
    void sortByEnemiesDefeated();

private:
    void maintainMaxEntries();
    std::string getCurrentTimestamp() const;
};

#endif // LEADERBOARD_H
