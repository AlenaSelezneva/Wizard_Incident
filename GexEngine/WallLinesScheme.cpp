
#include "WallLinesScheme.h"

WallLinesScheme::WallLinesScheme()
{
	wallsCount = 4;
	wallLines = std::vector< sf::VertexArray>(wallsCount);

	for (int i = 0; i < wallsCount; ++i) {
		wallLines[i] = sf::VertexArray(sf::LineStrip, 2);
		wallLines[i][0].color = sf::Color::Red;
		wallLines[i][1].color = sf::Color::Red;
	}

	wallLines[0][0] = sf::Vector2f(100, 40);
	wallLines[0][1] = sf::Vector2f(600, 0);

	wallLines[1][0] = sf::Vector2f(600, 0);
	wallLines[1][1] = sf::Vector2f(500, 500);

	wallLines[2][0] = sf::Vector2f(500, 500);
	wallLines[2][1] = sf::Vector2f(50, 350);

	wallLines[3][0] = sf::Vector2f(50, 350);
	wallLines[3][1] = sf::Vector2f(150, 200);


	doorsCount = 0;
	doorLines = sf::VertexArray(sf::LineStrip, doorsCount);
}
