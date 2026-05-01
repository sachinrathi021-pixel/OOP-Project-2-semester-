// LandingScreen.cpp - Landing page with background image and name input
// Demonstrates INHERITANCE and POLYMORPHISM
#include "LandingScreen.h"
#include "Game.h"
#include <iostream>
LandingScreen::LandingScreen(Game& gameRef)
    : Screen(gameRef),
      bgSprite(nullptr),
      titleText(nullptr),
      promptText(nullptr),
      nameText(nullptr),
      instructionText(nullptr),
      inputLength(0),
      cursorVisible(true),
      texturesLoaded(false)
{
    // Initialize input buffer
    for (int i = 0; i < 50; i++) {
        inputName[i] = '\0';
    }
    // Load background texture
    if (bgTexture.loadFromFile("Sprites/Landing Page.jpg")) {
        bgSprite = new sf::Sprite(bgTexture);
        // Scale sprite to fill 800x600 window
        sf::Vector2u texSize = bgTexture.getSize();
        float scaleX = 800.0f / texSize.x;
        float scaleY = 600.0f / texSize.y;
        bgSprite->setScale({scaleX, scaleY});
        texturesLoaded = true;
    } else {
        std::cout << "Error: Could not load Landing Page.jpg" << std::endl;
    }
    // Create text objects using the shared font (ASSOCIATION: using Game's font)
    sf::Font& font = game.getFont();
    titleText = new sf::Text(font, "METAL SLUG", 48);
    titleText->setFillColor(sf::Color(255, 200, 0));  // Gold color
    titleText->setPosition({200.0f, 30.0f});
    promptText = new sf::Text(font, "Enter Your Name:", 24);
    promptText->setFillColor(sf::Color::White);
    promptText->setPosition({270.0f, 400.0f});
    nameText = new sf::Text(font, "_", 28);
    nameText->setFillColor(sf::Color(0, 255, 0));  // Green
    nameText->setPosition({290.0f, 445.0f});
    instructionText = new sf::Text(font, "Press ENTER to continue", 18);
    instructionText->setFillColor(sf::Color(200, 200, 200));
    instructionText->setPosition({275.0f, 510.0f});
}
LandingScreen::~LandingScreen() {
    delete bgSprite;
    delete titleText;
    delete promptText;
    delete nameText;
    delete instructionText;
}
// POLYMORPHISM: overrides Screen::handleInput
void LandingScreen::handleInput(const sf::Event& event) {
    // Handle text input for name
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = textEvent->unicode;
        // Backspace
        if (unicode == 8) {
            if (inputLength > 0) {
                inputLength--;
                inputName[inputLength] = '\0';
            }
        }
        // Enter key
        else if (unicode == 13) {
            if (inputLength > 0) {
                // Store name in Game and switch to Welcome screen
                game.setPlayerName(inputName, inputLength);
                game.switchScreen(SCREEN_WELCOME);
            }
        }
        // Printable ASCII characters (space to tilde)
        else if (unicode >= 32 && unicode < 127 && inputLength < 20) {
            inputName[inputLength] = (char)unicode;
            inputLength++;
            inputName[inputLength] = '\0';
        }
    }
}
// POLYMORPHISM: overrides Screen::update
void LandingScreen::update(float deltaTime) {
    // Blink cursor every 0.5 seconds
    if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        cursorVisible = !cursorVisible;
        cursorClock.restart();
    }
    // Update displayed name text with cursor
    char displayStr[55];
    int pos = 0;
    for (int i = 0; i < inputLength && i < 49; i++) {
        displayStr[pos] = inputName[i];
        pos++;
    }
    if (cursorVisible) {
        displayStr[pos] = '_';
        pos++;
    }
    displayStr[pos] = '\0';
    nameText->setString(displayStr);
}
// POLYMORPHISM: overrides Screen::render
void LandingScreen::render(sf::RenderWindow& window) {
    // Draw background
    if (texturesLoaded && bgSprite != nullptr) {
        window.draw(*bgSprite);
    }
    // Draw semi-transparent dark overlay box for name input area
    sf::RectangleShape inputBox;
    inputBox.setSize({320.0f, 190.0f});
    inputBox.setPosition({240.0f, 385.0f});
    inputBox.setFillColor(sf::Color(0, 0, 0, 180));
    inputBox.setOutlineColor(sf::Color(255, 200, 0));
    inputBox.setOutlineThickness(2.0f);
    window.draw(inputBox);
    // Draw text elements
    if (titleText != nullptr) window.draw(*titleText);
    if (promptText != nullptr) window.draw(*promptText);
    if (nameText != nullptr) window.draw(*nameText);
    if (instructionText != nullptr) window.draw(*instructionText);
}
