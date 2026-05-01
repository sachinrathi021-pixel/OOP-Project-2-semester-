#pragma once
// Biome.h - Abstract base class for biome types
// Demonstrates ABSTRACTION + INHERITANCE
#include "NoiseProfile.h"

class Biome {
public:
    virtual ~Biome();
    // Pure virtual: what block type at a given row given the surface height
    // row 0 = bottom, row increases upward
    virtual int getBlockType(int row, int surfaceHeight) const = 0;
    // Pure virtual: returns the biome name for HUD display
    virtual const char* getBiomeName() const = 0;
    // Pure virtual: returns how the noise maps to surface height for this biome
    // baseHeight is from the noise profile, biome adjusts it
    virtual int adjustHeight(int baseHeight) const = 0;
    // Virtual: returns the background image index (0-8 mapping to 1.png-9.png)
    // Can cycle through multiple backgrounds
    virtual int getBackgroundIndex(int section) const = 0;
    // Does this biome have water?
    virtual bool hasWater() const;
    // Get the sea level for this biome
    virtual int getSeaLevel() const;
};
