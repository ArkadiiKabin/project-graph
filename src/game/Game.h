#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.h"

class Game{
    private:
    sf::Font font;
    Graph* graph;
    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;
    sf::RectangleShape controlPanel;
    sf::RectangleShape bfsButton;
    sf::RectangleShape dfsButton;
    sf::RectangleShape dijkstraButton;
    public:
    Game();
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
    void setGraph(Graph& graph);
};