
#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf
{
	class Sprite;
	class Text;
}

class TextNode;

class Animation;

void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);
//void			centerOriginHorizontally(TextNode* text);
//void			centerOrigin(Animation& animation);


float			toDegree(float radian);
float			toRadian(float degree);
float			length(sf::Vector2f v);
sf::Vector2f	normalize(sf::Vector2f v);
int				randomInt(int exclusiveMax);

// rectang;e left right flip
sf::IntRect		flip(const sf::IntRect& rec);
