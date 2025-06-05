#include "CombatSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "GameExceptions.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

CombatSystem::CombatSystem() 
    : combatActive(false), currentTurn(0), gameTime(0.0f),
      totalDamageDealt(0), totalDamageTaken(0), enemiesDefeated(0), turnsElapsed(0) {
    
    // Set up default event callbacks
    onEntityDeath = [this](std::shared_ptr<GameEntity> entity) {
        std::cout << entity->getName() << " has been defeated!\n";
        if (std::dynamic_pointer_cast<Enemy>(entity)) {
            enemiesDefeated++;
        }
    };
    
    onPlayerLevelUp = [](std::shared_ptr<Player> player) {
        std::cout << player->getName() << " has leveled up!\n";
    };
    
    onCombatStart = [this]() {
        std::cout << "Combat begins!\n";
        resetStatistics();
    };
    
    onCombatEnd = [this]() {
        std::cout << "Combat has ended!\n";
        printCombatSummary();
    };
}

void CombatSystem::startCombat(std::vector<std::shared_ptr<Player>>& playerList, 
                              std::vector<std::shared_ptr<Enemy>>& enemyList) {
    if (combatActive) {
        throw CombatException("Combat is already active");
    }
    
    players = playerList;
    enemies = enemyList;
    combatActive = true;
    currentTurn = 0;
    gameTime = 0.0f;
    
    if (onCombatStart) {
        onCombatStart();
    }
    
    // Schedule initial actions for all entities
    for (auto& player : players) {
        if (player->isAlive()) {
            scheduleAction(player, "turn", 100, [this, player]() {
                // Player turn logic would be handled by input system
            });
        }
    }
    
    for (auto& enemy : enemies) {
        if (enemy->isAlive()) {
            scheduleAction(enemy, "ai_turn", 50, [this, enemy]() {
                processEnemyAI(enemy);
            });
        }
    }
}

void CombatSystem::endCombat() {
    if (!combatActive) return;
    
    combatActive = false;
    actionQueue.clear();
    
    if (onCombatEnd) {
        onCombatEnd();
    }
}

void CombatSystem::scheduleAction(std::shared_ptr<GameEntity> actor, const std::string& actionType,
                                 int priority, std::function<void()> action) {
    if (!combatActive) return;
    
    actionQueue.push(CombatAction(actor, actionType, priority, gameTime, action));
}

void CombatSystem::processActions(float deltaTime) {
    if (!combatActive) return;
    
    gameTime += deltaTime;
    
    // Process all actions that are ready
    while (!actionQueue.empty()) {
        const CombatAction& action = actionQueue.top();
        
        // Check if the actor is still alive
        if (!action.actor->isAlive()) {
            actionQueue.pop();
            continue;
        }
        
        // Execute the action
        try {
            action.execute();
            actionQueue.pop();
        } catch (const std::exception& e) {
            std::cerr << "Error executing combat action: " << e.what() << std::endl;
            actionQueue.pop();
        }
        
        // Check if combat should end
        checkCombatEnd();
        if (!combatActive) break;
    }
}

void CombatSystem::processTurn() {
    if (!combatActive) return;
    
    turnsElapsed++;
    
    // Update all entities
    for (auto& player : players) {
        if (player->isAlive()) {
            player->update(1.0f); // 1 second per turn
        }
    }
    
    for (auto& enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->update(1.0f);
        }
    }
    
    nextTurn();
}

void CombatSystem::nextTurn() {
    currentTurn++;
    
    // Schedule next round of actions
    for (auto& enemy : enemies) {
        if (enemy->isAlive()) {
            scheduleAction(enemy, "ai_turn", 50, [this, enemy]() {
                processEnemyAI(enemy);
            });
        }
    }
}

void CombatSystem::playerAttack(std::shared_ptr<Player> player, std::shared_ptr<Enemy> target) {
    if (!player->isAlive() || !target->isAlive()) {
        throw CombatException("Cannot attack with dead entities");
    }
    
    try {
        int damage = player->attack();
        calculateDamage(player, target, damage);
        
        std::cout << player->getName() << " attacks " << target->getName() 
                  << " for " << damage << " damage!\n";
        
        totalDamageDealt += damage;
        
        // Check if target died
        if (!target->isAlive() && onEntityDeath) {
            onEntityDeath(target);
            
            // Award experience
            player->gainExperience(target->getExperienceReward());
        }
        
    } catch (const InvalidMoveException& e) {
        std::cout << "Cannot attack: " << e.what() << std::endl;
    }
}

void CombatSystem::enemyAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<Player> target) {
    if (!enemy->isAlive() || !target->isAlive()) {
        return;
    }
    
    if (enemy->canAttack()) {
        int damage = enemy->attack();
        calculateDamage(enemy, target, damage);
        
        std::cout << enemy->getName() << " attacks " << target->getName() 
                  << " for " << damage << " damage!\n";
        
        totalDamageTaken += damage;
        
        // Check if player died
        if (!target->isAlive() && onEntityDeath) {
            onEntityDeath(target);
        }
    }
}

