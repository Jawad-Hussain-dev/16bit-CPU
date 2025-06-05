#include "InputManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

InputManager::InputManager() {
    setupDefaultBindings();
}

void InputManager::handleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            keyStates[event.key.code] = true;
            addToKeyHistory(event.key.code);
            
            // Execute callback if registered
            auto keyPressedIt = keyPressedCallbacks.find(event.key.code);
            if (keyPressedIt != keyPressedCallbacks.end()) {
                keyPressedIt->second(event.key.code);
            }
            break;
            
        case sf::Event::KeyReleased:
            keyStates[event.key.code] = false;
            
            // Execute callback if registered
            auto keyReleasedIt = keyReleasedCallbacks.find(event.key.code);
            if (keyReleasedIt != keyReleasedCallbacks.end()) {
                keyReleasedIt->second(event.key.code);
            }
            break;
            
        case sf::Event::MouseButtonPressed:
            mouseButtonStates[event.mouseButton.button] = true;
            addToMouseHistory(event.mouseButton.button);
            
            // Execute callback if registered
            auto mousePressedIt = mouseButtonPressedCallbacks.find(event.mouseButton.button);
            if (mousePressedIt != mouseButtonPressedCallbacks.end()) {
                mousePressedIt->second(event.mouseButton.button, 
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
            break;
            
        case sf::Event::MouseButtonReleased:
            mouseButtonStates[event.mouseButton.button] = false;
            
            // Execute callback if registered
            auto mouseReleasedIt = mouseButtonReleasedCallbacks.find(event.mouseButton.button);
            if (mouseReleasedIt != mouseButtonReleasedCallbacks.end()) {
                mouseReleasedIt->second(event.mouseButton.button, 
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
            break;
            
        case sf::Event::MouseMoved:
            lastMousePosition = mousePosition;
            mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            
            if (mouseMoveCallback) {
                mouseMoveCallback(mousePosition);
            }
            break;
            
        case sf::Event::MouseWheelScrolled:
            if (mouseWheelCallback) {
                mouseWheelCallback(event.mouseWheelScroll.delta);
            }
            break;
            
        default:
            break;
    }
}

void InputManager::update() {
    updateInputStates();
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const {
    auto it = keyStates.find(key);
    return it == keyStates.end() || !it->second;
}

bool InputManager::isMouseButtonPressed(sf::Mouse::Button button) const {
    auto it = mouseButtonStates.find(button);
    return it != mouseButtonStates.end() && it->second;
}

bool InputManager::isMouseButtonReleased(sf::Mouse::Button button) const {
    auto it = mouseButtonStates.find(button);
    return it == mouseButtonStates.end() || !it->second;
}

bool InputManager::isActionPressed(const std::string& action) const {
    // Check key bindings
    auto keyIt = keyBindings.find(action);
    if (keyIt != keyBindings.end()) {
        return isKeyPressed(keyIt->second);
    }
    
    // Check mouse bindings
    auto mouseIt = mouseBindings.find(action);
    if (mouseIt != mouseBindings.end()) {
        return isMouseButtonPressed(mouseIt->second);
    }
    
    return false;
}

bool InputManager::isActionReleased(const std::string& action) const {
    // Check key bindings
    auto keyIt = keyBindings.find(action);
    if (keyIt != keyBindings.end()) {
        return isKeyReleased(keyIt->second);
    }
    
    // Check mouse bindings
    auto mouseIt = mouseBindings.find(action);
    if (mouseIt != mouseBindings.end()) {
        return isMouseButtonReleased(mouseIt->second);
    }
    
    return true;
}

bool InputManager::isActionHeld(const std::string& action) const {
    return isActionPressed(action);
}

void InputManager::bindKey(const std::string& action, sf::Keyboard::Key key) {
    keyBindings[action] = key;
}

void InputManager::bindMouseButton(const std::string& action, sf::Mouse::Button button) {
    mouseBindings[action] = button;
}

void InputManager::unbindAction(const std::string& action) {
    keyBindings.erase(action);
    mouseBindings.erase(action);
}

sf::Keyboard::Key InputManager::getBoundKey(const std::string& action) const {
    auto it = keyBindings.find(action);
    return it != keyBindings.end() ? it->second : sf::Keyboard::Unknown;
}

sf::Mouse::Button InputManager::getBoundMouseButton(const std::string& action) const {
    auto it = mouseBindings.find(action);
    return it != mouseBindings.end() ? it->second : sf::Mouse::ButtonCount;
}

void InputManager::onKeyPressed(sf::Keyboard::Key key, KeyCallback callback) {
    keyPressedCallbacks[key] = callback;
}

void InputManager::onKeyReleased(sf::Keyboard::Key key, KeyCallback callback) {
    keyReleasedCallbacks[key] = callback;
}

void InputManager::onMouseButtonPressed(sf::Mouse::Button button, MouseButtonCallback callback) {
    mouseButtonPressedCallbacks[button] = callback;
}

void InputManager::onMouseButtonReleased(sf::Mouse::Button button, MouseButtonCallback callback) {
    mouseButtonReleasedCallbacks[button] = callback;
}

void InputManager::onMouseMoved(MouseMoveCallback callback) {
    mouseMoveCallback = callback;
}

void InputManager::onMouseWheelScrolled(MouseWheelCallback callback) {
    mouseWheelCallback = callback;
}

void InputManager::clearKeyCallbacks(sf::Keyboard::Key key) {
    keyPressedCallbacks.erase(key);
    keyReleasedCallbacks.erase(key);
}

void InputManager::clearMouseButtonCallbacks(sf::Mouse::Button button) {
    mouseButtonPressedCallbacks.erase(button);
    mouseButtonReleasedCallbacks.erase(button);
}

void InputManager::clearAllCallbacks() {
    keyPressedCallbacks.clear();
    keyReleasedCallbacks.clear();
    mouseButtonPressedCallbacks.clear();
    mouseButtonReleasedCallbacks.clear();
    mouseMoveCallback = nullptr;
    mouseWheelCallback = nullptr;
}

bool InputManager::isKeyCombo(const std::vector<sf::Keyboard::Key>& combo) const {
    if (combo.size() > keyHistory.size()) {
        return false;
    }
    
    // Check if the combo matches the end of the key history
    auto historyStart = keyHistory.end() - combo.size();
    return std::equal(combo.begin(), combo.end(), historyStart);
}

bool InputManager::isMouseCombo(const std::vector<sf::Mouse::Button>& combo) const {
    if (combo.size() > mouseHistory.size()) {
        return false;
    }
    
    // Check if the combo matches the end of the mouse history
    auto historyStart = mouseHistory.end() - combo.size();
    return std::equal(combo.begin(), combo.end(), historyStart);
}

void InputManager::setupDefaultBindings() {
    // Movement
    bindKey("move_up", sf::Keyboard::W);
    bindKey("move_down", sf::Keyboard::S);
    bindKey("move_left", sf::Keyboard::A);
    bindKey("move_right", sf::Keyboard::D);
    
    // Alternative movement (arrow keys)
    bindKey("move_up_alt", sf::Keyboard::Up);
    bindKey("move_down_alt", sf::Keyboard::Down);
    bindKey("move_left_alt", sf::Keyboard::Left);
    bindKey("move_right_alt", sf::Keyboard::Right);
    
    // Actions
    bindKey("attack", sf::Keyboard::Space);
    bindKey("interact", sf::Keyboard::E);
    bindKey("inventory", sf::Keyboard::I);
    bindKey("pause", sf::Keyboard::Escape);
    bindKey("special_ability", sf::Keyboard::Q);
    
    // Mouse actions
    bindMouseButton("primary_action", sf::Mouse::Left);
    bindMouseButton("secondary_action", sf::Mouse::Right);
    bindMouseButton("middle_action", sf::Mouse::Middle);
}

void InputManager::loadBindingsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not load input bindings from " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string action = line.substr(0, delimiterPos);
            std::string binding = line.substr(delimiterPos + 1);
            
            // Try to parse as key first
            sf::Keyboard::Key key = stringToKey(binding);
            if (key != sf::Keyboard::Unknown) {
                bindKey(action, key);
            } else {
                // Try to parse as mouse button
                sf::Mouse::Button button = stringToMouseButton(binding);
                if (button != sf::Mouse::ButtonCount) {
                    bindMouseButton(action, button);
                }
            }
        }
    }
    
    file.close();
    std::cout << "Loaded input bindings from " << filename << std::endl;
}

