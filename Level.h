#pragma once
// Level.h - Level with terrain grid, biome regions, platforms
// COMPOSITION (owns block grid), AGGREGATION (holds Biome/NoiseProfile pointers)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include "PerlinNoise.h"
#include "NoiseProfile.h"
#include "Biome.h"

// Block size in pixels
const int BLOCK_SIZE = 32;

// Level dimensions
// Visible area: 800/32=25 cols, 600/32=18.75 rows -> 19 visible rows
const int VISIBLE_ROWS = 19;
const int MAX_LEVEL_HEIGHT = 20; // Slight buffer above visible

// Survival: 500 blocks wide (170 per biome)
const int SURVIVAL_WIDTH = 500;

// Campaign: sliding window of 200 columns (generates infinitely)
const int CAMPAIGN_WINDOW = 200;

// Max array size (fits both modes)
const int MAX_LEVEL_WIDTH = 500;

// Background and block texture counts
const int NUM_BACKGROUNDS = 9;
const int NUM_BLOCK_TEXTURES = 6;

// Max biome regions
const int MAX_BIOME_REGIONS = 4;

// Max floating platforms
const int MAX_PLATFORMS = 200;

struct Platform {
    int x, y;     // Block position
    int width;     // Width in blocks
    int blockType; // Block type to use
    bool active;
};

class Level {
private:
    // COMPOSITION: Level owns the block grid
    int blocks[MAX_LEVEL_WIDTH][MAX_LEVEL_HEIGHT];
    int biomeMap[MAX_LEVEL_WIDTH]; // Biome type per column
    int surfaceHeights[MAX_LEVEL_WIDTH]; // Surface height per column
    int levelWidth;
    int levelHeight;
    // Biome regions (AGGREGATION)
    Biome* biomes[MAX_BIOME_REGIONS];
    int biomeStartCol[MAX_BIOME_REGIONS];
    int numBiomeRegions;
    // Noise engine
    PerlinNoise perlin;
    NoiseProfile* profile;
    bool ownsProfile; // Whether we need to delete the profile
    // Textures
    sf::Texture blockTextures[NUM_BLOCK_TEXTURES];
    sf::Texture bgTextures[NUM_BACKGROUNDS];
    bool blockTexturesLoaded[NUM_BLOCK_TEXTURES];
    bool bgTexturesLoaded[NUM_BACKGROUNDS];
    // Campaign infinite scrolling
    bool isCampaign;
    int campaignOffset;    // Global column offset for the left edge of the window
    int campaignGenRight;  // Rightmost globally generated column
    // Platforms for climbing
    Platform platforms[MAX_PLATFORMS];
    int numPlatforms;
    // Internal methods
    void loadTextures();
    void generateColumn(int localCol, int globalCol);
    void addSteppingBlocks();
    void addFloatingPlatforms();
    void addAquaticIslands();
    Biome* getBiomeForColumn(int globalCol);
    int getBiomeTypeForColumn(int globalCol);
public:
    Level();
    ~Level();
    // Generate survival level (300 blocks wide)
    void generateSurvival(int seed, int levelNum);
    // Generate boss level (300 blocks, 4 phases)
    void generateBoss(int seed);
    // Start campaign (infinite generation)
    void generateCampaign(int seed, NoiseProfile* noiseProfile);
    // Extend campaign as camera moves
    void extendCampaign(float cameraX);
    // Render
    void render(sf::RenderWindow& window, float cameraX);
    // Info
    const char* getCurrentBiomeName(float cameraX) const;
    int getWidth() const;
    int getHeight() const;
    bool isCampaignMode() const;
    float getMaxCameraX() const;
};
