#include "ObjectWithQuest.h"

ObjectWithQuest::ObjectWithQuest(ObjectWithQuest::Type type)
    : questObjectType(type)
{
}

ObjectWithQuest::Type ObjectWithQuest::getQuestObjectType()
{
    return questObjectType;
}

void ObjectWithQuest::setQuestObjectType(ObjectWithQuest::Type t)
{
    questObjectType = t;
}
