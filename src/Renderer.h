#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

// Forward declarations
class GameEntity;
class Drawable;

/**
 * Renderer class for SFML graphics management
 * Demonstrates separation of concerns between graphics and game logic
 */
class Renderer {
private:
    std::unique_ptr<sf::RenderWindow> window;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    sf::View gameView;
    sf::View uiView;
    
    // Rendering layers
    std::vector<std::shared_ptr<Drawable>> backgroundLayer;
    std::vector<std::shared_ptr<Drawable>> gameLayer;
    std::vector<std::shared_ptr<Drawable>> effectLayer;
    std::vector<std::shared_ptr<Drawable>> uiLayer;
    
    // Screen properties
    unsigned int screenWidth;
    unsigned int screenHeight;
    bool fullscreen;
    
    // Camera properties
    sf::Vector2f cameraPosition;
    float cameraZoom;

public:
    Renderer(unsigned int width = 1024, unsigned int height = 768, const std::string& title = "Dungeon Crawler");
    ~Renderer() = default;

    // Window management
    bool initialize();
    void shutdown();
    bool isOpen() const;
    void close();
    
    // Event handling
    bool pollEvent(sf::Event& event);
    
    // Rendering
    void clear(const sf::Color& color = sf::Color::Black);
    void display();
    void render();
    
    // Layer management
    void addToLayer(std::shared_ptr<Drawable> drawable, const std::string& layer);
    void removeFromLayer(std::shared_ptr<Drawable> drawable, const std::string& layer);
    void clearLayer(const std::string& layer);
    
    // Resource management
    bool loadTexture(const std::string& name, const std::string& filename);
    bool loadFont(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    sf::Font& getFont(const std::string& name);
    
    // Camera control
    void setCameraPosition(float x, float y);
    void setCameraPosition(const sf::Vector2f& position);
    void moveCameraBy(float dx, float dy);
    void setCameraZoom(float zoom);
    void zoomCameraBy(float factor);
    sf::Vector2f getCameraPosition() const { return cameraPosition; }
    float getCameraZoom() const { return cameraZoom; }
    
    // View management
    void setGameView();
    void setUIView();
    
    // Drawing utilities
    void drawSprite(const sf::Sprite& sprite);
    void drawText(const sf::Text& text);
    void drawShape(const sf::Shape& shape);
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color = sf::Color::White);
    void drawRectangle(const sf::FloatRect& rect, const sf::Color& fillColor = sf::Color::Transparent, 
                      const sf::Color& outlineColor = sf::Color::White, float outlineThickness = 1.0f);
    void drawCircle(const sf::Vector2f& center, float radius, const sf::Color& fillColor = sf::Color::Transparent,
                   const sf::Color& outlineColor = sf::Color::White, float outlineThickness = 1.0f);
    
    // Screen utilities
    sf::Vector2f screenToWorld(const sf::Vector2i& screenPos) const;
    sf::Vector2i worldToScreen(const sf::Vector2f& worldPos) const;
    sf::FloatRect getViewBounds() const;
    
    // Getters
    sf::RenderWindow& getWindow() { return *window; }
    const sf::RenderWindow& getWindow() const { return *window; }
    unsigned int getWidth() const { return screenWidth; }
    unsigned int getHeight() const { return screenHeight; }
    
    // Settings
    void setFullscreen(bool fullscreen);
    void setVSync(bool enabled);
    void setFramerateLimit(unsigned int limit);

private:
    void updateViews();
    std::vector<std::shared_ptr<Drawable>>& getLayer(const std::string& layerName);
};

/**
 * Animation system for sprites
 */
class Animation {
private:
    std::vector<sf::IntRect> frames;
    float frameDuration;
    float currentTime;
    size_t currentFrame;
    bool looping;
    bool playing;

public:
    Animation(float duration = 0.1f, bool loop = true);
    
    void addFrame(const sf::IntRect& frame);
    void addFrames(const std::vector<sf::IntRect>& frameList);
    
    void play();
    void pause();
    void stop();
    void reset();
    
    void update(float deltaTime);
    const sf::IntRect& getCurrentFrame() const;
    
    bool isPlaying() const { return playing; }
    bool isFinished() const;
    
    void setFrameDuration(float duration) { frameDuration = duration; }
    void setLooping(bool loop) { looping = loop; }
};

/**
 * Animated sprite class
 */
class AnimatedSprite : public sf::Drawable, public sf::Transformable {
private:
    sf::Sprite sprite;
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation;
    
public:
    AnimatedSprite();
    explicit AnimatedSprite(const sf::Texture& texture);
    
    void setTexture(const sf::Texture& texture);
    void addAnimation(const std::string& name, const Animation& animation);
    void playAnimation(const std::string& name);
    void update(float deltaTime);
    
    // sf::Drawable interface
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    // Getters
    const sf::Sprite& getSprite() const { return sprite; }
    sf::Sprite& getSprite() { return sprite; }
    const std::string& getCurrentAnimation() const { return currentAnimation; }
};

