#pragma once
// AquaticBiome.h - Low altitude terrain with water/ocean
// INHERITANCE from Biome
#include "Biome.h"

class AquaticBiome : public Biome {
public:
    int getBlockType(int row, int surfaceHeight) const override;
    const char* getBiomeName() const override;
    int adjustHeight(int baseHeight) const override;
    int getBackgroundIndex(int section) const override;
    bool hasWater() const override;
};
