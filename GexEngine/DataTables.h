
#include "ResourceIdentifiers.h"
#include "Actor.h"
#include "Animation.h"
#include "TileType.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <map>
#include <vector>
#include <functional>


struct ActorData
{
	int									hitpoints;
	//int									damageDone;
	//float								speed;
	TextureID							textureID;
	std::map<Actor::State, Animation>	animations;

	std::string							npcName;
};

struct TileData {
	bool								isBlocking;
	TextureID							textureID;
	float								width;
	float								height;
};

// functions to fill data tables
std::map<Actor::Type, ActorData> initializeActorData();

std::map<Tile::Type, TileData> initializeTileData();
