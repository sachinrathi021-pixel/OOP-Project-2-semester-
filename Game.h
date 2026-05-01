#pragma once
// Game.h - Main game class
// Demonstrates COMPOSITION (owns AudioManager) and AGGREGATION (holds Screen pointers)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include "AudioManager.h"
// Forward declaration
class Screen;
// Screen indices
const int SCREEN_LANDING = 0;
const int SCREEN_WELCOME = 1;
const int SCREEN_LEVEL_SELECT = 2;
const int SCREEN_GAME = 3;
const int MAX_SCREENS = 4;
// Game modes
const int MODE_NONE = 0;
const int MODE_SURVIVAL = 1;
const int MODE_CAMPAIGN = 2;
class Game {
private:
    // ENCAPSULATION: private data members
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    // AGGREGATION: Game holds Screen pointers (screens could exist independently)
    Screen* screens[MAX_SCREENS];
    int currentScreen;
    // COMPOSITION: Game owns AudioManager (AudioManager dies when Game dies)
    AudioManager audioManager;
    // Player data
    char playerName[50];
    int nameLength;
    int gameMode;
    int selectedLevel;
public:
    Game();
    ~Game();
    // Main game loop
    void run();
    // Screen management
    void switchScreen(int screenIndex);
    int getCurrentScreen() const;
    // Player data access (ENCAPSULATION via getters/setters)
    void setPlayerName(const char* name, int length);
    const char* getPlayerName() const;
    int getPlayerNameLength() const;
    void setGameMode(int mode);
    int getGameMode() const;
    void setSelectedLevel(int level);
    int getSelectedLevel() const;
    // Shared resource access
    sf::Font& getFont();
    sf::RenderWindow& getWindow();
    // COMPOSITION access: AudioManager is part of Game
    AudioManager& getAudioManager();
};