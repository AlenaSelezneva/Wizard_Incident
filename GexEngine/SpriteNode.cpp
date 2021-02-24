
#include "SpriteNode.h"

SpriteNode::SpriteNode(const sf::Texture& texture)
	: sprite(texture)
{}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	: sprite(texture, textureRect)
{}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

sf::FloatRect SpriteNode::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite.getGlobalBounds());

	return box;
}

sf::FloatRect SpriteNode::getBaseTileRect() const
{
	auto rect = getBoundingRect();
	const float tileSize = 50.f;

	if (rect.height > tileSize) {
		rect.top = rect.top + rect.height - tileSize;
		rect.height = tileSize;
	}

	return rect;
}