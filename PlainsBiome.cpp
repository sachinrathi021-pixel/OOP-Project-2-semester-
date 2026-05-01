// PlainsBiome.cpp - Plains biome implementation
// Central altitudes with small gentle hills
#include "PlainsBiome.h"

int PlainsBiome::getBlockType(int row, int surfaceHeight) const {
    if (row == 0) return BLOCK_BEDROCK;            // Indestructible bottom
    if (row > surfaceHeight) return BLOCK_AIR;      // Above surface = air
    if (row == surfaceHeight) return BLOCK_GRASS;    // Surface = grass
    if (row >= surfaceHeight - 2) return BLOCK_DIRT; // 2 layers of dirt
    return BLOCK_STONE;                              // Deep = stone
}

const char* PlainsBiome::getBiomeName() const {
    return "Plains";
}

int PlainsBiome::adjustHeight(int baseHeight) const {
    // Plains: gentle rolling hills, range [3, 6]
    // Keep it low and flat
    int h = baseHeight;
    if (h > 6) h = 6;
    if (h < 3) h = 3;
    return h;
}

int PlainsBiome::getBackgroundIndex(int section) const {
    // Cycle through plains backgrounds: 1.png(0), 2.png(1), 5.png(4), 7.png(6)
    int bgs[] = {0, 1, 4, 6};
    int idx = section % 4;
    if (idx < 0) idx = 0;
    return bgs[idx];
}
