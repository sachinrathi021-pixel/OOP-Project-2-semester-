#pragma once
// LevelSelectScreen.h - Level selection for Survival mode
// Demonstrates INHERITANCE from Screen
#include "Screen.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
class LevelSelectScreen : public Screen {  // INHERITANCE
private:
    // ENCAPSULATION: private rendering data
    sf::Texture bgTexture;
    sf::Sprite* bgSprite;
    sf::Text* titleText;
    sf::Text* levelOptions[4];  // Level 1, 2, 3, Boss
    sf::Text* instructionText;
    int selectedLevel;          // 0-3
    bool texturesLoaded;
    bool keyUpReleased;
    bool keyDownReleased;
    bool keyEnterReleased;
    // Blinking indicator
    sf::Clock blinkClock;
    bool arrowVisible;
public:
    LevelSelectScreen(Game& gameRef);
    ~LevelSelectScreen();
    // POLYMORPHISM: override pure virtual functions
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};