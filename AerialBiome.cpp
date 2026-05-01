// AerialBiome.cpp - Aerial biome implementation
// High altitudes with jagged mountainous peaks
#include "AerialBiome.h"

int AerialBiome::getBlockType(int row, int surfaceHeight) const {
    if (row == 0) return BLOCK_BEDROCK;               // Indestructible bottom
    if (row > surfaceHeight) return BLOCK_AIR;         // Above surface = air
    if (row == surfaceHeight) return BLOCK_ANDESITE;    // Surface = mountain rock
    if (row >= surfaceHeight - 1) return BLOCK_STONE;   // 1 layer of stone below
    if (row >= surfaceHeight - 3) return BLOCK_DIRT;    // Then dirt
    return BLOCK_STONE;                                 // Deep = stone
}

const char* AerialBiome::getBiomeName() const {
    return "Aerial";
}

int AerialBiome::adjustHeight(int baseHeight) const {
    // Aerial: jagged peaks, scale UP to [5, 13]
    // This gives mountains up to ~70% of screen height
    int h = baseHeight + 3;
    if (h < 5) h = 5;
    if (h > 13) h = 13;
    return h;
}

int AerialBiome::getBackgroundIndex(int section) const {
    // Cycle through aerial backgrounds: 3.png(2), 4.png(3)
    int bgs[] = {2, 3};
    int idx = section % 2;
    if (idx < 0) idx = 0;
    return bgs[idx];
}