void InputManager::saveBindingsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not save input bindings to " << filename << std::endl;
        return;
    }
    
    // Save key bindings
    for (const auto& binding : keyBindings) {
        file << binding.first << "=" << keyToString(binding.second) << std::endl;
    }
    
    // Save mouse bindings
    for (const auto& binding : mouseBindings) {
        file << binding.first << "=" << mouseButtonToString(binding.second) << std::endl;
    }
    
    file.close();
    std::cout << "Saved input bindings to " << filename << std::endl;
}

std::string InputManager::keyToString(sf::Keyboard::Key key) const {
    // This is a simplified version - in a real implementation,
    // you'd want a complete mapping of all keys
    switch (key) {
        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::V: return "V";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";
        case sf::Keyboard::Space: return "Space";
        case sf::Keyboard::Escape: return "Escape";
        case sf::Keyboard::Up: return "Up";
        case sf::Keyboard::Down: return "Down";
        case sf::Keyboard::Left: return "Left";
        case sf::Keyboard::Right: return "Right";
        default: return "Unknown";
    }
}

sf::Keyboard::Key InputManager::stringToKey(const std::string& keyName) const {
    // This is a simplified version - in a real implementation,
    // you'd want a complete mapping of all keys
    if (keyName == "A") return sf::Keyboard::A;
    if (keyName == "B") return sf::Keyboard::B;
    if (keyName == "C") return sf::Keyboard::C;
    if (keyName == "D") return sf::Keyboard::D;
    if (keyName == "E") return sf::Keyboard::E;
    if (keyName == "F") return sf::Keyboard::F;
    if (keyName == "G") return sf::Keyboard::G;
    if (keyName == "H") return sf::Keyboard::H;
    if (keyName == "I") return sf::Keyboard::I;
    if (keyName == "J") return sf::Keyboard::J;
    if (keyName == "K") return sf::Keyboard::K;
    if (keyName == "L") return sf::Keyboard::L;
    if (keyName == "M") return sf::Keyboard::M;
    if (keyName == "N") return sf::Keyboard::N;
    if (keyName == "O") return sf::Keyboard::O;
    if (keyName == "P") return sf::Keyboard::P;
    if (keyName == "Q") return sf::Keyboard::Q;
    if (keyName == "R") return sf::Keyboard::R;
    if (keyName == "S") return sf::Keyboard::S;
    if (keyName == "T") return sf::Keyboard::T;
    if (keyName == "U") return sf::Keyboard::U;
    if (keyName == "V") return sf::Keyboard::V;
    if (keyName == "W") return sf::Keyboard::W;
    if (keyName == "X") return sf::Keyboard::X;
    if (keyName == "Y") return sf::Keyboard::Y;
    if (keyName == "Z") return sf::Keyboard::Z;
    if (keyName == "Space") return sf::Keyboard::Space;
    if (keyName == "Escape") return sf::Keyboard::Escape;
    if (keyName == "Up") return sf::Keyboard::Up;
    if (keyName == "Down") return sf::Keyboard::Down;
    if (keyName == "Left") return sf::Keyboard::Left;
    if (keyName == "Right") return sf::Keyboard::Right;
    
    return sf::Keyboard::Unknown;
}

