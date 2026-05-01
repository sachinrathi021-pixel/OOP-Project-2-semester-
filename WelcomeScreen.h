#pragma once
// WelcomeScreen.h - Welcome message and game mode selection
// Demonstrates INHERITANCE from Screen
#include "Screen.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
class WelcomeScreen : public Screen {  // INHERITANCE
private:
    // ENCAPSULATION: private data
    sf::Text* welcomeText;
    sf::Text* modeTitle;
    sf::Text* modeOptions[2];      // Survival and Campaign
    sf::Text* volumeText;
    sf::Text* volumeBarText;
    sf::Text* muteText;
    sf::Text* instructionText;
    int selectedMode;              // 0 = Survival, 1 = Campaign
    bool keyUpReleased;
    bool keyDownReleased;
    bool keyEnterReleased;
    bool keyMReleased;
    // Blinking arrow indicator
    sf::Clock blinkClock;
    bool arrowVisible;
public:
    WelcomeScreen(Game& gameRef);
    ~WelcomeScreen();
    // POLYMORPHISM: override base class virtual functions
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
};
