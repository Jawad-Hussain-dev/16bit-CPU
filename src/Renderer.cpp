#include "Renderer.h"
#include "Drawable.h"
#include "GameExceptions.h"
#include <iostream>
#include <algorithm>

Renderer::Renderer(unsigned int width, unsigned int height, const std::string& title)
    : screenWidth(width), screenHeight(height), fullscreen(false),
      cameraPosition(0.0f, 0.0f), cameraZoom(1.0f) {
    
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width, height), title, sf::Style::Default);
}

bool Renderer::initialize() {
    if (!window) {
        std::cerr << "Failed to create SFML window\n";
        return false;
    }
    
    // Set up views
    gameView.setSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
    gameView.setCenter(cameraPosition);
    
    uiView.setSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
    uiView.setCenter(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) / 2.0f);
    
    // Load default resources
    if (!loadFont("default", "assets/fonts/arial.ttf")) {
        std::cout << "Warning: Could not load default font\n";
    }
    
    // Set default settings
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    
    std::cout << "Renderer initialized successfully\n";
    return true;
}

void Renderer::shutdown() {
    if (window && window->isOpen()) {
        window->close();
    }
    
    // Clear all layers
    backgroundLayer.clear();
    gameLayer.clear();
    effectLayer.clear();
    uiLayer.clear();
    
    // Clear resources
    textures.clear();
    fonts.clear();
    
    std::cout << "Renderer shut down\n";
}

bool Renderer::isOpen() const {
    return window && window->isOpen();
}

void Renderer::close() {
    if (window) {
        window->close();
    }
}

bool Renderer::pollEvent(sf::Event& event) {
    return window && window->pollEvent(event);
}

void Renderer::clear(const sf::Color& color) {
    if (window) {
        window->clear(color);
    }
}

void Renderer::display() {
    if (window) {
        window->display();
    }
}

void Renderer::render() {
    if (!window) return;
    
    clear();
    
    // Render game layers with game view
    setGameView();
    
    // Render background layer
    for (auto& drawable : backgroundLayer) {
        if (drawable) {
            drawable->draw(*window);
        }
    }
    
    // Render game layer
    for (auto& drawable : gameLayer) {
        if (drawable) {
            drawable->draw(*window);
        }
    }
    
    // Render effect layer
    for (auto& drawable : effectLayer) {
        if (drawable) {
            drawable->draw(*window);
        }
    }
    
    // Render UI layer with UI view
    setUIView();
    for (auto& drawable : uiLayer) {
        if (drawable) {
            drawable->draw(*window);
        }
    }
    
    display();
}

void Renderer::addToLayer(std::shared_ptr<Drawable> drawable, const std::string& layer) {
    if (!drawable) return;
    
    auto& layerVector = getLayer(layer);
    layerVector.push_back(drawable);
}

void Renderer::removeFromLayer(std::shared_ptr<Drawable> drawable, const std::string& layer) {
    if (!drawable) return;
    
    auto& layerVector = getLayer(layer);
    layerVector.erase(
        std::remove(layerVector.begin(), layerVector.end(), drawable),
        layerVector.end()
    );
}

void Renderer::clearLayer(const std::string& layer) {
    getLayer(layer).clear();
}

bool Renderer::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (texture.loadFromFile(filename)) {
        textures[name] = std::move(texture);
        return true;
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }
}

bool Renderer::loadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (font.loadFromFile(filename)) {
        fonts[name] = std::move(font);
        return true;
    } else {
        std::cerr << "Failed to load font: " << filename << std::endl;
        return false;
    }
}

sf::Texture& Renderer::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    } else {
        throw ResourceLoadException("Texture not found: " + name);
    }
}

sf::Font& Renderer::getFont(const std::string& name) {
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        return it->second;
    } else {
        throw ResourceLoadException("Font not found: " + name);
    }
}

void Renderer::setCameraPosition(float x, float y) {
    cameraPosition.x = x;
    cameraPosition.y = y;
    updateViews();
}

void Renderer::setCameraPosition(const sf::Vector2f& position) {
    cameraPosition = position;
    updateViews();
}

void Renderer::moveCameraBy(float dx, float dy) {
    cameraPosition.x += dx;
    cameraPosition.y += dy;
    updateViews();
}

void Renderer::setCameraZoom(float zoom) {
    cameraZoom = std::max(0.1f, zoom); // Minimum zoom level
    updateViews();
}

void Renderer::zoomCameraBy(float factor) {
    setCameraZoom(cameraZoom * factor);
}

void Renderer::setGameView() {
    if (window) {
        window->setView(gameView);
    }
}

void Renderer::setUIView() {
    if (window) {
        window->setView(uiView);
    }
}

void Renderer::drawSprite(const sf::Sprite& sprite) {
    if (window) {
        window->draw(sprite);
    }
}

void Renderer::drawText(const sf::Text& text) {
    if (window) {
        window->draw(text);
    }
}

void Renderer::drawShape(const sf::Shape& shape) {
    if (window) {
        window->draw(shape);
    }
}

void Renderer::drawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color) {
    sf::Vertex line[] = {
        sf::Vertex(start, color),
        sf::Vertex(end, color)
    };
    
    if (window) {
        window->draw(line, 2, sf::Lines);
    }
}

