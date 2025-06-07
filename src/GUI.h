#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <string>

// Forward declarations
class GameManager;
class Player;
class Room;

// Simple button class for GUI interactions
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font* font;
    bool isPressed;
    std::function<void()> onClick;

public:
    Button(float x, float y, float width, float height, const std::string& label, sf::Font* buttonFont);
    
    void setOnClick(std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos);
    void update();
    void render(sf::RenderWindow& window);
    
    bool contains(const sf::Vector2i& point) const;
    void setPosition(float x, float y);
    void setText(const std::string& newText);
};

// Main GUI class
class GameGUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text playerInfoText;
    sf::Text roomInfoText;
    sf::Text inventoryText;
    sf::Text messageText;
    
    // GUI panels
    sf::RectangleShape playerPanel;
    sf::RectangleShape roomPanel;
    sf::RectangleShape inventoryPanel;
    sf::RectangleShape messagePanel;
    
    // Buttons
    std::vector<std::unique_ptr<Button>> buttons;
    
    // Game reference
    GameManager* gameManager;
    
    // GUI state
    bool showInventory;
    bool showHelp;
    std::string currentMessage;
    sf::Clock messageClock;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color panelColor;
    sf::Color textColor;
    sf::Color buttonColor;

public:
    GameGUI(GameManager* gm);
    ~GameGUI() = default;
    
    // Main GUI methods
    bool initialize();
    void run();
    void handleEvents();
    void update();
    void render();
    
    // Display methods
    void updatePlayerInfo();
    void updateRoomInfo();
    void updateInventoryInfo();
    void showMessage(const std::string& message, float duration = 3.0f);
    
    // Button creation
    void createButtons();
    void createMovementButtons();
    void createActionButtons();
    
    // Event handlers
    void onMoveNorth();
    void onMoveSouth();
    void onMoveEast();
    void onMoveWest();
    void onShowInventory();
    void onUseItem();
    void onPickupItem();
    void onShowStats();
    void onQuitGame();
    
    // Utility methods
    void centerText(sf::Text& text, const sf::RectangleShape& panel);
    sf::Color getHealthColor(int health, int maxHealth) const;
    void drawProgressBar(sf::RenderWindow& window, float x, float y, float width, float height, 
                        float progress, sf::Color fillColor, sf::Color backgroundColor);

private:
    bool loadFont();
    void setupPanels();
    void setupTexts();
    void clearButtons();
};

#endif // GUI_H

