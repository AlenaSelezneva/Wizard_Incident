#include "FriendlyNPC.h"
#include "DataTables.h"
#include "TextNode.h"
#include "Utility.h"


FriendlyNPC::FriendlyNPC(Actor::Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(type, textures, fonts)
    , ObjectWithQuest(questType)
{
    name = initializeActorData().at(type).npcName;

    std::unique_ptr<TextNode> nameText(new TextNode(fonts, name, 14));
    nameText.get()->setString(name);
    nameText->setPosition(0.f, -40.f);

    attachChild(std::move(nameText));
    directionToMove = direction_;
    movementCountDown = initializeActorData().at(type).countDownStart;
}

FriendlyNPC::FriendlyNPC(Actor* actor) 
    :Actor(actor->getType(), actor->getTextures(), actor->getFonts())
    , ObjectWithQuest(transformToQuestObjectType(actor->getType()))
{
    this->setPosition(actor->getPosition());
    this->setState(actor->getState());
    this->setVelocity(actor->getVelocity());

    hitPoints = 100;

    name = initializeActorData().at(actor->getType()).npcName;

    std::unique_ptr<TextNode> nameText(new TextNode(fonts, name, 14));
    nameText.get()->setString(name);
    nameText->setPosition(0.f, -40.f);

    attachChild(std::move(nameText));
}

void FriendlyNPC::setCanTalkToHero(bool canTalk)
{
    canTalkToHero_ = canTalk;
}

bool FriendlyNPC::canTalkToHero()
{
    return canTalkToHero_;
}

std::string FriendlyNPC::getName()
{
    return name;
}

void FriendlyNPC::setName(std::string n)
{
    name = n;
}

void FriendlyNPC::updateMovementPattern(sf::Time dt)
{
    movementCountDown -= dt;

    if (movementCountDown <= sf::Time::Zero) {
        movementCountDown = MOVEMENT_TIME_MAX;
        isMoving_ = !isMoving_;

        if (!isMoving_) {
            setVelocity(0.f, 0.f);
        }
        else {
            directionToMove = getNextDirection();

            float v = 80.f;
            sf::Vector2f velocity(0.f, 0.f);

            switch (directionToMove)
            {
            case Actor::Direction::Back:
                velocity = sf::Vector2f(-v, 0.f);
                break;
            case Actor::Direction::Front:
                velocity = sf::Vector2f(v, 0.f);
                break;
            case Actor::Direction::Right:
                velocity = sf::Vector2f(0.f, v);
                break;
            case Actor::Direction::Left:
                velocity = sf::Vector2f(0.f, -v);
                break;
            default:
                break;
            }

            setVelocity(velocity);
        }

    }
}

void FriendlyNPC::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (type_ != Actor::Type::Archmage)
        updateMovementPattern(dt);

    updateDirections();
    updateStates();

    //isCastingShield_ = false;
    Entity::updateCurrent(dt, commands);

    auto rec = animations_.at(state_).update(dt);

    sprite_.setTextureRect(rec);
    centerOrigin(sprite_);
}

Actor::Direction FriendlyNPC::getNextDirection()
{
    Direction res = Direction::Front;
    switch (directionToMove)
    {
    case Actor::Direction::Back:
        res = Direction::Right;
        break;
    case Actor::Direction::Right:
        res = Direction::Front;
        break;
    case Actor::Direction::Front:
        res = Direction::Left;
        break;
    case Actor::Direction::Left:
        res = Direction::Back;
        break;
    default:
        break;
    }
    return res;
}
