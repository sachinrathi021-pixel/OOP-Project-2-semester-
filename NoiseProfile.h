#pragma once
// NoiseProfile.h - Abstract noise profile + 3 concrete profiles
// Demonstrates ABSTRACTION + INHERITANCE + POLYMORPHISM
// Used as a factory pattern for terrain generation profiles
#include "PerlinNoise.h"

// Block type constants
const int BLOCK_AIR = 0;
const int BLOCK_GRASS = 1;
const int BLOCK_DIRT = 2;
const int BLOCK_STONE = 3;
const int BLOCK_ANDESITE = 4;
const int BLOCK_BEDROCK = 5;
const int BLOCK_WATER = 6;

// Biome type constants
const int BIOME_PLAINS = 0;
const int BIOME_AERIAL = 1;
const int BIOME_AQUATIC = 2;

// Sea level (row index from bottom, 0 = very bottom)
// With 19 visible rows, sea level at 14 gives ~65-70% water coverage
const int SEA_LEVEL = 14;

// Abstract class (ABSTRACTION)
class NoiseProfile {
public:
    virtual ~NoiseProfile();
    // Pure virtual: returns surface height (0-39) for a given column
    // POLYMORPHISM: each profile computes height differently
    virtual int getSurfaceHeight(PerlinNoise& noise, int column) const = 0;
    // Pure virtual: returns the profile name
    virtual const char* getProfileName() const = 0;
    // Returns number of octaves for fractal noise
    virtual int getOctaves() const;
    // Returns persistence for fractal noise
    virtual float getPersistence() const;
    // Returns lacunarity for fractal noise
    virtual float getLacunarity() const;
};

// Normal profile: balanced terrain (INHERITANCE)
class NormalProfile : public NoiseProfile {
public:
    int getSurfaceHeight(PerlinNoise& noise, int column) const override;
    const char* getProfileName() const override;
};

// Amplified profile: extreme peaks and deep oceans (INHERITANCE)
class AmplifiedProfile : public NoiseProfile {
public:
    int getSurfaceHeight(PerlinNoise& noise, int column) const override;
    const char* getProfileName() const override;
    int getOctaves() const override;
};

// Flat profile: gentle terrain with minimal variation (INHERITANCE)
class FlatProfile : public NoiseProfile {
public:
    int getSurfaceHeight(PerlinNoise& noise, int column) const override;
    const char* getProfileName() const override;
    float getPersistence() const override;
};
