#pragma once
// Screen.h - Abstract base class for all game screens (Abstraction)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
// Forward declaration
class Game;
// Abstract base class demonstrating ABSTRACTION
// All screen types inherit from this (INHERITANCE base)
class Screen {
protected:
    Game& game; // ASSOCIATION: Screen knows about Game but doesn't own it
    // Protected constructor - only derived classes can be instantiated
    Screen(Game& gameRef);
public:
    // Virtual destructor for proper cleanup through base pointer
    virtual ~Screen();
    // Pure virtual functions - ABSTRACTION
    // Derived classes MUST implement these (POLYMORPHISM)
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
