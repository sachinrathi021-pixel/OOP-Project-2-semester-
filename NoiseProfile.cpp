// NoiseProfile.cpp - Noise profile implementations
// Heights now target range [2, 13] for visible screen area (19 rows)
#include "NoiseProfile.h"

// --- Base class ---
NoiseProfile::~NoiseProfile() {}

int NoiseProfile::getOctaves() const { return 4; }
float NoiseProfile::getPersistence() const { return 0.5f; }
float NoiseProfile::getLacunarity() const { return 2.0f; }

// --- NormalProfile ---
int NormalProfile::getSurfaceHeight(PerlinNoise& noise, int column) const {
    float freq = 0.035f;
    float n = noise.fractalNoise(column * freq, getOctaves(), getPersistence(), getLacunarity());
    // Map [-1,1] to [3, 10] — moderate terrain
    int height = 6 + (int)(n * 4.0f);
    if (height < 2) height = 2;
    if (height > 13) height = 13;
    return height;
}

const char* NormalProfile::getProfileName() const {
    return "Normal";
}

// --- AmplifiedProfile ---
int AmplifiedProfile::getSurfaceHeight(PerlinNoise& noise, int column) const {
    float freq = 0.03f;
    float n = noise.fractalNoise(column * freq, getOctaves(), getPersistence(), getLacunarity());
    // Amplified: [2, 13] — extreme variation
    int height = 7 + (int)(n * 6.0f);
    if (height < 2) height = 2;
    if (height > 13) height = 13;
    return height;
}

const char* AmplifiedProfile::getProfileName() const {
    return "Amplified";
}

int AmplifiedProfile::getOctaves() const { return 6; }

// --- FlatProfile ---
int FlatProfile::getSurfaceHeight(PerlinNoise& noise, int column) const {
    float freq = 0.04f;
    float n = noise.fractalNoise(column * freq, getOctaves(), getPersistence(), getLacunarity());
    // Flat: [4, 8] — very gentle
    int height = 6 + (int)(n * 2.0f);
    if (height < 4) height = 4;
    if (height > 8) height = 8;
    return height;
}

const char* FlatProfile::getProfileName() const {
    return "Flat";
}

float FlatProfile::getPersistence() const { return 0.3f; }
