#include "GUI.h"
#include "GameManager.h"
#include "Player.h"
#include "Room.h"
#include <iostream>
#include <sstream>

// Button implementation
Button::Button(float x, float y, float width, float height, const std::string& label, sf::Font* buttonFont)
    : font(buttonFont), isPressed(false) {
    
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(70, 70, 70));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color(100, 100, 100));
    
    text.setFont(*font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    
    // Center text in button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        x + (width - textBounds.width) / 2,
        y + (height - textBounds.height) / 2
    );
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && contains(mousePos)) {
            isPressed = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left && isPressed && contains(mousePos)) {
            if (onClick) {
                onClick();
            }
        }
        isPressed = false;
    }
}

void Button::update() {
    // Visual feedback for button states
    if (isPressed) {
        shape.setFillColor(sf::Color(50, 50, 50));
    } else {
        shape.setFillColor(sf::Color(70, 70, 70));
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::contains(const sf::Vector2i& point) const {
    return shape.getGlobalBounds().contains(static_cast<float>(point.x), static_cast<float>(point.y));
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f size = shape.getSize();
    text.setPosition(
        x + (size.x - textBounds.width) / 2,
        y + (size.y - textBounds.height) / 2
    );
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f size = shape.getSize();
    text.setPosition(
        pos.x + (size.x - textBounds.width) / 2,
        pos.y + (size.y - textBounds.height) / 2
    );
}

// GameGUI implementation
GameGUI::GameGUI(GameManager* gm) 
    : window(sf::VideoMode(1024, 768), "Dungeon Crawler RPG"),
      gameManager(gm), showInventory(false), showHelp(false),
      backgroundColor(30, 30, 40), panelColor(50, 50, 60),
      textColor(220, 220, 220), buttonColor(70, 70, 70) {
}

bool GameGUI::initialize() {
    if (!loadFont()) {
        std::cerr << "Failed to load font!" << std::endl;
        return false;
    }
    
    setupPanels();
    setupTexts();
    createButtons();
    
    window.setFramerateLimit(60);
    return true;
}

void GameGUI::run() {
    while (window.isOpen() && gameManager->isGameRunning()) {
        handleEvents();
        update();
        render();
    }
}

void GameGUI::handleEvents() {
    sf::Event event;
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Handle button events
        for (auto& button : buttons) {
            button->handleEvent(event, mousePos);
        }
        
        // Handle keyboard shortcuts
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::I:
                    onShowInventory();
                    break;
                case sf::Keyboard::Q:
                    onQuitGame();
                    break;
                case sf::Keyboard::H:
                    showHelp = !showHelp;
                    break;
                case sf::Keyboard::Escape:
                    showInventory = false;
                    showHelp = false;
                    break;
                default:
                    break;
            }
        }
    }
}

void GameGUI::update() {
    // Update buttons
    for (auto& button : buttons) {
        button->update();
    }
    
    // Update game information
    updatePlayerInfo();
    updateRoomInfo();
    updateInventoryInfo();
    
    // Handle message timeout
    if (messageClock.getElapsedTime().asSeconds() > 3.0f) {
        currentMessage.clear();
    }
}

void GameGUI::render() {
    window.clear(backgroundColor);
    
    // Draw panels
    window.draw(playerPanel);
    window.draw(roomPanel);
    window.draw(inventoryPanel);
    window.draw(messagePanel);
    
    // Draw texts
    window.draw(titleText);
    window.draw(playerInfoText);
    window.draw(roomInfoText);
    window.draw(inventoryText);
    
    if (!currentMessage.empty()) {
        window.draw(messageText);
    }
    
    // Draw buttons
    for (auto& button : buttons) {
        button->render(window);
    }
    
    // Draw health bar
    if (gameManager->getPlayer()) {
        Player* player = gameManager->getPlayer();
        float healthPercent = static_cast<float>(player->getHealth()) / player->getMaxHealth();
        drawProgressBar(window, 20, 120, 200, 20, healthPercent, 
                       getHealthColor(player->getHealth(), player->getMaxHealth()), 
                       sf::Color(100, 100, 100));
    }
    
    window.display();
}

void GameGUI::updatePlayerInfo() {
    if (!gameManager->getPlayer()) return;
    
    Player* player = gameManager->getPlayer();
    std::stringstream ss;
    
    ss << "Player: " << player->getName() << "\n";
    ss << "Level: " << player->getLevel() << "\n";
    ss << "Health: " << player->getHealth() << "/" << player->getMaxHealth() << "\n";
    ss << "Moves: " << player->getMovesLeft() << "/" << player->getMaxMoves() << "\n";
    ss << "Gold: " << player->getGold() << "\n";
    ss << "Experience: " << player->getExperience();
    
    playerInfoText.setString(ss.str());
}

