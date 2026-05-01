// Biome.cpp - Biome abstract base class implementation
#include "Biome.h"

Biome::~Biome() {}

bool Biome::hasWater() const {
    return false;
}

int Biome::getSeaLevel() const {
    return SEA_LEVEL;
}
