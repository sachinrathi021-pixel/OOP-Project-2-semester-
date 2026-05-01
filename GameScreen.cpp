// GameScreen.cpp - Game screen with level/biome rendering
// Demonstrates INHERITANCE and POLYMORPHISM
#include "GameScreen.h"
#include "Game.h"
#include <SFML/Graphics/RectangleShape.hpp>

GameScreen::GameScreen(Game& gameRef)
    : Screen(gameRef),
      currentLevel(nullptr),
      campaignProfile(nullptr),
      cameraX(0.0f),
      scrollSpeed(200.0f),
      infoText(nullptr),
      biomeText(nullptr),
      positionText(nullptr),
      backText(nullptr),
      keyEscReleased(true),
      rightHeld(false),
      leftHeld(false),
      lastGameMode(-1),
      lastSelectedLevel(-1),
      levelGenerated(false)
{
    sf::Font& font = game.getFont();
    infoText = new sf::Text(font, "GAME SCREEN", 20);
    infoText->setFillColor(sf::Color(255, 200, 0));
    infoText->setPosition({10.0f, 10.0f});
    biomeText = new sf::Text(font, "Biome: ---", 18);
    biomeText->setFillColor(sf::Color(0, 255, 0));
    biomeText->setPosition({10.0f, 35.0f});
    positionText = new sf::Text(font, "Pos: 0", 16);
    positionText->setFillColor(sf::Color(200, 200, 200));
    positionText->setPosition({10.0f, 58.0f});
    backText = new sf::Text(font, "ESC: Menu  |  LEFT/RIGHT: Scroll", 14);
    backText->setFillColor(sf::Color(150, 150, 150));
    backText->setPosition({10.0f, 580.0f});
}

GameScreen::~GameScreen() {
    delete currentLevel;
    delete campaignProfile;
    delete infoText;
    delete biomeText;
    delete positionText;
    delete backText;
}

void GameScreen::generateLevel() {
    // Clean up old level
    delete currentLevel;
    currentLevel = nullptr;
    delete campaignProfile;
    campaignProfile = nullptr;
    cameraX = 0.0f;
    int mode = game.getGameMode();
    int level = game.getSelectedLevel();
    currentLevel = new Level();
    if (mode == MODE_SURVIVAL) {
        if (level == 4) {
            // Boss level
            currentLevel->generateBoss(level * 42 + 7);
        } else {
            // Regular survival levels 1-3
            currentLevel->generateSurvival(level * 31 + 5, level);
        }
    } else if (mode == MODE_CAMPAIGN) {
        // Campaign mode: use NormalProfile by default
        campaignProfile = new NormalProfile();
        currentLevel->generateCampaign(12345, campaignProfile);
    }
    lastGameMode = mode;
    lastSelectedLevel = level;
    levelGenerated = true;
}

// POLYMORPHISM: overrides Screen::handleInput
void GameScreen::handleInput(const sf::Event& event) {
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape && keyEscReleased) {
            keyEscReleased = false;
            levelGenerated = false; // Force regeneration on re-entry
            game.switchScreen(SCREEN_WELCOME);
        }
        if (keyEvent->code == sf::Keyboard::Key::Right) {
            rightHeld = true;
        }
        if (keyEvent->code == sf::Keyboard::Key::Left) {
            leftHeld = true;
        }
    }
    if (const auto* keyRel = event.getIf<sf::Event::KeyReleased>()) {
        if (keyRel->code == sf::Keyboard::Key::Escape) keyEscReleased = true;
        if (keyRel->code == sf::Keyboard::Key::Right) rightHeld = false;
        if (keyRel->code == sf::Keyboard::Key::Left) leftHeld = false;
    }
}