std::string InputManager::mouseButtonToString(sf::Mouse::Button button) const {
    switch (button) {
        case sf::Mouse::Left: return "LeftMouse";
        case sf::Mouse::Right: return "RightMouse";
        case sf::Mouse::Middle: return "MiddleMouse";
        case sf::Mouse::XButton1: return "XButton1";
        case sf::Mouse::XButton2: return "XButton2";
        default: return "Unknown";
    }
}

sf::Mouse::Button InputManager::stringToMouseButton(const std::string& buttonName) const {
    if (buttonName == "LeftMouse") return sf::Mouse::Left;
    if (buttonName == "RightMouse") return sf::Mouse::Right;
    if (buttonName == "MiddleMouse") return sf::Mouse::Middle;
    if (buttonName == "XButton1") return sf::Mouse::XButton1;
    if (buttonName == "XButton2") return sf::Mouse::XButton2;
    
    return sf::Mouse::ButtonCount;
}

void InputManager::addToKeyHistory(sf::Keyboard::Key key) {
    keyHistory.push_back(key);
    if (keyHistory.size() > MAX_HISTORY_SIZE) {
        keyHistory.erase(keyHistory.begin());
    }
}

void InputManager::addToMouseHistory(sf::Mouse::Button button) {
    mouseHistory.push_back(button);
    if (mouseHistory.size() > MAX_HISTORY_SIZE) {
        mouseHistory.erase(mouseHistory.begin());
    }
}

