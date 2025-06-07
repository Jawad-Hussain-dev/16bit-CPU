#include "Leaderboard.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

Leaderboard::Leaderboard(const std::string& file) : filename(file) {
    loadFromFile();
}

void Leaderboard::addScore(const PlayerScore& score) {
    scores.push_back(score);
    sortByScore();  // Keep sorted by score (highest first)
    maintainMaxEntries();
    saveToFile();
}

std::vector<PlayerScore> Leaderboard::getTopScores(size_t count) const {
    std::vector<PlayerScore> topScores = scores;
    
    // Sort by score (highest first)
    SortingAlgorithms<PlayerScore>::mergeSort(topScores, 
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score;  // Descending order
        });
    
    if (topScores.size() > count) {
        topScores.resize(count);
    }
    
    return topScores;
}

void Leaderboard::display() const {
    std::cout << "\n\033[93m🏆 === LEADERBOARD === 🏆\033[0m" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    if (scores.empty()) {
        std::cout << "No scores recorded yet!" << std::endl;
        return;
    }
    
    auto topScores = getTopScores();
    
    std::cout << std::left << std::setw(4) << "Rank" 
              << std::setw(15) << "Player" 
              << std::setw(8) << "Score"
              << std::setw(6) << "Level"
              << std::setw(10) << "Enemies"
              << std::setw(8) << "Rooms"
              << "Date" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < topScores.size(); ++i) {
        const auto& score = topScores[i];
        std::cout << std::left << std::setw(4) << (i + 1)
                  << std::setw(15) << score.playerName
                  << std::setw(8) << score.score
                  << std::setw(6) << score.level
                  << std::setw(10) << score.enemiesDefeated
                  << std::setw(8) << score.roomsExplored
                  << score.timestamp << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

void Leaderboard::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save leaderboard to " << filename << std::endl;
        return;
    }
    
    for (const auto& score : scores) {
        file << score.playerName << ","
             << score.score << ","
             << score.level << ","
             << score.enemiesDefeated << ","
             << score.roomsExplored << ","
             << score.timestamp << std::endl;
    }
    
    file.close();
}

void Leaderboard::loadFromFile() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // File doesn't exist yet, that's okay
        return;
    }
    
    scores.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, timestamp;
        int score, level, enemies, rooms;
        
        if (std::getline(ss, name, ',') &&
            ss >> score && ss.ignore() &&
            ss >> level && ss.ignore() &&
            ss >> enemies && ss.ignore() &&
            ss >> rooms && ss.ignore() &&
            std::getline(ss, timestamp)) {
            
            scores.emplace_back(name, score, level, enemies, rooms, timestamp);
        }
    }
    
    file.close();
    sortByScore();  // Ensure scores are sorted
}

void Leaderboard::clear() {
    scores.clear();
    saveToFile();
}

PlayerScore Leaderboard::getBestScore(const std::string& playerName) const {
    auto it = std::max_element(scores.begin(), scores.end(),
        [&playerName](const PlayerScore& a, const PlayerScore& b) {
            if (a.playerName == playerName && b.playerName == playerName) {
                return a.score < b.score;
            }
            if (a.playerName == playerName) return false;
            if (b.playerName == playerName) return true;
            return false;
        });
    
    if (it != scores.end() && it->playerName == playerName) {
        return *it;
    }
    
    // Return default score if player not found
    return PlayerScore(playerName, 0, 1, 0, 0, "Never played");
}

bool Leaderboard::isHighScore(int score) const {
    if (scores.size() < MAX_ENTRIES) {
        return true;  // Always a high score if leaderboard isn't full
    }
    
    auto topScores = getTopScores();
    return score > topScores.back().score;
}

void Leaderboard::sortByScore() {
    SortingAlgorithms<PlayerScore>::mergeSort(scores,
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score;  // Descending order
        });
}

void Leaderboard::sortByLevel() {
    SortingAlgorithms<PlayerScore>::quickSort(scores,
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.level > b.level;  // Descending order
        });
}

void Leaderboard::sortByEnemiesDefeated() {
    SortingAlgorithms<PlayerScore>::mergeSort(scores,
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.enemiesDefeated > b.enemiesDefeated;  // Descending order
        });
}

void Leaderboard::maintainMaxEntries() {
    if (scores.size() > MAX_ENTRIES) {
        scores.resize(MAX_ENTRIES);
    }
}

std::string Leaderboard::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

