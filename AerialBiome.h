#pragma once
// AerialBiome.h - High altitude mountainous terrain
// INHERITANCE from Biome
#include "Biome.h"

class AerialBiome : public Biome {
public:
    int getBlockType(int row, int surfaceHeight) const override;
    const char* getBiomeName() const override;
    int adjustHeight(int baseHeight) const override;
    int getBackgroundIndex(int section) const override;
};
