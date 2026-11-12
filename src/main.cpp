#include <SFML/Graphics.hpp>

#include "game/Game.hpp"
#include "game/Graph.hpp"
#include "ui/Menu.hpp"

int main() {
    sf::RenderWindow window(
        sf::VideoMode({1100, 950}),
        "Dungeon Explorer"
    );

    window.setFramerateLimit(60);

    // Создание и генерация графа.
    Graph graph;

    graph.GenerationRooms();
    graph.generateConnections();
    graph.PrintGraph();

    // Создание меню и игрового объекта.
    Menu menu;
    Game game;

    game.setGraph(graph);

    // Главный игровой цикл.
    while (window.isOpen()) {
        // Обработка событий.
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (menu.isStartGame()) {
                game.handleEvent(window, *event);
            } else {
                menu.handleEvent(window, *event);
            }
        }

        // Обновление меню.
        if (!menu.isStartGame()) {
            menu.update(window);
        }

        // Отрисовка текущего экрана.
        window.clear(sf::Color(25, 25, 35));

        if (menu.isStartGame()) {
            game.draw(window);
        } else {
            menu.draw(window);
        }

        window.display();
    }

    return 0;
}