void GameGUI::updateRoomInfo() {
    if (!gameManager->getCurrentRoom()) return;
    
    auto room = gameManager->getCurrentRoom();
    std::stringstream ss;
    
    ss << "Room " << room->getRoomID() << ": " << room->getRoomType() << "\n";
    ss << room->getDescription() << "\n\n";
    
    if (room->hasEnemies()) {
        ss << "Enemies: " << room->getEnemies().size() << "\n";
    }
    
    if (room->hasItems()) {
        ss << "Items: " << room->getItems().size() << "\n";
    }
    
    auto directions = room->getAvailableDirections();
    if (!directions.empty()) {
        ss << "Exits: ";
        for (size_t i = 0; i < directions.size(); ++i) {
            ss << directions[i];
            if (i < directions.size() - 1) ss << ", ";
        }
    }
    
    roomInfoText.setString(ss.str());
}

void GameGUI::updateInventoryInfo() {
    if (!gameManager->getPlayer()) return;
    
    Player* player = gameManager->getPlayer();
    std::stringstream ss;
    
    ss << "Inventory (" << player->getInventorySize() << "/20):\n\n";
    
    if (player->getInventorySize() == 0) {
        ss << "Empty";
    } else {
        // Note: This is simplified - in a real implementation,
        // you'd need to expose inventory items through the Player interface
        ss << "Items available\n";
        ss << "(Use buttons to interact)";
    }
    
    inventoryText.setString(ss.str());
}

void GameGUI::showMessage(const std::string& message, float duration) {
    currentMessage = message;
    messageText.setString(message);
    messageClock.restart();
}

void GameGUI::createButtons() {
    clearButtons();
    createMovementButtons();
    createActionButtons();
}

void GameGUI::createMovementButtons() {
    // Movement buttons
    auto northBtn = std::make_unique<Button>(400, 500, 80, 30, "North", &font);
    northBtn->setOnClick([this]() { onMoveNorth(); });
    buttons.push_back(std::move(northBtn));
    
    auto southBtn = std::make_unique<Button>(400, 570, 80, 30, "South", &font);
    southBtn->setOnClick([this]() { onMoveSouth(); });
    buttons.push_back(std::move(southBtn));
    
    auto eastBtn = std::make_unique<Button>(490, 535, 80, 30, "East", &font);
    eastBtn->setOnClick([this]() { onMoveEast(); });
    buttons.push_back(std::move(eastBtn));
    
    auto westBtn = std::make_unique<Button>(310, 535, 80, 30, "West", &font);
    westBtn->setOnClick([this]() { onMoveWest(); });
    buttons.push_back(std::move(westBtn));
}

void GameGUI::createActionButtons() {
    // Action buttons
    auto inventoryBtn = std::make_unique<Button>(600, 500, 100, 30, "Inventory", &font);
    inventoryBtn->setOnClick([this]() { onShowInventory(); });
    buttons.push_back(std::move(inventoryBtn));
    
    auto useItemBtn = std::make_unique<Button>(600, 540, 100, 30, "Use Item", &font);
    useItemBtn->setOnClick([this]() { onUseItem(); });
    buttons.push_back(std::move(useItemBtn));
    
    auto pickupBtn = std::make_unique<Button>(600, 580, 100, 30, "Pick Up", &font);
    pickupBtn->setOnClick([this]() { onPickupItem(); });
    buttons.push_back(std::move(pickupBtn));
    
    auto statsBtn = std::make_unique<Button>(720, 500, 100, 30, "Statistics", &font);
    statsBtn->setOnClick([this]() { onShowStats(); });
    buttons.push_back(std::move(statsBtn));
    
    auto quitBtn = std::make_unique<Button>(720, 580, 100, 30, "Quit", &font);
    quitBtn->setOnClick([this]() { onQuitGame(); });
    buttons.push_back(std::move(quitBtn));
}

// Event handlers (simplified - in a real implementation, these would interact with the game)
void GameGUI::onMoveNorth() {
    showMessage("Moving North...");
    // In a real implementation, this would call gameManager->moveInDirection("north")
}

void GameGUI::onMoveSouth() {
    showMessage("Moving South...");
}

void GameGUI::onMoveEast() {
    showMessage("Moving East...");
}

void GameGUI::onMoveWest() {
    showMessage("Moving West...");
}

