
#include "ResourceIdentifiers.h"
#include "Actor.h"
#include "Animation.h"
#include "TileType.h"
#include "InteractableObject.h"
#include "ObjectWithQuest.h"
#include "Level.h"

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

struct ActorLevelData
{
	ActorLevelData(Actor::Type t, ObjectWithQuest::Type t_, float x_, float y_)
		:type(t)
		, questType(t_)
		, x(x_)
		, y(y_)
	{}

	Actor::Type				type;
	ObjectWithQuest::Type	questType;
	float					x;
	float					y;
};

struct ObjectLevelData
{
	ObjectLevelData(InteractableObject::Type t, ObjectWithQuest::Type t_, float x_, float y_)
		:type(t)
		, questType(t_)
		, x(x_)
		, y(y_)
	{}

	InteractableObject::Type	type;
	ObjectWithQuest::Type	questType;
	float						x;
	float						y;
};

// functions to fill data tables
std::map<Actor::Type, ActorData> initializeActorData();

std::map<Tile::Type, TileData> initializeTileData();

std::map<Level, std::vector<ActorLevelData> > getLevelActorData();
std::map<Level, std::vector<ObjectLevelData> > getLevelObjectData();
std::map<Level, sf::Vector2f>	getLevelportalData();

ObjectWithQuest::Type			transformToQuestObjectType(Actor::Type t);

//std::vector<ActorLevelData>			 getActorLevelData(Level l);
//std::vector<ObjectLevelData>		 getObjectLevelData(Level l);

