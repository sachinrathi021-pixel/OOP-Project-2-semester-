#pragma once
// PerlinNoise.h - 1D Perlin Noise implementation from scratch
// Used for procedural terrain generation (height maps)
// Reference: Ken Perlin's original noise algorithm

class PerlinNoise {
private:
    // Permutation table - fixed size C-array (no vectors per constraint)
    int perm[512];
    int seed;
    // Helper: fade function (smoothstep) for interpolation
    float fade(float t) const;
    // Helper: linear interpolation
    float lerp(float a, float b, float t) const;
    // Helper: gradient function
    float grad(int hash, float x) const;
public:
    // Constructor with seed for reproducible terrain
    PerlinNoise(int seed = 0);
    ~PerlinNoise();
    // Set a new seed and regenerate permutation table
    void setSeed(int newSeed);
    // Core 1D Perlin noise - returns value in range [-1, 1]
    float noise(float x) const;
    // Fractal noise (multiple octaves stacked)
    // octaves: number of layers (more = more detail)
    // persistence: amplitude decay per octave (typically 0.5)
    // lacunarity: frequency multiplier per octave (typically 2.0)
    float fractalNoise(float x, int octaves, float persistence, float lacunarity) const;
};
