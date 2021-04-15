/*
Alena Selezneva
*/

#include "DialogAnswer.h"

DialogAnswer::DialogAnswer(std::string text):
	DialogNode(DialogNode::Type::HeroOption, text)
{}

DialogAnswer::DialogAnswer(std::string text, Attribute attr)
	: DialogNode(DialogNode::Type::HeroOption, text)
{
	attribute = attr;
}
