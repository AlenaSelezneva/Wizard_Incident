#pragma once
class ObjectWithQuest
{
public:
    enum class Type {
        None = 0,
        //Hero = 1,
        Archmage = 1 << 1,
        Dgery = 1 << 2,
        Elony = 1 << 3,
        Gazan = 1 << 4,
        Lunars = 1 << 5,
        Semrid = 1 << 6,
        MysteriousFigure = 1 << 7,

        BookshelfQuest  = 1 << 8,
        BookshelfNotQuest = 1 << 9,

        Bookshelf = BookshelfQuest | BookshelfNotQuest,
        TalkingNPC = Archmage | Dgery | Elony | Gazan | Lunars | Semrid,
    };


public:
    ObjectWithQuest(ObjectWithQuest::Type type);
    ObjectWithQuest::Type       getQuestObjectType();
    void                        setQuestObjectType(ObjectWithQuest::Type t);


private:
    Type            questObjectType;

};

