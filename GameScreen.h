#pragma once
// GameScreen.h - Game screen with level/biome rendering
// Demonstrates INHERITANCE from Screen
#include "Screen.h"
#include <SFML/Graphics/Text.hpp>
#include "Level.h"
#include "NoiseProfile.h"

class GameScreen : public Screen {  // INHERITANCE
private:
    // ENCAPSULATION
    // Level system
    Level* currentLevel;
    NoiseProfile* campaignProfile; // Owned for campaign mode
    float cameraX;
    float scrollSpeed;
    // HUD text
    sf::Text* infoText;
    sf::Text* biomeText;
    sf::Text* positionText;
    sf::Text* backText;
    // Input state
    bool keyEscReleased;
    bool rightHeld;
    bool leftHeld;
    // Track if level has been generated for current mode/level
    int lastGameMode;
    int lastSelectedLevel;
    bool levelGenerated;
public:
    GameScreen(Game& gameRef);
    ~GameScreen();
    // POLYMORPHISM: override pure virtual functions
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
private:
    // Helper: generate/regenerate the level based on current game settings
    void generateLevel();
};
