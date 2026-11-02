#pragma once

#include <SFML/Graphics.hpp>

class Menu {
private:
    bool startGame = false;

    sf::Font font;
    sf::Text title;

    sf::RectangleShape startButton;
    sf::Text startButtonText;

    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    public:
    Menu();
    bool isStartGame() const {return startGame;}
    void draw(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
};