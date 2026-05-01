// Level.cpp - Terrain generation with creative biome design
#include "Level.h"
#include "PlainsBiome.h"
#include "AerialBiome.h"
#include "AquaticBiome.h"
#include <cmath>
#include <iostream>

static unsigned int levelRand(unsigned int& seed) {
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}

Level::Level()
    : levelWidth(SURVIVAL_WIDTH), levelHeight(MAX_LEVEL_HEIGHT),
      numBiomeRegions(0), profile(nullptr), ownsProfile(false),
      isCampaign(false), campaignOffset(0), campaignGenRight(0), numPlatforms(0)
{
    for (int x = 0; x < MAX_LEVEL_WIDTH; x++) {
        for (int y = 0; y < MAX_LEVEL_HEIGHT; y++) blocks[x][y] = BLOCK_AIR;
        biomeMap[x] = BIOME_PLAINS;
        surfaceHeights[x] = 2;
    }
    for (int i = 0; i < MAX_BIOME_REGIONS; i++) { biomes[i] = nullptr; biomeStartCol[i] = 0; }
    for (int i = 0; i < MAX_PLATFORMS; i++) platforms[i].active = false;
    for (int i = 0; i < NUM_BLOCK_TEXTURES; i++) blockTexturesLoaded[i] = false;
    for (int i = 0; i < NUM_BACKGROUNDS; i++) bgTexturesLoaded[i] = false;
    loadTextures();
}

Level::~Level() {
    for (int i = 0; i < MAX_BIOME_REGIONS; i++) { delete biomes[i]; biomes[i] = nullptr; }
    if (ownsProfile) delete profile;
}

void Level::loadTextures() {
    const char* bf[] = {"","Sprites/blocks/grass_block_side.png","Sprites/blocks/dirt.png",
        "Sprites/blocks/stone.png","Sprites/blocks/andesite.png","Sprites/blocks/deepslate_top.png"};
    for (int i = 1; i < NUM_BLOCK_TEXTURES; i++)
        if (blockTextures[i].loadFromFile(bf[i])) blockTexturesLoaded[i] = true;
    const char* bg[] = {"Sprites/Background/1.png","Sprites/Background/2.png","Sprites/Background/3.png",
        "Sprites/Background/4.png","Sprites/Background/5.png","Sprites/Background/6.png",
        "Sprites/Background/7.png","Sprites/Background/8.png","Sprites/Background/9.png"};
    for (int i = 0; i < NUM_BACKGROUNDS; i++)
        if (bgTextures[i].loadFromFile(bg[i])) bgTexturesLoaded[i] = true;
}

Biome* Level::getBiomeForColumn(int globalCol) {
    if (isCampaign) {
        // Campaign: biome changes every 130 columns, cycling Plains->Aerial->Aquatic
        int cycle = (globalCol / 130) % 3;
        return biomes[cycle];
    }
    int idx = 0;
    for (int i = numBiomeRegions - 1; i >= 0; i--)
        if (globalCol >= biomeStartCol[i]) { idx = i; break; }
    return biomes[idx];
}

int Level::getBiomeTypeForColumn(int globalCol) {
    if (isCampaign) return (globalCol / 130) % 3;
    Biome* b = getBiomeForColumn(globalCol);
    if (!b) return BIOME_PLAINS;
    const char* n = b->getBiomeName();
    if (n[0] == 'A' && n[1] == 'e') return BIOME_AERIAL;
    if (n[0] == 'A' && n[1] == 'q') return BIOME_AQUATIC;
    return BIOME_PLAINS;
}

