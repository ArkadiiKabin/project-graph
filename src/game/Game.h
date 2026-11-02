#pragma once
#include <SFML/Graphics.hpp>

class Game{
    private:
    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;
    public:
    Game();
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
};