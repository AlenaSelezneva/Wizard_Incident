
#include "ResourceIdentifiers.h"
#include "Actor.h"
#include "Animation.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <map>
#include <vector>
#include <functional>


struct ActorData
{
	int									hitpoints;
	int									damageDone;
	float								speed;
	TextureID							texture;
	std::map<Actor::State, Animation>	animations;

};

// functions to fill data tables
std::map<Actor::Type, ActorData> initializeActorData();