void GameGUI::onShowInventory() {
    showInventory = !showInventory;
    showMessage(showInventory ? "Inventory opened" : "Inventory closed");
}

void GameGUI::onUseItem() {
    showMessage("Select an item to use");
}

void GameGUI::onPickupItem() {
    showMessage("Picking up items...");
}

void GameGUI::onShowStats() {
    showMessage("Statistics displayed");
}

void GameGUI::onQuitGame() {
    showMessage("Quitting game...");
    window.close();
}

void GameGUI::centerText(sf::Text& text, const sf::RectangleShape& panel) {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect panelBounds = panel.getGlobalBounds();
    
    text.setPosition(
        panelBounds.left + (panelBounds.width - textBounds.width) / 2,
        panelBounds.top + (panelBounds.height - textBounds.height) / 2
    );
}

sf::Color GameGUI::getHealthColor(int health, int maxHealth) const {
    float ratio = static_cast<float>(health) / maxHealth;
    
    if (ratio > 0.6f) {
        return sf::Color::Green;
    } else if (ratio > 0.3f) {
        return sf::Color::Yellow;
    } else {
        return sf::Color::Red;
    }
}

void GameGUI::drawProgressBar(sf::RenderWindow& window, float x, float y, float width, float height,
                             float progress, sf::Color fillColor, sf::Color bgColor) {
    // Background
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setPosition(x, y);
    background.setFillColor(bgColor);
    window.draw(background);
    
    // Fill
    sf::RectangleShape fill(sf::Vector2f(width * progress, height));
    fill.setPosition(x, y);
    fill.setFillColor(fillColor);
    window.draw(fill);
    
    // Border
    sf::RectangleShape border(sf::Vector2f(width, height));
    border.setPosition(x, y);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color::White);
    window.draw(border);
}

bool GameGUI::loadFont() {
    // Try to load a system font (this is simplified - in a real implementation,
    // you'd include a font file with your project)
    if (!font.loadFromFile("arial.ttf")) {
        // Fallback: try to create a basic font or use default
        // For this example, we'll just return false
        // In a real implementation, you'd include a font file
        std::cerr << "Warning: Could not load font file. GUI text may not display correctly." << std::endl;
        return false;
    }
    return true;
}

void GameGUI::setupPanels() {
    // Player info panel
    playerPanel.setSize(sf::Vector2f(250, 200));
    playerPanel.setPosition(20, 20);
    playerPanel.setFillColor(panelColor);
    playerPanel.setOutlineThickness(2);
    playerPanel.setOutlineColor(sf::Color(100, 100, 100));
    
    // Room info panel
    roomPanel.setSize(sf::Vector2f(400, 200));
    roomPanel.setPosition(300, 20);
    roomPanel.setFillColor(panelColor);
    roomPanel.setOutlineThickness(2);
    roomPanel.setOutlineColor(sf::Color(100, 100, 100));
    
    // Inventory panel
    inventoryPanel.setSize(sf::Vector2f(250, 300));
    inventoryPanel.setPosition(750, 20);
    inventoryPanel.setFillColor(panelColor);
    inventoryPanel.setOutlineThickness(2);
    inventoryPanel.setOutlineColor(sf::Color(100, 100, 100));
    
    // Message panel
    messagePanel.setSize(sf::Vector2f(980, 60));
    messagePanel.setPosition(20, 680);
    messagePanel.setFillColor(sf::Color(40, 40, 50));
    messagePanel.setOutlineThickness(2);
    messagePanel.setOutlineColor(sf::Color(100, 100, 100));
}

void GameGUI::setupTexts() {
    // Title
    titleText.setFont(font);
    titleText.setString("Dungeon Crawler RPG");
    titleText.setCharacterSize(24);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(400, 250);
    
    // Player info
    playerInfoText.setFont(font);
    playerInfoText.setCharacterSize(14);
    playerInfoText.setFillColor(textColor);
    playerInfoText.setPosition(30, 30);
    
    // Room info
    roomInfoText.setFont(font);
    roomInfoText.setCharacterSize(14);
    roomInfoText.setFillColor(textColor);
    roomInfoText.setPosition(310, 30);
    
    // Inventory
    inventoryText.setFont(font);
    inventoryText.setCharacterSize(14);
    inventoryText.setFillColor(textColor);
    inventoryText.setPosition(760, 30);
    
    // Message
    messageText.setFont(font);
    messageText.setCharacterSize(16);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(30, 690);
}

void GameGUI::clearButtons() {
    buttons.clear();
}

