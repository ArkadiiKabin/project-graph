#include <SFML/Graphics.hpp>

#include "game/Game.hpp"
#include "game/Graph.hpp"
#include "ui/Menu.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1100, 950}), "Dungeon Explorer");
    window.setFramerateLimit(60);
    Graph graph;
    graph.GenerationRooms();
    graph.generateConnections();
    graph.PrintGraph();
    Menu menu;
    Game game;
    game.setGraph(graph);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (menu.isStartGame())
                game.handleEvent(window, *event);
            else
                menu.handleEvent(window, *event);
        }
        if (!menu.isStartGame()) menu.update(window);
        window.clear(sf::Color(25, 25, 35));
        if (menu.isStartGame())
            game.draw(window);
        else
            menu.draw(window);
        window.display();
    }

    return 0;
}