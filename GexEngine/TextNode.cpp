/*
Alena Selezneva
*/

#include "TextNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder_t& fonts, const std::string& text_, int charSize)
{
	text.setFont(fonts.get(FontID::Main));
	text.setCharacterSize(charSize);
	setString(text_);
}

void TextNode::setString(const std::string& text_)
{
	text.setString(text_);
	centerOrigin(text);
}

void TextNode::setTextColor(sf::Color color)
{
	//text.setColor(color);
	text.setFillColor(color);
}

void TextNode::setCharacterSize(size_t size)
{
	text.setCharacterSize(size);
}

sf::Text* TextNode::getText()
{
	return &text;
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(text, states);
}
