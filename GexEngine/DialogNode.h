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
		Message, HeroOption, Count
	};


public:
						DialogNode( DialogNode::Type type, std::string text);
						DialogNode();

	std::string			getText();
	void				setText(std::string t);
	//bool				isAnswerRequired();
	DialogNode::Type	getType();
	void				attachChild(DialogNode* child);
	std::vector<DialogNode*>* getChildren();


protected:
	Type						type;
	std::string					text;
	std::vector<DialogNode*>	children;


};

