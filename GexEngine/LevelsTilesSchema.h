#include "TileType.h"

#include <vector>

#pragma once
class LevelsTilesSchema
{
public:
	static std::vector<std::vector<Tile::Type> > getLevelLayout();

private:
	static std::vector<std::vector<Tile::Type> > levelLayout;

};

