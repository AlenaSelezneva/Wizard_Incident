/*
Alena Selezneva
*/

#include "DialogNode.h"

DialogNode::DialogNode(std::string text, DialogNode::Type type)
	: text(text)
	, type(type)
{
	children = std::list<DialogNode*>();
}

std::string DialogNode::getText()
{
	return text;
}

bool DialogNode::isAnswerRequired()
{
	return children.size() > 1;
}

DialogNode::Type DialogNode::getType()
{
	return type;
}

void DialogNode::attachChild(DialogNode* child)
{
	children.push_back(child);
}
