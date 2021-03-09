#include "DialogMessage.h"

DialogMessage::DialogMessage(std::string text)
	: DialogNode( DialogNode::Type::Message, text)
{}

bool DialogMessage::isAnswerRequired()
{
	if (children.size() == 0)
		return false;
	if (children.size() > 1 || children[0]->getType() == DialogNode::Type::HeroOption)
		return true;
}
