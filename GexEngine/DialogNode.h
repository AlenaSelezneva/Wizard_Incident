/*
Alena Selezneva
*/

#pragma once
#include <string>
#include <vector>
#include <list>

class DialogNode
{
public:
	enum class Type {
		Message, Answer, Count
	};


public:
						DialogNode(std::string text, DialogNode::Type type);

	std::string			getText();
	bool				isAnswerRequired();
	DialogNode::Type	getType();
	void				attachChild(DialogNode* child);


protected:
	std::string					text;
	std::list<DialogNode*>		children;
	Type						type;

};

