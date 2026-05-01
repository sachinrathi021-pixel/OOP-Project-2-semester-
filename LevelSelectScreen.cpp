// LevelSelectScreen.cpp - Level selection for Survival mode
// Demonstrates INHERITANCE and POLYMORPHISM
#include "LevelSelectScreen.h"
#include "Game.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
LevelSelectScreen::LevelSelectScreen(Game& gameRef)
    : Screen(gameRef),
      bgSprite(nullptr),
      titleText(nullptr),
      instructionText(nullptr),
      selectedLevel(0),
      texturesLoaded(false),
      keyUpReleased(true),
      keyDownReleased(true),
      keyEnterReleased(true),
      arrowVisible(true)
{
    // Load background texture
    if (bgTexture.loadFromFile("Sprites/Select Option.jpg")) {
        bgSprite = new sf::Sprite(bgTexture);
        // Scale to fill window
        sf::Vector2u texSize = bgTexture.getSize();
        float scaleX = 800.0f / texSize.x;
        float scaleY = 600.0f / texSize.y;
        bgSprite->setScale({scaleX, scaleY});
        texturesLoaded = true;
    } else {
        std::cout << "Error: Could not load Select Option.jpg" << std::endl;
    }
    sf::Font& font = game.getFont();
    titleText = new sf::Text(font, "SELECT LEVEL - THE GAUNTLET", 32);
    titleText->setFillColor(sf::Color(255, 200, 0));
    titleText->setPosition({130.0f, 60.0f});
    // Level options
    const char* levelNames[4] = {
        "Level 1 - Desert Assault",
        "Level 2 - Harbor Siege",
        "Level 3 - Mountain Pass",
        "Boss Level - Final Stand"
    };
    for (int i = 0; i < 4; i++) {
        levelOptions[i] = new sf::Text(font, levelNames[i], 24);
        levelOptions[i]->setPosition({220.0f, 200.0f + i * 60.0f});
        if (i == 0) {
            levelOptions[i]->setFillColor(sf::Color(255, 200, 0));
        } else {
            levelOptions[i]->setFillColor(sf::Color(200, 200, 200));
        }
    }
    instructionText = new sf::Text(font, "Use UP/DOWN to select, ENTER to confirm", 18);
    instructionText->setFillColor(sf::Color(200, 200, 200));
    instructionText->setPosition({175.0f, 520.0f});
}
LevelSelectScreen::~LevelSelectScreen() {
    delete bgSprite;
    delete titleText;
    for (int i = 0; i < 4; i++) {
        delete levelOptions[i];
    }
    delete instructionText;
}
// POLYMORPHISM: overrides Screen::handleInput
void LevelSelectScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Navigate up
        if (keyEvent->code == sf::Keyboard::Key::Up && keyUpReleased) {
            selectedLevel--;
            if (selectedLevel < 0) selectedLevel = 3;
            keyUpReleased = false;
        }
        // Navigate down
        if (keyEvent->code == sf::Keyboard::Key::Down && keyDownReleased) {
            selectedLevel++;
            if (selectedLevel > 3) selectedLevel = 0;
            keyDownReleased = false;
        }
        // Confirm selection
        if (keyEvent->code == sf::Keyboard::Key::Enter && keyEnterReleased) {
            keyEnterReleased = false;
            game.setSelectedLevel(selectedLevel + 1); // 1-based level number
            game.switchScreen(SCREEN_GAME);
        }
        // Go back
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game.switchScreen(SCREEN_WELCOME);
        }
    }
    if (const auto* keyRel = event.getIf<sf::Event::KeyReleased>()) {
        if (keyRel->code == sf::Keyboard::Key::Up) keyUpReleased = true;
        if (keyRel->code == sf::Keyboard::Key::Down) keyDownReleased = true;
        if (keyRel->code == sf::Keyboard::Key::Enter) keyEnterReleased = true;
    }
}
// POLYMORPHISM: overrides Screen::update
void LevelSelectScreen::update(float deltaTime) {
    // Update option colors based on selection
    for (int i = 0; i < 4; i++) {
        if (i == selectedLevel) {
            levelOptions[i]->setFillColor(sf::Color(255, 200, 0));  // Gold highlight
        } else {
            levelOptions[i]->setFillColor(sf::Color(200, 200, 200)); // Gray
        }
    }
    // Blink arrow indicator
    if (blinkClock.getElapsedTime().asSeconds() > 0.4f) {
        arrowVisible = !arrowVisible;
        blinkClock.restart();
    }
}
// POLYMORPHISM: overrides Screen::render
void LevelSelectScreen::render(sf::RenderWindow& window) {
    // Draw background
    if (texturesLoaded && bgSprite != nullptr) {
        window.draw(*bgSprite);
    }
    // Draw semi-transparent overlay for readability
    sf::RectangleShape overlay;
    overlay.setSize({500.0f, 420.0f});
    overlay.setPosition({150.0f, 40.0f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    overlay.setOutlineColor(sf::Color(255, 200, 0));
    overlay.setOutlineThickness(2.0f);
    window.draw(overlay);
    // Draw title
    window.draw(*titleText);
    // Draw selection arrow
    if (arrowVisible) {
        sf::Font& font = game.getFont();
        sf::Text arrow(font, ">>", 24);
        arrow.setFillColor(sf::Color(255, 200, 0));
        arrow.setPosition({175.0f, 200.0f + selectedLevel * 60.0f});
        window.draw(arrow);
    }
    // Draw level options
    for (int i = 0; i < 4; i++) {
        window.draw(*levelOptions[i]);
    }
    // Draw instruction
    window.draw(*instructionText);
}