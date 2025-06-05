#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

/**
 * Input management system with event callbacks
 * Demonstrates lambda functions and event-driven programming
 */
class InputManager {
public:
    // Input event types
    enum class InputEvent {
        KeyPressed,
        KeyReleased,
        MousePressed,
        MouseReleased,
        MouseMoved,
        MouseWheelScrolled
    };

    // Input callback types using lambdas
    using KeyCallback = std::function<void(sf::Keyboard::Key)>;
    using MouseButtonCallback = std::function<void(sf::Mouse::Button, sf::Vector2i)>;
    using MouseMoveCallback = std::function<void(sf::Vector2i)>;
    using MouseWheelCallback = std::function<void(float)>;

private:
    // Current input states
    std::unordered_map<sf::Keyboard::Key, bool> keyStates;
    std::unordered_map<sf::Mouse::Button, bool> mouseButtonStates;
    sf::Vector2i mousePosition;
    sf::Vector2i lastMousePosition;
    
    // Event callbacks
    std::unordered_map<sf::Keyboard::Key, KeyCallback> keyPressedCallbacks;
    std::unordered_map<sf::Keyboard::Key, KeyCallback> keyReleasedCallbacks;
    std::unordered_map<sf::Mouse::Button, MouseButtonCallback> mouseButtonPressedCallbacks;
    std::unordered_map<sf::Mouse::Button, MouseButtonCallback> mouseButtonReleasedCallbacks;
    MouseMoveCallback mouseMoveCallback;
    MouseWheelCallback mouseWheelCallback;
    
    // Input bindings
    std::unordered_map<std::string, sf::Keyboard::Key> keyBindings;
    std::unordered_map<std::string, sf::Mouse::Button> mouseBindings;
    
    // Input history for combos
    std::vector<sf::Keyboard::Key> keyHistory;
    std::vector<sf::Mouse::Button> mouseHistory;
    static const size_t MAX_HISTORY_SIZE = 10;

public:
    InputManager();
    ~InputManager() = default;

    // Event processing
    void handleEvent(const sf::Event& event);
    void update();
    
    // State queries
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isKeyReleased(sf::Keyboard::Key key) const;
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    bool isMouseButtonReleased(sf::Mouse::Button button) const;
    
    // Action-based input (using bindings)
    bool isActionPressed(const std::string& action) const;
    bool isActionReleased(const std::string& action) const;
    bool isActionHeld(const std::string& action) const;
    
    // Mouse utilities
    sf::Vector2i getMousePosition() const { return mousePosition; }
    sf::Vector2i getMouseDelta() const { return mousePosition - lastMousePosition; }
    
    // Key binding management
    void bindKey(const std::string& action, sf::Keyboard::Key key);
    void bindMouseButton(const std::string& action, sf::Mouse::Button button);
    void unbindAction(const std::string& action);
    sf::Keyboard::Key getBoundKey(const std::string& action) const;
    sf::Mouse::Button getBoundMouseButton(const std::string& action) const;
    
    // Callback registration using lambdas
    void onKeyPressed(sf::Keyboard::Key key, KeyCallback callback);
    void onKeyReleased(sf::Keyboard::Key key, KeyCallback callback);
    void onMouseButtonPressed(sf::Mouse::Button button, MouseButtonCallback callback);
    void onMouseButtonReleased(sf::Mouse::Button button, MouseButtonCallback callback);
    void onMouseMoved(MouseMoveCallback callback);
    void onMouseWheelScrolled(MouseWheelCallback callback);
    
    // Clear callbacks
    void clearKeyCallbacks(sf::Keyboard::Key key);
    void clearMouseButtonCallbacks(sf::Mouse::Button button);
    void clearAllCallbacks();
    
    // Input combo detection
    bool isKeyCombo(const std::vector<sf::Keyboard::Key>& combo) const;
    bool isMouseCombo(const std::vector<sf::Mouse::Button>& combo) const;
    
    // Default key bindings setup
    void setupDefaultBindings();
    
    // Input configuration
    void loadBindingsFromFile(const std::string& filename);
    void saveBindingsToFile(const std::string& filename) const;
    
    // Utility functions
    std::string keyToString(sf::Keyboard::Key key) const;
    sf::Keyboard::Key stringToKey(const std::string& keyName) const;
    std::string mouseButtonToString(sf::Mouse::Button button) const;
    sf::Mouse::Button stringToMouseButton(const std::string& buttonName) const;

private:
    void addToKeyHistory(sf::Keyboard::Key key);
    void addToMouseHistory(sf::Mouse::Button button);
    void updateInputStates();
};

/**
 * Input context system for different game states
 */
class InputContext {
private:
    std::string name;
    std::unordered_map<std::string, std::function<void()>> actionCallbacks;
    bool active;

public:
    InputContext(const std::string& contextName);
    
    void addAction(const std::string& action, std::function<void()> callback);
    void removeAction(const std::string& action);
    void executeAction(const std::string& action);
    
    void activate() { active = true; }
    void deactivate() { active = false; }
    bool isActive() const { return active; }
    
    const std::string& getName() const { return name; }
};

/**
 * Input context manager for handling different input contexts
 */
class InputContextManager {
private:
    std::unordered_map<std::string, std::unique_ptr<InputContext>> contexts;
    std::vector<std::string> activeContexts; // Stack of active contexts
    InputManager* inputManager;

public:
    InputContextManager(InputManager* manager);
    ~InputContextManager() = default;

    // Context management
    void addContext(const std::string& name);
    void removeContext(const std::string& name);
    void pushContext(const std::string& name);
    void popContext();
    void clearContexts();
    
    // Context queries
    bool hasContext(const std::string& name) const;
    bool isContextActive(const std::string& name) const;
    InputContext* getContext(const std::string& name);
    
    // Process input for active contexts
    void processInput();
    
    // Get current active context
    std::string getCurrentContext() const;
};

