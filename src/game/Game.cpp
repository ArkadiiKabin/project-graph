#include "Game.h"
#include <string>
#include <cmath>
Game::Game(){
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) {
        return;
    }
    // левая панель
    dungeonArea.setSize({550.f, 500.f});
    dungeonArea.setPosition({20.f, 20.f});
    dungeonArea.setFillColor(sf::Color(35, 35, 40));

    // правая панель
    controlPanel.setSize({190.f, 500.f});
    controlPanel.setPosition({590.f, 20.f});
    controlPanel.setFillColor(sf::Color(35, 35, 45));

    // кнопка бфс
    bfsButton.setSize({155.f, 45.f});
    bfsButton.setPosition({607.f, 100.f});
    bfsButton.setFillColor(sf::Color(65, 65, 80));

    // кнопка дфс
    dfsButton.setSize({155.f, 45.f});
    dfsButton.setPosition({607.f, 165.f});
    dfsButton.setFillColor(sf::Color(65, 65, 80));

    // кнопка дэйкстры
    dijkstraButton.setSize({155.f, 45.f});
    dijkstraButton.setPosition({607.f, 230.f});
    dijkstraButton.setFillColor(sf::Color(65, 65, 80));

    // камера
    dungeonCamera.setSize({550.f, 500.f});
    dungeonCamera.setCenter({295.f, 270.f});
    dungeonCamera.setViewport(
    sf::FloatRect(
        {0.025f, 0.0333f},
        {0.6875f, 0.8333f}
    )
);
}

void Game::draw(sf::RenderWindow& window){
    window.setView(dungeonCamera);
    window.draw(dungeonArea);
    if(graph == nullptr) return;
    for(int i = 0; i < graph->getRoomCount(); i++){
        const Room& room = graph->getRoom(i);
        for(const Connection& connection : room.connectedRooms){
            if(connection.roomId <= room.id) continue;
            const Room& neighbor = graph->getRoom(connection.roomId);

            float dx = neighbor.position.x - room.position.x;
            float dy = neighbor.position.y - room.position.y;
            float length = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
            sf::RectangleShape line({length, 2.f});
            line.setOrigin({0.f, 1.f});
            line.setPosition(room.position);
            line.setRotation(sf::degrees(angle));
            line.setFillColor(sf::Color(130, 130, 140));
            window.draw(line);
        }
    }
    for(int i = 0; i < graph->getRoomCount(); i++){
        const Room& room = graph->getRoom(i);
        sf::CircleShape circle(12.f);
        circle.setOrigin({12.f, 12.f});
        circle.setPosition(room.position);
        circle.setFillColor(sf::Color(70, 160, 220));
        window.draw(circle);
        sf::Text number(font, std::to_string(room.id), 10);
        number.setFillColor(sf::Color::White);
        number.setOrigin(number.getLocalBounds().size / 2.f);
        number.setPosition({room.position.x - 0.5f, room.position.y - 2.f});
        window.draw(number);
    }
    // Возвращаем обычную камеру окна
    window.setView(window.getDefaultView());

    // Рисуем панель и кнопки
    window.draw(controlPanel);
    window.draw(bfsButton);
    window.draw(dfsButton);
    window.draw(dijkstraButton);

    // Заголовок панели
    sf::Text title(font, "ALGORITHMS", 20);
    title.setFillColor(sf::Color::White);
    title.setPosition({610.f, 50.f});
    window.draw(title);

    // Надписи на кнопках
    sf::Text bfsText(font, "BFS", 18);
    bfsText.setFillColor(sf::Color::White);
    bfsText.setPosition({665.f, 110.f});
    window.draw(bfsText);

    sf::Text dfsText(font, "DFS", 18);
    dfsText.setFillColor(sf::Color::White);
    dfsText.setPosition({665.f, 175.f});
    window.draw(dfsText);

    sf::Text dijkstraText(font, "Dijkstra", 18);
    dijkstraText.setFillColor(sf::Color::White);
    dijkstraText.setPosition({645.f, 240.f});
    window.draw(dijkstraText);
}

void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event){
    if(const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()){
        sf::Vector2i mousePos = mouse->position;
        sf::Vector2f beforePos = window.mapPixelToCoords(mousePos, dungeonCamera);
        if(mouse->delta > 0) dungeonCamera.zoom(0.9f);
        else dungeonCamera.zoom(1.1f);
        sf::Vector2f afterPos = window.mapPixelToCoords(mousePos, dungeonCamera);
        dungeonCamera.move(beforePos - afterPos);
    }
}

void Game::setGraph(Graph& graph){
    this->graph = &graph;
}