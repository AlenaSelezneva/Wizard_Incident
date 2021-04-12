#include "DataTables.h"
#include "JsonFrameParser.h"

std::map<Actor::Type, ActorData> initializeActorData()
{
    std::map<Actor::Type, ActorData> data;

    data[Actor::Type::Gazan].npcName = "Gazan";

    /////////////////////////////////////////// Archmage ///////////////////////////////////////////////

    data[Actor::Type::Archmage].textureID = TextureID::Archmage;
    data[Actor::Type::Archmage].npcName = "Archmage";

    JsonFrameParser frames = JsonFrameParser("Media/Textures/Archmage_sheet.json");

    data[Actor::Type::Archmage].animations[Actor::State::MoveBack].addFrameSet(frames.getFramesFor("walk_back"));
    data[Actor::Type::Archmage].animations[Actor::State::MoveBack].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Archmage].animations[Actor::State::MoveBack].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::MoveRight].addFrameSet(frames.getFramesFor("walk_right"));
    data[Actor::Type::Archmage].animations[Actor::State::MoveRight].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Archmage].animations[Actor::State::MoveRight].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::MoveFront].addFrameSet(frames.getFramesFor("walk_front"));
    data[Actor::Type::Archmage].animations[Actor::State::MoveFront].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Archmage].animations[Actor::State::MoveFront].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::MoveLeft].addFrameSet(frames.getFramesFor("walk_left"));
    data[Actor::Type::Archmage].animations[Actor::State::MoveLeft].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Archmage].animations[Actor::State::MoveLeft].setRepeating(true);

    auto idleBack = frames.getFramesFor("cast_back/Archmage-0");
    idleBack.push_back(frames.getFramesFor("cast_back/Archmage-1")[0]);

    data[Actor::Type::Archmage].animations[Actor::State::IdleBack].addFrameSet(idleBack);
    data[Actor::Type::Archmage].animations[Actor::State::IdleBack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::IdleBack].setRepeating(true);

    auto idleRight = frames.getFramesFor("cast_right/Archmage-21");
    idleRight.push_back(frames.getFramesFor("cast_right/Archmage-22")[0]);

    data[Actor::Type::Archmage].animations[Actor::State::IdleRight].addFrameSet(idleRight);
    data[Actor::Type::Archmage].animations[Actor::State::IdleRight].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::IdleRight].setRepeating(true);

    auto idleFront = frames.getFramesFor("cast_front/Archmage-14");
    idleFront.push_back(frames.getFramesFor("cast_front/Archmage-15")[0]);

    data[Actor::Type::Archmage].animations[Actor::State::IdleFront].addFrameSet(idleFront);
    data[Actor::Type::Archmage].animations[Actor::State::IdleFront].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::IdleFront].setRepeating(true);

    auto idleLeft = frames.getFramesFor("cast_left/Archmage-7");
    idleLeft.push_back(frames.getFramesFor("cast_left/Archmage-8")[0]);

    data[Actor::Type::Archmage].animations[Actor::State::IdleLeft].addFrameSet(idleLeft);
    data[Actor::Type::Archmage].animations[Actor::State::IdleLeft].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::IdleLeft].setRepeating(true);


    data[Actor::Type::Archmage].animations[Actor::State::SpellCastBack].addFrameSet(frames.getFramesFor("cast_back"));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastBack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastBack].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::SpellCastRight].addFrameSet(frames.getFramesFor("cast_right"));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastRight].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastRight].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::SpellCastFront].addFrameSet(frames.getFramesFor("cast_front"));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastFront].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastFront].setRepeating(true);

    data[Actor::Type::Archmage].animations[Actor::State::SpellCastLeft].addFrameSet(frames.getFramesFor("cast_left"));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastLeft].setDuration(sf::seconds(1.f));
    data[Actor::Type::Archmage].animations[Actor::State::SpellCastLeft].setRepeating(true);

    /////////////////////////////////////////// HERO ///////////////////////////////////////////////

    data[Actor::Type::Hero].textureID = TextureID::Hero;

    frames = JsonFrameParser("Media/Textures/hero.json");

    data[Actor::Type::Hero].animations[Actor::State::MoveBack].addFrameSet(frames.getFramesFor("walk_back"));
    data[Actor::Type::Hero].animations[Actor::State::MoveBack].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Hero].animations[Actor::State::MoveBack].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::MoveRight].addFrameSet(frames.getFramesFor("walk_right"));
    data[Actor::Type::Hero].animations[Actor::State::MoveRight].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Hero].animations[Actor::State::MoveRight].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::MoveFront].addFrameSet(frames.getFramesFor("walk_front"));
    data[Actor::Type::Hero].animations[Actor::State::MoveFront].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Hero].animations[Actor::State::MoveFront].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::MoveLeft].addFrameSet(frames.getFramesFor("walk_left"));
    data[Actor::Type::Hero].animations[Actor::State::MoveLeft].setDuration(sf::seconds(1.5f));
    data[Actor::Type::Hero].animations[Actor::State::MoveLeft].setRepeating(true);

    idleBack = frames.getFramesFor("cast_back-1");
    idleBack.push_back(frames.getFramesFor("cast_back-2")[0]);

    data[Actor::Type::Hero].animations[Actor::State::IdleBack].addFrameSet(idleBack);
    data[Actor::Type::Hero].animations[Actor::State::IdleBack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::IdleBack].setRepeating(true);

    idleRight = frames.getFramesFor("cast_right-21");
    idleRight.push_back(frames.getFramesFor("cast_right-22")[0]);

    data[Actor::Type::Hero].animations[Actor::State::IdleRight].addFrameSet(idleRight);
    data[Actor::Type::Hero].animations[Actor::State::IdleRight].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::IdleRight].setRepeating(true);

    idleFront = frames.getFramesFor("cast_front-14");
    idleFront.push_back(frames.getFramesFor("cast_front-15")[0]);

    data[Actor::Type::Hero].animations[Actor::State::IdleFront].addFrameSet(idleFront);
    data[Actor::Type::Hero].animations[Actor::State::IdleFront].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::IdleFront].setRepeating(true);

    idleLeft = frames.getFramesFor("cast_left-7");
    idleLeft.push_back(frames.getFramesFor("cast_left-8")[0]);

    data[Actor::Type::Hero].animations[Actor::State::IdleLeft].addFrameSet(idleLeft);
    data[Actor::Type::Hero].animations[Actor::State::IdleLeft].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::IdleLeft].setRepeating(true);


    data[Actor::Type::Hero].animations[Actor::State::SpellCastBack].addFrameSet(frames.getFramesFor("cast_back"));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastBack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastBack].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::SpellCastRight].addFrameSet(frames.getFramesFor("cast_right"));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastRight].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastRight].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::SpellCastFront].addFrameSet(frames.getFramesFor("cast_front"));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastFront].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastFront].setRepeating(true);

    data[Actor::Type::Hero].animations[Actor::State::SpellCastLeft].addFrameSet(frames.getFramesFor("cast_left"));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastLeft].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero].animations[Actor::State::SpellCastLeft].setRepeating(true);


    return data;
}

void initializeActorAnimations(std::map<Actor::Type, ActorData>* data, Actor::Type type) {

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

std::map<Level, std::vector<ActorLevelData>> getLevelActorData()
{
    auto map = std::map<Level, std::vector<ActorLevelData>>();

    map[Level::First] = std::vector<ActorLevelData>();
    map[Level::First].push_back(ActorLevelData(Actor::Type::Archmage, ObjectWithQuest::Type::Archmage, +200.f, -200.f));
    map[Level::First].push_back(ActorLevelData(Actor::Type::Gazan, ObjectWithQuest::Type::Gazan, +500.f, -500.f));


    return map;
}

std::map<Level, std::vector<ObjectLevelData>> getLevelObjectData()
{
    auto map = std::map<Level, std::vector<ObjectLevelData>>();

    map[Level::First] = std::vector<ObjectLevelData>();
    map[Level::First].push_back(ObjectLevelData(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfQuest, -100.f, -1100.f));
    map[Level::First].push_back(ObjectLevelData(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, +300.f, -1100.f));
    map[Level::First].push_back(ObjectLevelData(InteractableObject::Type::BookshelfNotQuest, ObjectWithQuest::Type::BookshelfNotQuest, +700.f, -1100.f));

    return map;
}
