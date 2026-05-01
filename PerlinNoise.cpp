// PerlinNoise.cpp - 1D Perlin Noise from scratch
// Implements Ken Perlin's gradient noise algorithm
#include "PerlinNoise.h"

// Default permutation table (Ken Perlin's original)
static const int DEFAULT_PERM[256] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,
    140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,
    247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,
    57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
    74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,
    60,211,133,230,220,105,92,41,55,46,245,40,244,102,143,54,
    65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,
    200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,
    52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,
    207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,
    119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,
    129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,
    218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,
    81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,
    4,184,204,176,115,121,50,45,127,4,150,254,138,236,205,93,
    222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

PerlinNoise::PerlinNoise(int seed) : seed(seed) {
    setSeed(seed);
}

PerlinNoise::~PerlinNoise() {
}

void PerlinNoise::setSeed(int newSeed) {
    seed = newSeed;
    // Fill permutation table with default values
    for (int i = 0; i < 256; i++) {
        perm[i] = DEFAULT_PERM[i];
    }
    // Shuffle based on seed using a simple LCG-style approach
    if (newSeed != 0) {
        unsigned int s = (unsigned int)newSeed;
        for (int i = 255; i > 0; i--) {
            s = s * 1103515245 + 12345; // LCG
            int j = (int)((s >> 16) & 0x7FFF) % (i + 1);
            // Swap
            int tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }
    }
    // Duplicate the table for overflow safety
    for (int i = 0; i < 256; i++) {
        perm[256 + i] = perm[i];
    }
}

float PerlinNoise::fade(float t) const {
    // 6t^5 - 15t^4 + 10t^3 (improved Perlin fade)
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float a, float b, float t) const {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x) const {
    // Simple 1D gradient: hash determines direction
    return (hash & 1) == 0 ? x : -x;
}

float PerlinNoise::noise(float x) const {
    // Find unit interval containing x
    int xi = (int)x;
    if (x < 0) xi = xi - 1; // Handle negative values
    xi = xi & 255; // Wrap to 0-255
    // Fractional part
    float xf = x - (int)x;
    if (x < 0) xf = x - ((int)x - 1);
    // Fade curve
    float u = fade(xf);
    // Hash the two integer endpoints
    int h0 = perm[xi];
    int h1 = perm[xi + 1];
    // Gradient and interpolate
    float g0 = grad(h0, xf);
    float g1 = grad(h1, xf - 1.0f);
    return lerp(g0, g1, u);
}

float PerlinNoise::fractalNoise(float x, int octaves, float persistence, float lacunarity) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f; // For normalization
    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    // Normalize to [-1, 1]
    if (maxValue > 0.0f) {
        total /= maxValue;
    }
    return total;
}
