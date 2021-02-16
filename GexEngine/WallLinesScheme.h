#pragma once
#include <SFML\Graphics\VertexArray.hpp>


class WallLinesScheme
{
public:
	WallLinesScheme();

public:
	std::vector< sf::VertexArray> wallLines;
	sf::VertexArray doorLines;

	int wallsCount;
	int doorsCount;



};

