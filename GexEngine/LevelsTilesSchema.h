#include "TileType.h"

#include <vector>

#pragma once
class LevelsTilesSchema
{
public:
	static std::vector<std::vector<Tile::Type> > getLevelLayout();
	static std::vector<std::vector<Tile::Type> > getBattleLevelLayout();

private:
	static std::vector<std::vector<Tile::Type>> IntVectorToTileType(std::vector<std::vector<int> > input, int n, int m);

private:
	static std::vector<std::vector<Tile::Type> > levelLayout;

};