// POLYMORPHISM: overrides Screen::update
void GameScreen::update(float deltaTime) {
    // Check if we need to generate/regenerate the level
    int mode = game.getGameMode();
    int level = game.getSelectedLevel();
    if (!levelGenerated || mode != lastGameMode || level != lastSelectedLevel) {
        generateLevel();
    }
    // Scroll camera with arrow keys
    if (rightHeld) {
        cameraX += scrollSpeed * deltaTime;
    }
    if (leftHeld) {
        cameraX -= scrollSpeed * deltaTime;
    }
    // Clamp camera
    if (cameraX < 0.0f) cameraX = 0.0f;
    if (currentLevel != nullptr) {
        float maxCam = currentLevel->getMaxCameraX();
        if (maxCam < 0.0f) maxCam = 0.0f;
        if (cameraX > maxCam) cameraX = maxCam;
    }
    // Update HUD info text
    char infoStr[100];
    int pos = 0;
    if (mode == MODE_SURVIVAL) {
        const char* prefix = "SURVIVAL - Level ";
        for (int i = 0; prefix[i] != '\0'; i++) infoStr[pos++] = prefix[i];
        int lv = game.getSelectedLevel();
        if (lv == 4) {
            const char* b = "BOSS";
            for (int i = 0; b[i] != '\0'; i++) infoStr[pos++] = b[i];
        } else {
            infoStr[pos++] = '0' + lv;
        }
    } else {
        const char* prefix = "CAMPAIGN MODE";
        for (int i = 0; prefix[i] != '\0'; i++) infoStr[pos++] = prefix[i];
    }
    // Append player name
    const char* appendSep = "  |  ";
    for (int i = 0; appendSep[i] != '\0'; i++) infoStr[pos++] = appendSep[i];
    const char* name = game.getPlayerName();
    for (int i = 0; name[i] != '\0' && i < 20; i++) infoStr[pos++] = name[i];
    infoStr[pos] = '\0';
    infoText->setString(infoStr);
    // Update biome text
    if (currentLevel != nullptr) {
        const char* biomeName = currentLevel->getCurrentBiomeName(cameraX);
        char biomeStr[60];
        int bp = 0;
        const char* bprefix = "Biome: ";
        for (int i = 0; bprefix[i] != '\0'; i++) biomeStr[bp++] = bprefix[i];
        for (int i = 0; biomeName[i] != '\0'; i++) biomeStr[bp++] = biomeName[i];
        biomeStr[bp] = '\0';
        biomeText->setString(biomeStr);
        // Color biome text based on type
        if (biomeName[0] == 'P') biomeText->setFillColor(sf::Color(34, 200, 34));
        else if (biomeName[0] == 'A' && biomeName[1] == 'e') biomeText->setFillColor(sf::Color(200, 200, 255));
        else if (biomeName[0] == 'A' && biomeName[1] == 'q') biomeText->setFillColor(sf::Color(50, 150, 255));
        else biomeText->setFillColor(sf::Color(200, 200, 200));
    }
    // Update position text
    int colPos = (int)(cameraX / BLOCK_SIZE);
    char posStr[40];
    int pp = 0;
    const char* pprefix = "Block: ";
    for (int i = 0; pprefix[i] != '\0'; i++) posStr[pp++] = pprefix[i];
    // Convert colPos to string (simple int to string)
    if (colPos == 0) {
        posStr[pp++] = '0';
    } else {
        char digits[10];
        int nd = 0;
        int tmp = colPos;
        while (tmp > 0) {
            digits[nd++] = '0' + (tmp % 10);
            tmp /= 10;
        }
        for (int i = nd - 1; i >= 0; i--) {
            posStr[pp++] = digits[i];
        }
    }
    // Append level width
    const char* ofStr = " / ";
    for (int i = 0; ofStr[i] != '\0'; i++) posStr[pp++] = ofStr[i];
    if (currentLevel != nullptr) {
        int w = currentLevel->getWidth();
        char wdigits[10];
        int wnd = 0;
        while (w > 0) {
            wdigits[wnd++] = '0' + (w % 10);
            w /= 10;
        }
        for (int i = wnd - 1; i >= 0; i--) {
            posStr[pp++] = wdigits[i];
        }
    }
    posStr[pp] = '\0';
    positionText->setString(posStr);
}

// POLYMORPHISM: overrides Screen::render
void GameScreen::render(sf::RenderWindow& window) {
    // Render the level (background + blocks)
    if (currentLevel != nullptr) {
        currentLevel->render(window, cameraX);
    }
    // Draw HUD background strip
    sf::RectangleShape hudBg;
    hudBg.setSize({300.0f, 75.0f});
    hudBg.setPosition({5.0f, 5.0f});
    hudBg.setFillColor(sf::Color(0, 0, 0, 160));
    hudBg.setOutlineColor(sf::Color(255, 200, 0, 100));
    hudBg.setOutlineThickness(1.0f);
    window.draw(hudBg);
    // Draw HUD text
    window.draw(*infoText);
    window.draw(*biomeText);
    window.draw(*positionText);
    // Draw bottom bar
    sf::RectangleShape bottomBar;
    bottomBar.setSize({350.0f, 20.0f});
    bottomBar.setPosition({5.0f, 577.0f});
    bottomBar.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(bottomBar);
    window.draw(*backText);
}