void Level::generateColumn(int localCol, int globalCol) {
    if (localCol < 0 || localCol >= MAX_LEVEL_WIDTH) return;
    Biome* biome = getBiomeForColumn(globalCol);
    if (!biome) return;
    int biomeType = getBiomeTypeForColumn(globalCol);
    int surfH = 2;

    if (biomeType == BIOME_AERIAL) {
        // Tall jagged mountains with guaranteed peaks every ~35 columns
        // Sin wave creates reliable peak placement
        float pi = 3.14159f;
        float peak = sinf(globalCol * pi / 35.0f);
        peak = peak < 0 ? -peak : peak; // abs -> peaks every ~35 cols
        peak = peak * peak; // Sharpen peaks (more mountain-like)
        // Noise for jagged variation
        float jag = perlin.noise(globalCol * 0.15f) * 0.15f + 0.85f;
        // Base=2, peaks up to 17 (nearly full screen)
        surfH = 2 + (int)(peak * 15.0f * jag);
        // Add small random variation
        float micro = perlin.noise(globalCol * 0.4f);
        surfH += (int)(micro * 1.5f);
        if (surfH < 2) surfH = 2;
        if (surfH > 17) surfH = 17;
    } else if (biomeType == BIOME_AQUATIC) {
        // Very low ground — water fills above to SEA_LEVEL(14)
        float n = perlin.fractalNoise(globalCol * 0.04f, 2, 0.5f, 2.0f);
        surfH = 1 + (int)((n + 1.0f) * 0.5f);
        if (surfH < 1) surfH = 1;
        if (surfH > 2) surfH = 2;
    } else {
        // Plains: visible hills every ~37 columns, 2-3 blocks above flat ground
        float pi = 3.14159f;
        float hill = sinf(globalCol * pi / 37.0f);
        hill = hill < 0 ? -hill : hill; // abs -> hill every ~37 cols
        // Noise for natural feel
        float variation = perlin.noise(globalCol * 0.08f) * 0.3f + 0.7f;
        surfH = 2 + (int)(hill * 3.5f * variation);
        if (surfH < 2) surfH = 2;
        if (surfH > 5) surfH = 5;
    }

    surfaceHeights[localCol] = surfH;
    biomeMap[localCol] = biomeType;
    for (int y = 0; y < levelHeight; y++)
        blocks[localCol][y] = biome->getBlockType(y, surfH);
}

void Level::addSteppingBlocks() {
    int width = isCampaign ? CAMPAIGN_WINDOW : levelWidth;
    if (width > MAX_LEVEL_WIDTH) width = MAX_LEVEL_WIDTH;
    for (int x = 1; x < width - 1; x++) {
        int h = surfaceHeights[x];
        int hNext = (x + 1 < width) ? surfaceHeights[x + 1] : h;
        int hPrev = surfaceHeights[x - 1];
        // Add stepping blocks when rise is >= 2
        if (hNext - h >= 2) {
            for (int s = 1; s < hNext - h; s++) {
                int sy = h + s;
                if (sy < levelHeight && blocks[x][sy] == BLOCK_AIR)
                    blocks[x][sy] = BLOCK_STONE;
            }
        }
        if (hPrev - h >= 2) {
            for (int s = 1; s < hPrev - h; s++) {
                int sy = h + s;
                if (sy < levelHeight && blocks[x][sy] == BLOCK_AIR)
                    blocks[x][sy] = BLOCK_STONE;
            }
        }
    }
}

void Level::addFloatingPlatforms() {
    int width = isCampaign ? CAMPAIGN_WINDOW : levelWidth;
    if (width > MAX_LEVEL_WIDTH) width = MAX_LEVEL_WIDTH;
    unsigned int seed = 54321;
    numPlatforms = 0;
    for (int x = 3; x < width - 5; x += 4) {
        if (biomeMap[x] != BIOME_AERIAL) continue;
        unsigned int r = levelRand(seed);
        if ((r % 100) < 35 && numPlatforms < MAX_PLATFORMS) {
            int surfH = surfaceHeights[x];
            int platH = surfH + 2 + (int)(levelRand(seed) % 3);
            if (platH >= levelHeight - 1) continue;
            int platW = 2 + (int)(levelRand(seed) % 2);
            for (int px = 0; px < platW && (x + px) < width; px++)
                if (blocks[x + px][platH] == BLOCK_AIR)
                    blocks[x + px][platH] = BLOCK_ANDESITE;
            platforms[numPlatforms].x = x; platforms[numPlatforms].y = platH;
            platforms[numPlatforms].width = platW; platforms[numPlatforms].active = true;
            numPlatforms++;
        }
    }
}

