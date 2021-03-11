
#include "SceneNode.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


class SpriteNode;

#pragma once
class UiNode : public SceneNode
{
public:
	explicit		UiNode(const sf::Texture& texture);
	UiNode(const sf::Texture& texture, const sf::IntRect& textureRect);
	sf::FloatRect	getBoundingRect() const override;
	sf::FloatRect	getBaseTileRect() const override;

	void			setTexture(sf::Texture& texture);

	void				setVisible(bool b);
	bool				isVisible();


protected:
	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite		sprite;
	bool isVisible_;
};

