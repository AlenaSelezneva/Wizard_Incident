#include "InteractableObject.h"

#include <SFML/Graphics/RenderTarget.hpp>

InteractableObject::InteractableObject(Type type, ObjectWithQuest::Type questType, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Entity(100)
	, ObjectWithQuest(questType)
	, type_(type)
	, sprite_()
{

	if (type_ & InteractableObject::Type::Bookshelf) {
		sprite_.setTexture(textures.get(TextureID::BookShelf));
	}
}

unsigned int InteractableObject::getCategory() const
{
	switch (type_)
	{
	case InteractableObject::Type::BookshelfQuest:
		return Category::InteractableObject;
		break;
	case InteractableObject::Type::BookshelfNotQuest:
		return Category::InteractableObject;
		break;
	default:
		return Category::None;
		break;
	}
}

sf::FloatRect InteractableObject::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());

	return box;
}

sf::FloatRect InteractableObject::getBaseTileRect() const
{
	auto rect = getBoundingRect();
	const float tileSize = 50.f;

	if (rect.height > tileSize) {
		rect.top = rect.top + rect.height - tileSize;
		rect.height = tileSize;
	}

	return rect;
}

InteractableObject::Type InteractableObject::getType() const
{
	return type_;
}

void InteractableObject::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}
