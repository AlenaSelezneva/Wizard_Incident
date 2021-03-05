#include "DataTables.h"

std::map<Actor::Type, ActorData> initializeActorData()
{
    return std::map<Actor::Type, ActorData>();
}

std::map<Tile::Type, TileData> initializeTileData()
{
    std::map<Tile::Type, TileData> map;
    
    map[Tile::Type::Floor].isBlocking = false;
    map[Tile::Type::Floor].textureID = TextureID::Floor;
    map[Tile::Type::Floor].width = 50.f;
    map[Tile::Type::Floor].height = 50.f;

    map[Tile::Type::Wall].isBlocking = true;
    map[Tile::Type::Wall].textureID = TextureID::Wall;
    map[Tile::Type::Wall].width = 50.f;
    map[Tile::Type::Wall].height = 150.f;

    return map;
}
