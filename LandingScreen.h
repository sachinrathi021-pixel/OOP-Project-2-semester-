#pragma once
// LandingScreen.h - Landing page with name input
// Demonstrates INHERITANCE from Screen
#include "Screen.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
class LandingScreen : public Screen {  // INHERITANCE
private:
    // ENCAPSULATION: all rendering data is private
    sf::Texture bgTexture;
    sf::Sprite* bgSprite;
    sf::Text* titleText;
    sf::Text* promptText;
    sf::Text* nameText;
    sf::Text* instructionText;
    // Name input data
    char inputName[50];
    int inputLength;
    // Cursor blink
    sf::Clock cursorClock;
    bool cursorVisible;
    bool texturesLoaded;
public:
    LandingScreen(Game& gameRef);
    ~LandingScreen();  // Override destructor for cleanup
    // POLYMORPHISM: these override pure virtual functions from Screen
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