void InputManager::updateInputStates() {
    // Update real-time key states
    for (auto& state : keyStates) {
        state.second = sf::Keyboard::isKeyPressed(state.first);
    }
    
    // Update real-time mouse button states
    for (auto& state : mouseButtonStates) {
        state.second = sf::Mouse::isButtonPressed(state.first);
    }
}

// InputContext implementation
InputContext::InputContext(const std::string& contextName) : name(contextName), active(false) {
}

void InputContext::addAction(const std::string& action, std::function<void()> callback) {
    actionCallbacks[action] = callback;
}

void InputContext::removeAction(const std::string& action) {
    actionCallbacks.erase(action);
}

void InputContext::executeAction(const std::string& action) {
    if (!active) return;
    
    auto it = actionCallbacks.find(action);
    if (it != actionCallbacks.end()) {
        it->second();
    }
}

// InputContextManager implementation
InputContextManager::InputContextManager(InputManager* manager) : inputManager(manager) {
}

void InputContextManager::addContext(const std::string& name) {
    contexts[name] = std::make_unique<InputContext>(name);
}

void InputContextManager::removeContext(const std::string& name) {
    // Remove from active contexts if present
    auto it = std::find(activeContexts.begin(), activeContexts.end(), name);
    if (it != activeContexts.end()) {
        activeContexts.erase(it);
    }
    
    // Remove from contexts map
    contexts.erase(name);
}

void InputContextManager::pushContext(const std::string& name) {
    auto it = contexts.find(name);
    if (it != contexts.end()) {
        activeContexts.push_back(name);
        it->second->activate();
    }
}

void InputContextManager::popContext() {
    if (!activeContexts.empty()) {
        std::string contextName = activeContexts.back();
        activeContexts.pop_back();
        
        auto it = contexts.find(contextName);
        if (it != contexts.end()) {
            it->second->deactivate();
        }
    }
}

void InputContextManager::clearContexts() {
    for (const std::string& contextName : activeContexts) {
        auto it = contexts.find(contextName);
        if (it != contexts.end()) {
            it->second->deactivate();
        }
    }
    activeContexts.clear();
}

bool InputContextManager::hasContext(const std::string& name) const {
    return contexts.find(name) != contexts.end();
}

bool InputContextManager::isContextActive(const std::string& name) const {
    return std::find(activeContexts.begin(), activeContexts.end(), name) != activeContexts.end();
}

InputContext* InputContextManager::getContext(const std::string& name) {
    auto it = contexts.find(name);
    return it != contexts.end() ? it->second.get() : nullptr;
}

void InputContextManager::processInput() {
    // Process input for all active contexts (in reverse order - top of stack first)
    for (auto it = activeContexts.rbegin(); it != activeContexts.rend(); ++it) {
        auto contextIt = contexts.find(*it);
        if (contextIt != contexts.end() && contextIt->second->isActive()) {
            // Check all bound actions for this context
            // This would need to be implemented based on your specific action system
        }
    }
}

std::string InputContextManager::getCurrentContext() const {
    return activeContexts.empty() ? "" : activeContexts.back();
}