void Level::addAquaticIslands() {
    // Build structured aquatic pools:
    // Entry wall -> 28-col water pool -> exit stairs -> repeat
    int width = isCampaign ? CAMPAIGN_WINDOW : levelWidth;
    if (width > MAX_LEVEL_WIDTH) width = MAX_LEVEL_WIDTH;
    // Find aquatic biome boundaries
    int aquaStart = -1, aquaEnd = -1;
    for (int x = 0; x < width; x++) {
        if (biomeMap[x] == BIOME_AQUATIC && aquaStart < 0) aquaStart = x;
        if (biomeMap[x] == BIOME_AQUATIC) aquaEnd = x;
    }
    if (aquaStart < 0) return;
    int wallH = SEA_LEVEL + 2; // Walls go above water
    if (wallH >= levelHeight) wallH = levelHeight - 1;
    int x = aquaStart;
    while (x < aquaEnd - 5) {
        // --- ENTRY WALL (3 wide) ---
        for (int wx = x; wx < x + 3 && wx < width; wx++) {
            for (int y = 0; y <= wallH; y++) {
                if (y == 0) blocks[wx][y] = BLOCK_BEDROCK;
                else if (y == wallH) blocks[wx][y] = BLOCK_GRASS;
                else blocks[wx][y] = BLOCK_STONE;
            }
            for (int y = wallH + 1; y < levelHeight; y++) blocks[wx][y] = BLOCK_AIR;
            surfaceHeights[wx] = wallH;
        }
        x += 3;
        // --- WATER POOL (28 wide) ---
        int poolEnd = x + 28;
        if (poolEnd > aquaEnd - 8) poolEnd = aquaEnd - 8;
        for (int px = x; px < poolEnd && px < width; px++) {
            int ground = 1;
            blocks[px][0] = BLOCK_BEDROCK;
            blocks[px][1] = BLOCK_DIRT;
            surfaceHeights[px] = ground;
            for (int y = ground + 1; y <= SEA_LEVEL; y++)
                blocks[px][y] = BLOCK_WATER;
            for (int y = SEA_LEVEL + 1; y < levelHeight; y++)
                blocks[px][y] = BLOCK_AIR;
        }
        x = poolEnd;
        if (x >= aquaEnd - 5) break;
        // --- EXIT STAIRS (5 wide, climbing out of water) ---
        for (int s = 0; s < 5 && x + s < width; s++) {
            int stairH = 2 + s * 3; // Step up: 2, 5, 8, 11, 14
            if (stairH > wallH) stairH = wallH;
            for (int y = 0; y <= stairH; y++) {
                if (y == 0) blocks[x + s][y] = BLOCK_BEDROCK;
                else if (y == stairH) blocks[x + s][y] = BLOCK_GRASS;
                else if (y >= stairH - 1) blocks[x + s][y] = BLOCK_DIRT;
                else blocks[x + s][y] = BLOCK_STONE;
            }
            // Water below stair height up to sea level
            for (int y = stairH + 1; y <= SEA_LEVEL; y++)
                if (blocks[x + s][y] == BLOCK_AIR) blocks[x + s][y] = BLOCK_WATER;
            for (int y = SEA_LEVEL + 1; y < levelHeight; y++)
                blocks[x + s][y] = BLOCK_AIR;
            surfaceHeights[x + s] = stairH;
        }
        x += 5;
    }
}

void Level::generateSurvival(int seed, int levelNum) {
    isCampaign = false;
    levelWidth = SURVIVAL_WIDTH;
    perlin.setSeed(seed);
    if (ownsProfile) delete profile;
    profile = new NormalProfile(); ownsProfile = true;
    for (int i = 0; i < MAX_BIOME_REGIONS; i++) { delete biomes[i]; biomes[i] = nullptr; }
    // 500 blocks: each biome gets ~170 columns
    biomes[0] = new PlainsBiome();  biomeStartCol[0] = 0;
    biomes[1] = new AerialBiome();  biomeStartCol[1] = 170;
    biomes[2] = new AquaticBiome(); biomeStartCol[2] = 340;
    numBiomeRegions = 3;
    for (int x = 0; x < levelWidth; x++) generateColumn(x, x);
    // Smooth transitions (10 col blend at biome boundaries)
    for (int x = 160; x < 180 && x < levelWidth; x++) {
        float t = (float)(x - 160) / 20.0f;
        int h1 = surfaceHeights[160]; int h2 = surfaceHeights[180 < levelWidth ? 180 : levelWidth-1];
        int blendH = h1 + (int)((h2 - h1) * t);
        if (blendH < 2) blendH = 2;
        surfaceHeights[x] = blendH;
        Biome* b = (t < 0.5f) ? biomes[0] : biomes[1];
        for (int y = 0; y < levelHeight; y++) blocks[x][y] = b->getBlockType(y, blendH);
    }
    for (int x = 330; x < 350 && x < levelWidth; x++) {
        float t = (float)(x - 330) / 20.0f;
        int h1 = surfaceHeights[330]; int h2 = surfaceHeights[350 < levelWidth ? 350 : levelWidth-1];
        int blendH = h1 + (int)((h2 - h1) * t);
        if (blendH < 1) blendH = 1;
        surfaceHeights[x] = blendH;
        Biome* b = (t < 0.5f) ? biomes[1] : biomes[2];
        for (int y = 0; y < levelHeight; y++) blocks[x][y] = b->getBlockType(y, blendH);
        if (t > 0.3f && blendH < SEA_LEVEL) {
            for (int y = blendH + 1; y <= SEA_LEVEL && y < levelHeight; y++)
                if (blocks[x][y] == BLOCK_AIR) blocks[x][y] = BLOCK_WATER;
        }
    }
    addSteppingBlocks(); addFloatingPlatforms(); addAquaticIslands();
}

