#include "FriendlyNPC.h"
#include "DataTables.h"
#include "TextNode.h"


FriendlyNPC::FriendlyNPC(Actor::Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts)
    :Actor(type, textures, fonts)
    , ObjectWithQuest(questType)
{
    name = initializeActorData().at(type).npcName;

    std::unique_ptr<TextNode> nameText(new TextNode(fonts, name, 14));
    nameText.get()->setString(name);
    nameText->setPosition(0.f, -40.f);

    attachChild(std::move(nameText));

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