// AquaticBiome.cpp - Aquatic biome implementation
// Low altitudes with sea level and underwater areas
#include "AquaticBiome.h"

int AquaticBiome::getBlockType(int row, int surfaceHeight) const {
    if (row == 0) return BLOCK_BEDROCK;                             // Indestructible bottom
    if (row > SEA_LEVEL) return BLOCK_AIR;                          // Above sea level = air
    if (row > surfaceHeight && row <= SEA_LEVEL) return BLOCK_WATER; // Water fills between terrain and sea level
    if (row == surfaceHeight) return BLOCK_DIRT;                     // Seafloor = dirt
    if (row >= surfaceHeight - 2) return BLOCK_DIRT;                 // Mud layers
    return BLOCK_STONE;                                              // Deep = stone
}

const char* AquaticBiome::getBiomeName() const {
    return "Aquatic";
}

int AquaticBiome::adjustHeight(int baseHeight) const {
    // Aquatic: LOW terrain, range [2, 4]
    // Sea level is at row 6, so water fills above terrain
    // This makes water ~2-4 blocks deep - enough for underwater exploration
    int h = baseHeight - 3;
    if (h < 2) h = 2;
    if (h > 4) h = 4;
    return h;
}

int AquaticBiome::getBackgroundIndex(int section) const {
    // Ocean background: 6.png(5)
    return 5;
}

bool AquaticBiome::hasWater() const {
    return true;
}