void Level::generateBoss(int seed) {
    isCampaign = false;
    levelWidth = SURVIVAL_WIDTH;
    perlin.setSeed(seed);
    if (ownsProfile) delete profile;
    profile = new NormalProfile(); ownsProfile = true;
    for (int i = 0; i < MAX_BIOME_REGIONS; i++) { delete biomes[i]; biomes[i] = nullptr; }
    biomes[0] = new PlainsBiome();  biomeStartCol[0] = 0;
    biomes[1] = new AerialBiome();  biomeStartCol[1] = 125;
    biomes[2] = new AquaticBiome(); biomeStartCol[2] = 250;
    biomes[3] = new PlainsBiome();  biomeStartCol[3] = 375;
    numBiomeRegions = 4;
    for (int x = 0; x < levelWidth; x++) generateColumn(x, x);
    addSteppingBlocks(); addFloatingPlatforms(); addAquaticIslands();
}

void Level::generateCampaign(int seed, NoiseProfile* noiseProfile) {
    isCampaign = true;
    levelWidth = CAMPAIGN_WINDOW;
    perlin.setSeed(seed);
    profile = noiseProfile; ownsProfile = false;
    for (int i = 0; i < MAX_BIOME_REGIONS; i++) { delete biomes[i]; biomes[i] = nullptr; }
    biomes[0] = new PlainsBiome();
    biomes[1] = new AerialBiome();
    biomes[2] = new AquaticBiome();
    numBiomeRegions = 3;
    campaignOffset = 0; campaignGenRight = 0;
    for (int x = 0; x < CAMPAIGN_WINDOW; x++) generateColumn(x, x);
    campaignGenRight = CAMPAIGN_WINDOW;
    addSteppingBlocks(); addFloatingPlatforms(); addAquaticIslands();
}

void Level::extendCampaign(float cameraX) {
    if (!isCampaign || !profile) return;
    int cameraCol = (int)(cameraX / BLOCK_SIZE);
    int threshold = campaignOffset + (CAMPAIGN_WINDOW * 3 / 5);
    if (cameraCol > threshold) {
        int shift = CAMPAIGN_WINDOW / 4;
        for (int x = 0; x < CAMPAIGN_WINDOW - shift; x++) {
            for (int y = 0; y < levelHeight; y++) blocks[x][y] = blocks[x + shift][y];
            biomeMap[x] = biomeMap[x + shift];
            surfaceHeights[x] = surfaceHeights[x + shift];
        }
        campaignOffset += shift;
        for (int x = CAMPAIGN_WINDOW - shift; x < CAMPAIGN_WINDOW; x++)
            generateColumn(x, campaignOffset + x);
        campaignGenRight = campaignOffset + CAMPAIGN_WINDOW;
        addSteppingBlocks(); addFloatingPlatforms(); addAquaticIslands();
    }
}

