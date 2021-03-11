#include "UiNode.h"


UiNode::UiNode(const sf::Texture& texture)
	: sprite(texture)
{}

UiNode::UiNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	: sprite(texture, textureRect)
{}

void UiNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isVisible_) {
		states.transform *= getTransform();

		// draw current node and its children
		drawCurrent(target, states);
		SceneNode::drawChildren(target, states);
	}

}

void UiNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isVisible_)
		target.draw(sprite, states);
}

sf::FloatRect UiNode::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite.getGlobalBounds());

	return box;
}

sf::FloatRect UiNode::getBaseTileRect() const
{
	auto rect = getBoundingRect();
	const float tileSize = 50.f;

	if (rect.height > tileSize) {
		rect.top = rect.top + rect.height - tileSize;
		rect.height = tileSize;
	}

	return rect;
}

void UiNode::setTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
}


void UiNode::setVisible(bool b)
{
	isVisible_ = b;
}

bool UiNode::isVisible()
{
	return isVisible_;
}

