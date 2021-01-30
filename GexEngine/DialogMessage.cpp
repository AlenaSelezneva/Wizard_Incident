#include "DialogMessage.h"

DialogMessage::DialogMessage(std::string text)
	: DialogNode(text, DialogNode::Type::Message)
{}
