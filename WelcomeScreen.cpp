// WelcomeScreen.cpp - Welcome message, mode selection, and volume control
// Demonstrates INHERITANCE and POLYMORPHISM
#include "WelcomeScreen.h"
#include "Game.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
WelcomeScreen::WelcomeScreen(Game& gameRef)
    : Screen(gameRef),
      welcomeText(nullptr),
      modeTitle(nullptr),
      volumeText(nullptr),
      volumeBarText(nullptr),
      muteText(nullptr),
      instructionText(nullptr),
      selectedMode(0),
      keyUpReleased(true),
      keyDownReleased(true),
      keyEnterReleased(true),
      keyMReleased(true),
      arrowVisible(true)
{
    sf::Font& font = game.getFont();
    // Welcome message - will be updated each time screen is shown
    welcomeText = new sf::Text(font, "Welcome!", 36);
    welcomeText->setFillColor(sf::Color(0, 255, 0));  // Green
    welcomeText->setPosition({200.0f, 50.0f});
    modeTitle = new sf::Text(font, "Select Game Mode:", 28);
    modeTitle->setFillColor(sf::Color::White);
    modeTitle->setPosition({230.0f, 140.0f});
    // Mode options
    modeOptions[0] = new sf::Text(font, "1) THE GAUNTLET - Survival Mode", 22);
    modeOptions[0]->setFillColor(sf::Color(255, 200, 0));
    modeOptions[0]->setPosition({180.0f, 210.0f});
    modeOptions[1] = new sf::Text(font, "2) THE INFINITE WORLD - Campaign Mode", 22);
    modeOptions[1]->setFillColor(sf::Color(200, 200, 200));
    modeOptions[1]->setPosition({180.0f, 260.0f});
    // Volume controls
    volumeText = new sf::Text(font, "Volume: 50%", 20);
    volumeText->setFillColor(sf::Color(150, 150, 255));
    volumeText->setPosition({250.0f, 370.0f});
    volumeBarText = new sf::Text(font, "[=====-----] LEFT/RIGHT to adjust", 16);
    volumeBarText->setFillColor(sf::Color(150, 150, 150));
    volumeBarText->setPosition({200.0f, 405.0f});
    muteText = new sf::Text(font, "Press M to Mute/Unmute", 16);
    muteText->setFillColor(sf::Color(150, 150, 150));
    muteText->setPosition({270.0f, 435.0f});
    instructionText = new sf::Text(font, "Use UP/DOWN to select, ENTER to confirm", 18);
    instructionText->setFillColor(sf::Color(200, 200, 200));
    instructionText->setPosition({175.0f, 520.0f});
}
WelcomeScreen::~WelcomeScreen() {
    delete welcomeText;
    delete modeTitle;
    delete modeOptions[0];
    delete modeOptions[1];
    delete volumeText;
    delete volumeBarText;
    delete muteText;
    delete instructionText;
}
// POLYMORPHISM: overrides Screen::handleInput
void WelcomeScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Navigation
        if (keyEvent->code == sf::Keyboard::Key::Up && keyUpReleased) {
            selectedMode = 0;
            keyUpReleased = false;
        }
        if (keyEvent->code == sf::Keyboard::Key::Down && keyDownReleased) {
            selectedMode = 1;
            keyDownReleased = false;
        }
        // Confirm selection
        if (keyEvent->code == sf::Keyboard::Key::Enter && keyEnterReleased) {
            keyEnterReleased = false;
            if (selectedMode == 0) {
                // Survival Mode -> Level Select
                game.setGameMode(MODE_SURVIVAL);
                game.switchScreen(SCREEN_LEVEL_SELECT);
            } else {
                // Campaign Mode -> Game Screen directly
                game.setGameMode(MODE_CAMPAIGN);
                game.switchScreen(SCREEN_GAME);
            }
        }
        // Volume control
        if (keyEvent->code == sf::Keyboard::Key::Left) {
            float vol = game.getAudioManager().getVolume();
            game.getAudioManager().setVolume(vol - 5.0f);
        }
        if (keyEvent->code == sf::Keyboard::Key::Right) {
            float vol = game.getAudioManager().getVolume();
            game.getAudioManager().setVolume(vol + 5.0f);
        }
        // Mute toggle
        if (keyEvent->code == sf::Keyboard::Key::M && keyMReleased) {
            game.getAudioManager().toggleMute();
            keyMReleased = false;
        }
    }
    if (const auto* keyRel = event.getIf<sf::Event::KeyReleased>()) {
        if (keyRel->code == sf::Keyboard::Key::Up) keyUpReleased = true;
        if (keyRel->code == sf::Keyboard::Key::Down) keyDownReleased = true;
        if (keyRel->code == sf::Keyboard::Key::Enter) keyEnterReleased = true;
        if (keyRel->code == sf::Keyboard::Key::M) keyMReleased = true;
    }
}
// POLYMORPHISM: overrides Screen::update
void WelcomeScreen::update(float deltaTime) {
    // Update welcome text with player name
    const char* name = game.getPlayerName();
    char welcomeStr[80];
    int pos = 0;
    // Build "Welcome, <name>!" string manually
    const char* prefix = "Welcome, ";
    for (int i = 0; prefix[i] != '\0'; i++) {
        welcomeStr[pos++] = prefix[i];
    }
    for (int i = 0; name[i] != '\0' && i < 49; i++) {
        welcomeStr[pos++] = name[i];
    }
    welcomeStr[pos++] = '!';
    welcomeStr[pos] = '\0';
    welcomeText->setString(welcomeStr);
    // Update mode option colors based on selection
    if (selectedMode == 0) {
        modeOptions[0]->setFillColor(sf::Color(255, 200, 0));  // Highlighted gold
        modeOptions[1]->setFillColor(sf::Color(150, 150, 150)); // Dim gray
    } else {
        modeOptions[0]->setFillColor(sf::Color(150, 150, 150));
        modeOptions[1]->setFillColor(sf::Color(255, 200, 0));
    }
    // Update volume display
    float vol = game.getAudioManager().getVolume();
    bool muted = game.getAudioManager().isMuted();
    // Build volume percentage string
    char volStr[40];
    if (muted) {
        // "Volume: MUTED"
        const char* mutedStr = "Volume: MUTED";
        for (int i = 0; mutedStr[i] != '\0'; i++) {
            volStr[i] = mutedStr[i];
            volStr[i + 1] = '\0';
        }
        volumeText->setFillColor(sf::Color(255, 80, 80));  // Red when muted
    } else {
        // "Volume: XX%"
        int volInt = (int)vol;
        const char* volPrefix = "Volume: ";
        int p = 0;
        for (int i = 0; volPrefix[i] != '\0'; i++) {
            volStr[p++] = volPrefix[i];
        }
        // Convert int to string
        if (volInt >= 100) {
            volStr[p++] = '1';
            volStr[p++] = '0';
            volStr[p++] = '0';
        } else if (volInt >= 10) {
            volStr[p++] = '0' + (volInt / 10);
            volStr[p++] = '0' + (volInt % 10);
        } else {
            volStr[p++] = '0' + volInt;
        }
        volStr[p++] = '%';
        volStr[p] = '\0';
        volumeText->setFillColor(sf::Color(150, 150, 255));
    }
    volumeText->setString(volStr);
    // Build volume bar
    char barStr[60];
    int bp = 0;
    barStr[bp++] = '[';
    int filled = (int)(vol / 10.0f);
    for (int i = 0; i < 10; i++) {
        if (i < filled) {
            barStr[bp++] = '=';
        } else {
            barStr[bp++] = '-';
        }
    }
    barStr[bp++] = ']';
    barStr[bp++] = ' ';
    const char* barSuffix = "LEFT/RIGHT to adjust";
    for (int i = 0; barSuffix[i] != '\0'; i++) {
        barStr[bp++] = barSuffix[i];
    }
    barStr[bp] = '\0';
    volumeBarText->setString(barStr);
    // Blink arrow
    if (blinkClock.getElapsedTime().asSeconds() > 0.4f) {
        arrowVisible = !arrowVisible;
        blinkClock.restart();
    }
}
// POLYMORPHISM: overrides Screen::render
void WelcomeScreen::render(sf::RenderWindow& window) {
    // Black background is already set by Game::run() clear()
    // Draw all text elements
    window.draw(*welcomeText);
    window.draw(*modeTitle);
    // Draw selection arrow next to selected mode
    if (arrowVisible) {
        sf::Font& font = game.getFont();
        sf::Text arrow(font, ">>", 22);
        arrow.setFillColor(sf::Color(255, 200, 0));
        if (selectedMode == 0) {
            arrow.setPosition({140.0f, 210.0f});
        } else {
            arrow.setPosition({140.0f, 260.0f});
        }
        window.draw(arrow);
    }
    window.draw(*modeOptions[0]);
    window.draw(*modeOptions[1]);
    // Draw separator line
    sf::RectangleShape separator;
    separator.setSize({400.0f, 2.0f});
    separator.setPosition({200.0f, 340.0f});
    separator.setFillColor(sf::Color(100, 100, 100));
    window.draw(separator);
    window.draw(*volumeText);
    window.draw(*volumeBarText);
    window.draw(*muteText);
    window.draw(*instructionText);
}