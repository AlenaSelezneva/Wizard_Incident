#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "ObjectWithQuest.h"

#include <SFML/Graphics/Sprite.hpp>

#pragma once
class InteractableObject : public Entity, public ObjectWithQuest
{
public:
    enum Type {
        None = 0,
        BookshelfQuest = 1,
        BookshelfNotQuest = 1 << 1,

        Bookshelf = BookshelfQuest | BookshelfNotQuest,
        Count
    };



public:
    InteractableObject(Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts);
    ~InteractableObject() = default;

    unsigned int    getCategory() const override;
    sf::FloatRect   getBoundingRect() const override;
    sf::FloatRect   getBaseTileRect() const override;

    InteractableObject::Type     getType() const;


private:
    void         drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const override;

protected:
    Type                            type_;
    /*mutable sf::Sprite              sprite_;*/
    sf::Sprite                      sprite_;
};

