#include "DataTables.h"
#include "JsonFrameParser.h"

std::map<Actor::Type, ActorData> initializeActorData()
{
    std::map<Actor::Type, ActorData> data;

    data[Actor::Type::Hero].texture = TextureID::Hero;

    JsonFrameParser frames = JsonFrameParser("Media/Textures/Archmage_sheet.json");

    data[Actor::Type::Hero].animations[Actor::State::MoveDown].addFrameSet(frames.getFramesFor("walk_right"));
    data[Actor::Type::Hero].animations[Actor::State::MoveDown].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::MoveDown].setRepeating(true);

    return data;
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
