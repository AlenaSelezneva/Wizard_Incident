
#include <SFML/Graphics.hpp>
#include "Application.h"
#include <iostream>

int main()
{
    try {
        Application app;
        app.run();
    }
    catch (std::exception& e) {
        std::cout << "\n\nException: " << e.what() << "\n";
    }

   /* sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
    sf::CircleShape shape(20.f, 6);
    shape.setPosition(640.f, 360.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }*/

    return 0;
}