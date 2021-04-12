#include "TileType.h"
#include "Level.h"


#include <vector>

#pragma once
class LevelsTilesSchema
{
public:
	static std::vector<std::vector<Tile::Type> > getLevelLayout(Level l);

private:
	static std::vector<std::vector<Tile::Type> > getFirstLevelLayout();
	static std::vector<std::vector<Tile::Type> > getSecondLevelLayout();
	static std::vector<std::vector<Tile::Type> > getDangeonLevelLayout();

private:
	static std::vector<std::vector<Tile::Type>> IntVectorToTileType(std::vector<std::vector<int> > input, int n, int m);

private:
	static std::vector<std::vector<Tile::Type> > levelLayout;

};
