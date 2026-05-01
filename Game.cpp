// Game.cpp - Main game class implementation
// Demonstrates COMPOSITION, AGGREGATION, and ASSOCIATION
#include "Game.h"
#include "Screen.h"
#include "LandingScreen.h"
#include "WelcomeScreen.h"
#include "LevelSelectScreen.h"
#include "GameScreen.h"
#include <iostream>
Game::Game()
    : window(sf::VideoMode({800u, 600u}), "Metal Slug"),
      currentScreen(SCREEN_LANDING),
      nameLength(0),
      gameMode(MODE_NONE),
      selectedLevel(0)
{
    // Initialize player name to empty
    for (int i = 0; i < 50; i++) {
        playerName[i] = '\0';
    }
    // Load shared font resource
    if (!font.openFromFile("Fonts/arial.ttf")) {
        std::cout << "Error: Could not load font file!" << std::endl;
    }
    // AGGREGATION: Create screen objects (Game holds but doesn't strictly own their lifecycle concept)
    screens[SCREEN_LANDING] = new LandingScreen(*this);
    screens[SCREEN_WELCOME] = new WelcomeScreen(*this);
    screens[SCREEN_LEVEL_SELECT] = new LevelSelectScreen(*this);
    screens[SCREEN_GAME] = new GameScreen(*this);
    // COMPOSITION: AudioManager is created as part of Game automatically
    // Try to play background music (will gracefully fail if file not found)
    audioManager.playMusic("music.mp3");
    window.setFramerateLimit(60);
}
Game::~Game() {
    // Clean up AGGREGATED screens
    for (int i = 0; i < MAX_SCREENS; i++) {
        delete screens[i];
        screens[i] = nullptr;
    }
    // AudioManager (COMPOSITION) is destroyed automatically with Game
}
void Game::run() {
    while (window.isOpen()) {
        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        // Process events - SFML 3 style
        while (const auto event = window.pollEvent()) {
            // Window close event
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            // POLYMORPHISM: calling virtual handleInput through base pointer
            if (currentScreen >= 0 && currentScreen < MAX_SCREENS) {
                screens[currentScreen]->handleInput(*event);
            }
        }
        // POLYMORPHISM: calling virtual update through base pointer
        if (currentScreen >= 0 && currentScreen < MAX_SCREENS) {
            screens[currentScreen]->update(deltaTime);
        }
        // Render
        window.clear(sf::Color::Black);
        // POLYMORPHISM: calling virtual render through base pointer
        if (currentScreen >= 0 && currentScreen < MAX_SCREENS) {
            screens[currentScreen]->render(window);
        }
        window.display();
    }
}
void Game::switchScreen(int screenIndex) {
    if (screenIndex >= 0 && screenIndex < MAX_SCREENS) {
        currentScreen = screenIndex;
    }
}
int Game::getCurrentScreen() const {
    return currentScreen;
}
void Game::setPlayerName(const char* name, int length) {
    nameLength = 0;
    for (int i = 0; i < length && i < 49; i++) {
        playerName[i] = name[i];
        nameLength++;
    }
    playerName[nameLength] = '\0';
}
const char* Game::getPlayerName() const {
    return playerName;
}
int Game::getPlayerNameLength() const {
    return nameLength;
}
void Game::setGameMode(int mode) {
    gameMode = mode;
}
int Game::getGameMode() const {
    return gameMode;
}
void Game::setSelectedLevel(int level) {
    selectedLevel = level;
}
int Game::getSelectedLevel() const {
    return selectedLevel;
}
sf::Font& Game::getFont() {
    return font;
}
sf::RenderWindow& Game::getWindow() {
    return window;
}
AudioManager& Game::getAudioManager() {
    return audioManager;
}