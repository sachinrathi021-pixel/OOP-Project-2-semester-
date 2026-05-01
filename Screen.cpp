// Screen.cpp - Abstract base class implementation
#include "Screen.h"
// Protected constructor - stores reference to Game (ASSOCIATION)
Screen::Screen(Game& gameRef) : game(gameRef) {
}
// Virtual destructor
Screen::~Screen() {
}