void Renderer::drawRectangle(const sf::FloatRect& rect, const sf::Color& fillColor, 
                            const sf::Color& outlineColor, float outlineThickness) {
    sf::RectangleShape rectangle;
    rectangle.setPosition(rect.left, rect.top);
    rectangle.setSize(sf::Vector2f(rect.width, rect.height));
    rectangle.setFillColor(fillColor);
    rectangle.setOutlineColor(outlineColor);
    rectangle.setOutlineThickness(outlineThickness);
    
    drawShape(rectangle);
}

void Renderer::drawCircle(const sf::Vector2f& center, float radius, const sf::Color& fillColor,
                         const sf::Color& outlineColor, float outlineThickness) {
    sf::CircleShape circle;
    circle.setPosition(center.x - radius, center.y - radius);
    circle.setRadius(radius);
    circle.setFillColor(fillColor);
    circle.setOutlineColor(outlineColor);
    circle.setOutlineThickness(outlineThickness);
    
    drawShape(circle);
}

sf::Vector2f Renderer::screenToWorld(const sf::Vector2i& screenPos) const {
    if (window) {
        return window->mapPixelToCoords(screenPos, gameView);
    }
    return sf::Vector2f(0, 0);
}

sf::Vector2i Renderer::worldToScreen(const sf::Vector2f& worldPos) const {
    if (window) {
        return window->mapCoordsToPixel(worldPos, gameView);
    }
    return sf::Vector2i(0, 0);
}

sf::FloatRect Renderer::getViewBounds() const {
    sf::Vector2f center = gameView.getCenter();
    sf::Vector2f size = gameView.getSize();
    
    return sf::FloatRect(
        center.x - size.x / 2.0f,
        center.y - size.y / 2.0f,
        size.x,
        size.y
    );
}

void Renderer::setFullscreen(bool fs) {
    fullscreen = fs;
    
    if (window) {
        window->close();
        
        if (fullscreen) {
            window->create(sf::VideoMode::getDesktopMode(), "Dungeon Crawler", sf::Style::Fullscreen);
        } else {
            window->create(sf::VideoMode(screenWidth, screenHeight), "Dungeon Crawler", sf::Style::Default);
        }
        
        updateViews();
    }
}

void Renderer::setVSync(bool enabled) {
    if (window) {
        window->setVerticalSyncEnabled(enabled);
    }
}

void Renderer::setFramerateLimit(unsigned int limit) {
    if (window) {
        window->setFramerateLimit(limit);
    }
}

void Renderer::updateViews() {
    gameView.setCenter(cameraPosition);
    gameView.setSize(
        static_cast<float>(screenWidth) / cameraZoom,
        static_cast<float>(screenHeight) / cameraZoom
    );
}

std::vector<std::shared_ptr<Drawable>>& Renderer::getLayer(const std::string& layerName) {
    if (layerName == "background") return backgroundLayer;
    if (layerName == "game") return gameLayer;
    if (layerName == "effect") return effectLayer;
    if (layerName == "ui") return uiLayer;
    
    // Default to game layer
    return gameLayer;
}

// Animation implementation
Animation::Animation(float duration, bool loop)
    : frameDuration(duration), currentTime(0.0f), currentFrame(0),
      looping(loop), playing(false) {
}

void Animation::addFrame(const sf::IntRect& frame) {
    frames.push_back(frame);
}

void Animation::addFrames(const std::vector<sf::IntRect>& frameList) {
    frames.insert(frames.end(), frameList.begin(), frameList.end());
}

void Animation::play() {
    playing = true;
}

void Animation::pause() {
    playing = false;
}

void Animation::stop() {
    playing = false;
    reset();
}

void Animation::reset() {
    currentTime = 0.0f;
    currentFrame = 0;
}

void Animation::update(float deltaTime) {
    if (!playing || frames.empty()) return;
    
    currentTime += deltaTime;
    
    if (currentTime >= frameDuration) {
        currentTime = 0.0f;
        currentFrame++;
        
        if (currentFrame >= frames.size()) {
            if (looping) {
                currentFrame = 0;
            } else {
                currentFrame = frames.size() - 1;
                playing = false;
            }
        }
    }
}

const sf::IntRect& Animation::getCurrentFrame() const {
    if (frames.empty()) {
        static sf::IntRect emptyFrame(0, 0, 0, 0);
        return emptyFrame;
    }
    
    return frames[currentFrame];
}

bool Animation::isFinished() const {
    return !looping && currentFrame >= frames.size() - 1 && !playing;
}

// AnimatedSprite implementation
AnimatedSprite::AnimatedSprite() {
}

AnimatedSprite::AnimatedSprite(const sf::Texture& texture) {
    sprite.setTexture(texture);
}

void AnimatedSprite::setTexture(const sf::Texture& texture) {
    sprite.setTexture(texture);
}

void AnimatedSprite::addAnimation(const std::string& name, const Animation& animation) {
    animations[name] = animation;
}

void AnimatedSprite::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        currentAnimation = name;
        it->second.play();
    }
}

void AnimatedSprite::update(float deltaTime) {
    if (!currentAnimation.empty()) {
        auto it = animations.find(currentAnimation);
        if (it != animations.end()) {
            it->second.update(deltaTime);
            sprite.setTextureRect(it->second.getCurrentFrame());
        }
    }
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}

