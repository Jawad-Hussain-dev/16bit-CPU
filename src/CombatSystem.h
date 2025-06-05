#pragma once
#include "Templates.h"
#include <memory>
#include <vector>
#include <functional>

// Forward declarations
class Player;
class Enemy;
class GameEntity;

/**
 * Combat action structure for priority queue
 */
struct CombatAction {
    std::shared_ptr<GameEntity> actor;
    std::string actionType;
    int priority;
    float timestamp;
    std::function<void()> execute;
    
    CombatAction(std::shared_ptr<GameEntity> entity, const std::string& type, 
                int prio, float time, std::function<void()> action)
        : actor(entity), actionType(type), priority(prio), timestamp(time), execute(action) {}
    
    // Comparison for priority queue (higher priority first, then earlier timestamp)
    bool operator<(const CombatAction& other) const {
        if (priority != other.priority) {
            return priority < other.priority; // Higher priority first
        }
        return timestamp > other.timestamp; // Earlier timestamp first
    }
};

/**
 * Turn-based combat system using priority queues
 */
class CombatSystem {
private:
    PriorityContainer<CombatAction> actionQueue;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Enemy>> enemies;
    bool combatActive;
    int currentTurn;
    float gameTime;
    
    // Combat statistics
    int totalDamageDealt;
    int totalDamageTaken;
    int enemiesDefeated;
    int turnsElapsed;

public:
    CombatSystem();
    ~CombatSystem() = default;

    // Combat management
    void startCombat(std::vector<std::shared_ptr<Player>>& playerList, 
                    std::vector<std::shared_ptr<Enemy>>& enemyList);
    void endCombat();
    bool isCombatActive() const { return combatActive; }
    
    // Action scheduling
    void scheduleAction(std::shared_ptr<GameEntity> actor, const std::string& actionType,
                       int priority, std::function<void()> action);
    void processActions(float deltaTime);
    
    // Turn management
    void processTurn();
    void nextTurn();
    int getCurrentTurn() const { return currentTurn; }
    
    // Combat actions
    void playerAttack(std::shared_ptr<Player> player, std::shared_ptr<Enemy> target);
    void enemyAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<Player> target);
    void useItem(std::shared_ptr<Player> player, const std::string& itemName);
    void castSpell(std::shared_ptr<Player> player, const std::string& spellName, 
                  std::shared_ptr<GameEntity> target);
    
    // Utility functions
    void calculateDamage(std::shared_ptr<GameEntity> attacker, 
                        std::shared_ptr<GameEntity> defender, int baseDamage);
    void applyStatusEffect(std::shared_ptr<GameEntity> target, const std::string& effect, 
                          int duration);
    void checkCombatEnd();
    
    // AI decision making
    void processEnemyAI(std::shared_ptr<Enemy> enemy);
    std::shared_ptr<Player> selectTarget(std::shared_ptr<Enemy> enemy);
    
    // Statistics
    void resetStatistics();
    void printCombatSummary() const;
    int getTotalDamageDealt() const { return totalDamageDealt; }
    int getTotalDamageTaken() const { return totalDamageTaken; }
    int getEnemiesDefeated() const { return enemiesDefeated; }
    int getTurnsElapsed() const { return turnsElapsed; }
    
    // Event callbacks using lambdas
    std::function<void(std::shared_ptr<GameEntity>)> onEntityDeath;
    std::function<void(std::shared_ptr<Player>)> onPlayerLevelUp;
    std::function<void()> onCombatStart;
    std::function<void()> onCombatEnd;
};

/**
 * Status effect system
 */
struct StatusEffect {
    std::string name;
    int duration;
    std::function<void(std::shared_ptr<GameEntity>)> applyEffect;
    std::function<void(std::shared_ptr<GameEntity>)> removeEffect;
    
    StatusEffect(const std::string& effectName, int dur,
                std::function<void(std::shared_ptr<GameEntity>)> apply,
                std::function<void(std::shared_ptr<GameEntity>)> remove)
        : name(effectName), duration(dur), applyEffect(apply), removeEffect(remove) {}
};

/**
 * Damage calculation utilities
 */
class DamageCalculator {
public:
    static int calculatePhysicalDamage(int attackPower, int defense, int level = 1);
    static int calculateMagicalDamage(int spellPower, int magicResist, int level = 1);
    static int calculateCriticalHit(int baseDamage, float critChance, float critMultiplier = 2.0f);
    static bool rollForHit(int accuracy, int evasion);
    static int applyRandomVariation(int baseDamage, float variation = 0.1f);
};