void Level::render(sf::RenderWindow& window, float cameraX) {
    if (isCampaign) extendCampaign(cameraX);
    int cameraCol = (int)(cameraX / BLOCK_SIZE);
    int centerCol = cameraCol + 12;
    int localCenter = isCampaign ? (centerCol - campaignOffset) : centerCol;
    int maxLocal = isCampaign ? CAMPAIGN_WINDOW : levelWidth;
    if (localCenter < 0) localCenter = 0;
    if (localCenter >= maxLocal) localCenter = maxLocal - 1;
    int curBiome = biomeMap[localCenter];
    // Background
    int bgSec = centerCol / 25;
    int bgIdx = 0;
    if (curBiome == BIOME_AERIAL && biomes[1]) bgIdx = biomes[1]->getBackgroundIndex(bgSec);
    else if (curBiome == BIOME_AQUATIC && biomes[2]) bgIdx = biomes[2]->getBackgroundIndex(bgSec);
    else if (biomes[0]) bgIdx = biomes[0]->getBackgroundIndex(bgSec);
    if (bgIdx >= 0 && bgIdx < NUM_BACKGROUNDS && bgTexturesLoaded[bgIdx]) {
        sf::Sprite bgSpr(bgTextures[bgIdx]);
        sf::Vector2u bs = bgTextures[bgIdx].getSize();
        float sc = 600.0f / bs.y;
        bgSpr.setScale({sc, sc});
        float px = -cameraX * 0.3f;
        float bw = bs.x * sc;
        while (px > 0) px -= bw;
        while (px < -bw) px += bw;
        for (float dx = px; dx < 800.0f; dx += bw) {
            bgSpr.setPosition({dx, 0.0f});
            window.draw(bgSpr);
        }
    }
    // Blocks
    int startV = (int)(cameraX / BLOCK_SIZE);
    int endV = startV + 26;
    for (int gx = startV; gx < endV; gx++) {
        int lx = isCampaign ? (gx - campaignOffset) : gx;
        if (lx < 0 || lx >= maxLocal) continue;
        float sx = (gx * BLOCK_SIZE) - cameraX;
        for (int y = 0; y < levelHeight; y++) {
            int bt = blocks[lx][y];
            if (bt == BLOCK_AIR) continue;
            float sy = 600.0f - (y + 1) * BLOCK_SIZE;
            if (sy < -BLOCK_SIZE || sy > 600.0f) continue;
            if (bt == BLOCK_WATER) {
                sf::RectangleShape wb;
                wb.setSize({(float)BLOCK_SIZE, (float)BLOCK_SIZE});
                wb.setPosition({sx, sy});
                int alpha = 90 + (y * 5); if (alpha > 170) alpha = 170;
                wb.setFillColor(sf::Color(20, 70, 180, alpha));
                wb.setOutlineColor(sf::Color(40, 120, 220, 40));
                wb.setOutlineThickness(0.5f);
                window.draw(wb);
            } else if (bt >= 1 && bt < NUM_BLOCK_TEXTURES && blockTexturesLoaded[bt]) {
                sf::Sprite bs(blockTextures[bt]);
                sf::Vector2u ts = blockTextures[bt].getSize();
                bs.setScale({(float)BLOCK_SIZE / ts.x, (float)BLOCK_SIZE / ts.y});
                bs.setPosition({sx, sy});
                window.draw(bs);
            } else {
                sf::RectangleShape fb;
                fb.setSize({(float)BLOCK_SIZE, (float)BLOCK_SIZE});
                fb.setPosition({sx, sy});
                sf::Color c = sf::Color(128, 128, 128);
                if (bt == BLOCK_GRASS) c = sf::Color(34, 139, 34);
                else if (bt == BLOCK_DIRT) c = sf::Color(139, 90, 43);
                else if (bt == BLOCK_ANDESITE) c = sf::Color(100, 100, 110);
                else if (bt == BLOCK_BEDROCK) c = sf::Color(40, 40, 40);
                fb.setFillColor(c); fb.setOutlineColor(sf::Color(0,0,0,50)); fb.setOutlineThickness(0.5f);
                window.draw(fb);
            }
        }
    }
}

const char* Level::getCurrentBiomeName(float cameraX) const {
    int col = (int)(cameraX / BLOCK_SIZE) + 12;
    int lc = isCampaign ? (col - campaignOffset) : col;
    if (lc < 0) lc = 0;
    int mx = isCampaign ? CAMPAIGN_WINDOW : levelWidth;
    if (lc >= mx) lc = mx - 1;
    int bt = biomeMap[lc];
    if (bt == BIOME_AERIAL) return "Aerial";
    if (bt == BIOME_AQUATIC) return "Aquatic";
    return "Plains";
}

int Level::getWidth() const { return levelWidth; }
int Level::getHeight() const { return levelHeight; }
bool Level::isCampaignMode() const { return isCampaign; }
float Level::getMaxCameraX() const {
    if (isCampaign) return 999999.0f;
    return (float)(levelWidth * BLOCK_SIZE) - 800.0f;
}