void CombatSystem::useItem(std::shared_ptr<Player> player, const std::string& itemName) {
    // Implementation would depend on item system
    std::cout << player->getName() << " uses " << itemName << "!\n";
}

void CombatSystem::castSpell(std::shared_ptr<Player> player, const std::string& spellName, 
                            std::shared_ptr<GameEntity> target) {
    // Implementation would depend on spell system
    std::cout << player->getName() << " casts " << spellName << " on " << target->getName() << "!\n";
}

void CombatSystem::calculateDamage(std::shared_ptr<GameEntity> attacker, 
                                  std::shared_ptr<GameEntity> defender, int baseDamage) {
    // Apply damage calculations
    int finalDamage = DamageCalculator::applyRandomVariation(baseDamage);
    
    // Apply damage to defender
    defender->takeDamage(finalDamage);
}

void CombatSystem::applyStatusEffect(std::shared_ptr<GameEntity> target, const std::string& effect, 
                                    int duration) {
    std::cout << target->getName() << " is affected by " << effect << " for " << duration << " turns!\n";
    // Status effect implementation would go here
}

void CombatSystem::checkCombatEnd() {
    // Check if all players are dead
    bool allPlayersDead = std::all_of(players.begin(), players.end(),
        [](const std::shared_ptr<Player>& player) { return !player->isAlive(); });
    
    // Check if all enemies are dead
    bool allEnemiesDead = std::all_of(enemies.begin(), enemies.end(),
        [](const std::shared_ptr<Enemy>& enemy) { return !enemy->isAlive(); });
    
    if (allPlayersDead || allEnemiesDead) {
        endCombat();
    }
}

void CombatSystem::processEnemyAI(std::shared_ptr<Enemy> enemy) {
    if (!enemy->isAlive()) return;
    
    // Find a target
    std::shared_ptr<Player> target = selectTarget(enemy);
    if (target && target->isAlive()) {
        // Perform AI logic
        enemy->performAI(*target);
        
        // Attack if possible
        if (enemy->canAttack()) {
            enemyAttack(enemy, target);
        }
    }
}

std::shared_ptr<Player> CombatSystem::selectTarget(std::shared_ptr<Enemy> enemy) {
    // Simple AI: target the player with lowest health
    std::shared_ptr<Player> target = nullptr;
    int lowestHealth = INT_MAX;
    
    for (auto& player : players) {
        if (player->isAlive() && player->getHealth() < lowestHealth) {
            lowestHealth = player->getHealth();
            target = player;
        }
    }
    
    return target;
}

void CombatSystem::resetStatistics() {
    totalDamageDealt = 0;
    totalDamageTaken = 0;
    enemiesDefeated = 0;
    turnsElapsed = 0;
}

void CombatSystem::printCombatSummary() const {
    std::cout << "\n=== Combat Summary ===\n";
    std::cout << "Turns elapsed: " << turnsElapsed << "\n";
    std::cout << "Total damage dealt: " << totalDamageDealt << "\n";
    std::cout << "Total damage taken: " << totalDamageTaken << "\n";
    std::cout << "Enemies defeated: " << enemiesDefeated << "\n";
    std::cout << "=====================\n\n";
}

// DamageCalculator implementation
int DamageCalculator::calculatePhysicalDamage(int attackPower, int defense, int level) {
    int baseDamage = attackPower - (defense / 2);
    baseDamage = std::max(1, baseDamage); // Minimum 1 damage
    
    // Level scaling
    float levelMultiplier = 1.0f + (level - 1) * 0.1f;
    return static_cast<int>(baseDamage * levelMultiplier);
}

int DamageCalculator::calculateMagicalDamage(int spellPower, int magicResist, int level) {
    int baseDamage = spellPower - (magicResist / 3);
    baseDamage = std::max(1, baseDamage); // Minimum 1 damage
    
    // Level scaling
    float levelMultiplier = 1.0f + (level - 1) * 0.15f;
    return static_cast<int>(baseDamage * levelMultiplier);
}

int DamageCalculator::calculateCriticalHit(int baseDamage, float critChance, float critMultiplier) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    if (dis(gen) < critChance) {
        std::cout << "Critical hit!\n";
        return static_cast<int>(baseDamage * critMultiplier);
    }
    
    return baseDamage;
}

bool DamageCalculator::rollForHit(int accuracy, int evasion) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dis(1, 100);
    
    int hitChance = 85 + accuracy - evasion; // Base 85% hit chance
    hitChance = std::clamp(hitChance, 5, 95); // Between 5% and 95%
    
    return dis(gen) <= hitChance;
}

int DamageCalculator::applyRandomVariation(int baseDamage, float variation) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-variation, variation);
    
    float modifier = 1.0f + dis(gen);
    return std::max(1, static_cast<int>(baseDamage * modifier));
}

