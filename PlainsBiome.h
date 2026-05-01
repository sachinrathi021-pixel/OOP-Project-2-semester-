#pragma once
// PlainsBiome.h - Central altitude terrain with small hills
// INHERITANCE from Biome
#include "Biome.h"

class PlainsBiome : public Biome {
public:
    int getBlockType(int row, int surfaceHeight) const override;
    const char* getBiomeName() const override;
    int adjustHeight(int baseHeight) const override;
    int getBackgroundIndex(int section) const override;
};
