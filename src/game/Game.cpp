#include "Game.h"
#include <string>
#include <cmath>
#include <sstream>

Game::Game()
    : infoTitle(font),
      infoContent(font),
      startLabel(font),
      endLabel(font),
      startValue(font),
      endValue(font),
      dungeonInfoText(font)
{
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) {
        return;
    }

    // Левая панель
    dungeonArea.setSize({800.f, 800.f});
    dungeonArea.setPosition({20.f, 20.f});
    dungeonArea.setFillColor(sf::Color(35, 35, 40));

    // Правая панель
    controlPanel.setSize({240.f, 800.f});
    controlPanel.setPosition({840.f, 20.f});
    controlPanel.setFillColor(sf::Color(35, 35, 45));

    // Кнопка BFS
    bfsButton.setSize({155.f, 45.f});
    bfsButton.setPosition({882.f, 130.f});
    bfsButton.setFillColor(sf::Color(65, 65, 80));

    // Кнопка DFS
    dfsButton.setSize({155.f, 45.f});
    dfsButton.setPosition({882.f, 195.f});
    dfsButton.setFillColor(sf::Color(65, 65, 80));

    // Кнопка Дейкстры
    dijkstraButton.setSize({155.f, 45.f});
    dijkstraButton.setPosition({882.f, 260.f});
    dijkstraButton.setFillColor(sf::Color(65, 65, 80));

    // Поле начальной комнаты
    startInput.setSize({155.f, 35.f});
    startInput.setPosition({882.f, 345.f});
    startInput.setFillColor(sf::Color(50, 50, 65));

    // Поле конечной комнаты
    endInput.setSize({155.f, 35.f});
    endInput.setPosition({882.f, 420.f});
    endInput.setFillColor(sf::Color(50, 50, 65));

    // Кнопка генерации
    generateButton.setSize({155.f, 40.f});
    generateButton.setPosition({882.f, 495.f});
    generateButton.setFillColor(sf::Color(65, 100, 80));

    // Подписи
    startLabel.setString("Start room:");
    startLabel.setCharacterSize(14);
    startLabel.setPosition({882.f, 320.f});

    endLabel.setString("End room:");
    endLabel.setCharacterSize(14);
    endLabel.setPosition({882.f, 395.f});

    // Значения
    startValue.setString(startInputText);
    startValue.setCharacterSize(16);
    startValue.setPosition({892.f, 350.f});

    endValue.setString(endInputText);
    endValue.setCharacterSize(16);
    endValue.setPosition({892.f, 425.f});

    // Нижняя информационная панель
    infoPanel.setSize({1060.f, 100.f});
    infoPanel.setPosition({20.f, 830.f});
    infoPanel.setFillColor(sf::Color(35, 35, 45));

    // Заголовок
    infoTitle.setCharacterSize(18);
    infoTitle.setFillColor(sf::Color::White);
    infoTitle.setPosition({35.f, 840.f});
    infoTitle.setString("ALGORITHM RESULTS");

    // Содержимое
    infoContent.setCharacterSize(14);
    infoContent.setFillColor(sf::Color(200, 200, 210));
    infoContent.setPosition({35.f, 870.f});
    infoContent.setString("Select an algorithm to see its results");

    // Кнопка переключения
    infoToggleButton.setSize({165.f, 40.f});
    infoToggleButton.setPosition({895.f, 855.f});
    infoToggleButton.setFillColor(sf::Color(65, 85, 105));

    // Панель информации о подземелье
    dungeonInfoPanel.setSize({1060.f, 280.f});
    dungeonInfoPanel.setPosition({20.f, 915.f});
    dungeonInfoPanel.setFillColor(sf::Color(35, 35, 45));

    dungeonInfoText.setCharacterSize(11);
    dungeonInfoText.setFillColor(sf::Color(200, 200, 210));
    dungeonInfoText.setPosition({35.f, 925.f});

    // Камера
    dungeonCamera.setSize({800.f, 800.f});
    dungeonCamera.setCenter({420.f, 420.f});

    dungeonCamera.setViewport(
        sf::FloatRect(
            {0.01818f, 0.02105f},
            {0.72727f, 0.8421f}
        )
    );
}


void Game::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.draw(dungeonArea);
    window.setView(dungeonCamera);
    if (graph == nullptr) return;
    // Рисуем связи между комнатами
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);
        for (const Connection& connection : room.connectedRooms) {
            if (connection.roomId <= room.id) continue;
            const Room& neighbor = graph->getRoom(connection.roomId);
            float dx = neighbor.position.x - room.position.x;
            float dy = neighbor.position.y - room.position.y;
            float length = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
            sf::RectangleShape line({length, 1.f});
            line.setOrigin({0.f, 1.f});
            line.setPosition(room.position);
            line.setRotation(sf::degrees(angle));
            bool isBfsEdge = false;
            bool isDfsEdge = false;
            bool isDijkstraEdge = false;
            // Проверяем путь BFS
            for (size_t j = 0; j + 1 < bfsPath.size(); j++) {
                if ((bfsPath[j] == room.id && bfsPath[j + 1] == neighbor.id) ||
                    (bfsPath[j] == neighbor.id && bfsPath[j + 1] == room.id)) {
                    isBfsEdge = true;
                    break;
                }
            }
            // Проверяем путь DFS
            for (size_t i = 0; i + 1 < dfsPath.size(); ++i) {
                if ((dfsPath[i] == room.id && dfsPath[i + 1] == neighbor.id) ||
                    (dfsPath[i] == neighbor.id && dfsPath[i + 1] == room.id)) {
                    isDfsEdge = true;
                    break;
                }
            }
            // Проверяем путь Дейкстры
            for (size_t j = 0; j + 1 < dijPath.size(); j++) {
                if ((dijPath[j] == room.id && dijPath[j + 1] == neighbor.id) ||
                    (dijPath[j] == neighbor.id && dijPath[j + 1] == room.id)) {
                    isDijkstraEdge = true;
                    break;
                }
            }

            // Выбираем цвет связи
            if (isBfsEdge) {
                line.setFillColor(sf::Color::Yellow);
            }
            else if (isDfsEdge) {
                line.setFillColor(sf::Color::Red);
            }
            else if (isDijkstraEdge) {
                line.setFillColor(sf::Color(255, 140, 0));
            }
            else {
                line.setFillColor(sf::Color(130, 130, 140));
            }

            window.draw(line);
            sf::Text weightText(font, std::to_string(connection.weight), 12);
            weightText.setFillColor(sf::Color::White);

            // Находим середину линии
            float midX = (room.position.x + neighbor.position.x) / 2.f;
            float midY = (room.position.y + neighbor.position.y) / 2.f;

            // Вычисляем направление перпендикулярно линии
            float offsetX = -dy / length * 12.f;
            float offsetY = dx / length * 12.f;

            // Смещаем текст относительно центра линии
            weightText.setOrigin(weightText.getLocalBounds().size / 2.f);
            weightText.setPosition({
                midX + offsetX,
                midY + offsetY
            });

            window.draw(weightText);
        }
    }

    // Рисуем комнаты
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);

        // Тень комнаты
        sf::CircleShape shadow(14.f);
        shadow.setOrigin({14.f, 14.f});
        shadow.setPosition(room.position + sf::Vector2f(2.f, 3.f));
        shadow.setFillColor(sf::Color(10, 10, 15, 150));
        window.draw(shadow);

        // Основная комната
        sf::CircleShape circle(14.f);
        circle.setOrigin({14.f, 14.f});
        circle.setPosition(room.position);
        circle.setFillColor(sf::Color(55, 145, 210));
        circle.setOutlineThickness(2.f);
        circle.setOutlineColor(sf::Color(170, 215, 245));
        window.draw(circle);

        // Номер комнаты
        sf::Text number(font, std::to_string(room.id), 11);
        number.setFillColor(sf::Color::White);
        number.setOrigin(number.getLocalBounds().size / 2.f);
        number.setPosition({
            room.position.x - 0.5f,
            room.position.y - 2.f
        });
        window.draw(number);
    }

    // Возвращаем обычную камеру окна
    window.setView(window.getDefaultView());

    // Рисуем панели
    window.draw(controlPanel);
    window.draw(bfsButton);
    window.draw(dfsButton);
    window.draw(dijkstraButton);
    window.draw(generateButton);

    // Рисуем поля ввода
    window.draw(startInput);
    window.draw(endInput);

    window.draw(startLabel);
    window.draw(endLabel);

    window.draw(startValue);
    window.draw(endValue);

    // Нижняя информационная панель
    window.draw(infoPanel);

    if (showDungeonInfo) {
        // Режим информации о подземелье
        sf::Text panelTitle(font, "DUNGEON INFORMATION", 18);
        panelTitle.setFillColor(sf::Color::White);
        panelTitle.setPosition({35.f, 840.f});
        window.draw(panelTitle);

        if (graph != nullptr) {
            int roomCount = graph->getRoomCount();
            int connectionCount = 0;

            for (int i = 0; i < roomCount; i++) {
                connectionCount += graph->getRoom(i).connectedRooms.size();
            }

            connectionCount /= 2;

            float averageDegree = 0.f;

            if (roomCount > 0) {
                averageDegree = 2.f * connectionCount / roomCount;
            }

            std::ostringstream text;
            text << "Rooms: " << roomCount
                << "    Connections: " << connectionCount
                << "    Average degree: " << averageDegree;

            sf::Text dungeonSummary(font, text.str(), 14);
            dungeonSummary.setFillColor(sf::Color(200, 200, 210));
            dungeonSummary.setPosition({35.f, 875.f});

            window.draw(dungeonSummary);
        }
    }
    else {
        infoTitle.setString("ALGORITHM RESULTS");
        window.draw(infoTitle);

        sf::Text bfsInfo(font, "BFS: " + bfsResult, 13);
        bfsInfo.setFillColor(sf::Color::Yellow);
        bfsInfo.setPosition({35.f, 865.f});
        window.draw(bfsInfo);

        sf::Text dfsInfo(font, "DFS: " + dfsResult, 13);
        dfsInfo.setFillColor(sf::Color::Red);
        dfsInfo.setPosition({35.f, 885.f});
        window.draw(dfsInfo);

        sf::Text dijkstraInfo(font, "DIJKSTRA: " + dijkstraResult, 13);
        dijkstraInfo.setFillColor(sf::Color(255, 140, 0));
        dijkstraInfo.setPosition({35.f, 905.f});
        window.draw(dijkstraInfo);
    }

    // Кнопка переключения
    window.draw(infoToggleButton);

    sf::Text toggleText(
        font,
        showDungeonInfo ? "ALGORITHMS" : "DUNGEON INFO",
        12
    );

    toggleText.setFillColor(sf::Color::White);
    toggleText.setOrigin(toggleText.getLocalBounds().size / 2.f);
    toggleText.setPosition({977.5f, 875.f});

    window.draw(toggleText);

    // Заголовок
    sf::Text title(font, "ALGORITHMS", 16);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().size / 2.f);
    title.setPosition({959.5f, 75.f});
    window.draw(title);

    // Надпись BFS
    sf::Text bfsText(font, "BFS", 16);
    bfsText.setFillColor(sf::Color::White);
    bfsText.setOrigin(bfsText.getLocalBounds().size / 2.f);
    bfsText.setPosition({959.5f, 152.5f});
    window.draw(bfsText);

    // Надпись DFS
    sf::Text dfsText(font, "DFS", 16);
    dfsText.setFillColor(sf::Color::White);
    dfsText.setOrigin(dfsText.getLocalBounds().size / 2.f);
    dfsText.setPosition({959.5f, 217.5f});
    window.draw(dfsText);

    // Надпись Dijkstra
    sf::Text dijkstraText(font, "DIJKSTRA", 14);
    dijkstraText.setFillColor(sf::Color::White);
    dijkstraText.setOrigin(dijkstraText.getLocalBounds().size / 2.f);
    dijkstraText.setPosition({959.5f, 282.5f});
    window.draw(dijkstraText);

    // Надпись Generate
    sf::Text generateText(font, "NEW DUNGEON", 12);
    generateText.setFillColor(sf::Color::White);
    generateText.setOrigin(generateText.getLocalBounds().size / 2.f);
    generateText.setPosition({959.5f, 515.f});
    window.draw(generateText);
}


void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    // Масштабирование графа колесиком мыши
    if (const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {

        sf::Vector2f beforePos =
            window.mapPixelToCoords(mouse->position, dungeonCamera);

        sf::Vector2f cameraSize = dungeonCamera.getSize();

        if (mouse->delta > 0 && cameraSize.x > 275.f) {
            dungeonCamera.zoom(0.9f);
        }
        else if (mouse->delta < 0 && cameraSize.x < 825.f) {
            dungeonCamera.zoom(1.1f);
        }

        sf::Vector2f afterPos =
            window.mapPixelToCoords(mouse->position, dungeonCamera);

        dungeonCamera.move(beforePos - afterPos);
    }

    // Нажатие мыши
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                mouse->position,
                window.getDefaultView()
            );
            // Переключение нижней информационной панели
            if (infoToggleButton.getGlobalBounds().contains(mousePos)) {
                showDungeonInfo = !showDungeonInfo;
                return;
            }
            // Выбираем поле ввода
            startInputActive = startInput.getGlobalBounds().contains(mousePos);
            endInputActive = endInput.getGlobalBounds().contains(mousePos);
            if (graph != nullptr) {
                if(generateButton.getGlobalBounds().contains(mousePos)){
                    graph->clear();
                    graph->GenerationRooms();
                    graph->generateConnections();
                    dungeonInfoText.setString(graph->PrintGraph());
                    bfsPath.clear();
                    dfsPath.clear();
                    dijPath.clear();
                    bfsResult = "Not calculated";
                    dfsResult = "Not calculated";
                    dijkstraResult = "Not calculated";
                    infoTitle.setString("NEW DUNGEON");
                    infoContent.setString("A new dungeon has been generated");
                }
                // Проверяем, нажата ли кнопка алгоритма
                bool algorithmButtonPressed =
                    bfsButton.getGlobalBounds().contains(mousePos) ||
                    dfsButton.getGlobalBounds().contains(mousePos) ||
                    dijkstraButton.getGlobalBounds().contains(mousePos);
                if (algorithmButtonPressed) {
                    // Проверяем пустые поля
                    if (startInputText.empty() || endInputText.empty()) {
                        infoTitle.setString("Input error");
                        infoContent.setString("Please enter both room IDs");
                        return;
                    }
                    int startRoom = std::stoi(startInputText);
                    int endRoom = std::stoi(endInputText);
                    auto makePathString = [](const std::vector<int>& path) {
                        std::ostringstream result;

                        for (size_t i = 0; i < path.size(); i++) {
                            if (i > 0) {
                                result << " -> ";
                            }

                            result << path[i];
                        }

                        if (path.empty()) {
                            return std::string("Path not found");
                        }

                        return result.str();
                    };
                    // Проверяем существование комнат
                    if (startRoom >= graph->getRoomCount() ||
                        endRoom >= graph->getRoomCount()) {
                        infoTitle.setString("Input error");
                        infoContent.setString("Room ID does not exist");
                        return;
                    }
                    // BFS
                    if (bfsButton.getGlobalBounds().contains(mousePos)) {
                        bfsPath = graph->BFS(startRoom, endRoom);

                        if (bfsPath.empty()) {
                            bfsResult = "No path found";
                        }
                        else {
                            bfsResult = makePathString(bfsPath);
                            bfsResult += " | Rooms: " + std::to_string(bfsPath.size());
                        }
                        dfsPath.clear();
                        dijPath.clear();
                    }
                    // DFS
                    if (dfsButton.getGlobalBounds().contains(mousePos)) {
                        dfsPath = graph->DFS(startRoom, endRoom);

                        if (dfsPath.empty()) {
                            dfsResult = "No path found";
                        }
                        else {
                            dfsResult = makePathString(dfsPath);
                            dfsResult += " | Rooms: " + std::to_string(dfsPath.size());
                        }
                        bfsPath.clear();
                        dijPath.clear();
                    }
                    // Dijkstra
                    if (dijkstraButton.getGlobalBounds().contains(mousePos)) {
                        dijPath = graph->Dijkstra(startRoom, endRoom);

                        if (dijPath.empty()) {
                            dijkstraResult = "No path found";
                        }
                        else {
                            int totalCost = 0;

                            // Считаем стоимость всех соединений маршрута
                            for (int i = 0; i < dijPath.size() - 1; i++) {
                                const Room& room = graph->getRoom(dijPath[i]);

                                for (const Connection& connection : room.connectedRooms) {
                                    if (connection.roomId == dijPath[i + 1]) {
                                        totalCost += connection.weight;
                                        break;
                                    }
                                }
                            }

                            dijkstraResult = makePathString(dijPath);
                            dijkstraResult += " | Cost: " + std::to_string(totalCost);
                            dijkstraResult += " | Rooms: " + std::to_string(dijPath.size());
                        }
                        bfsPath.clear();
                        dfsPath.clear();
                    }
                }
            }
        }
    }

    // Ввод текста с клавиатуры
    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        // Ввод цифр
        if (text->unicode >= '0' && text->unicode <= '9') {
            if (startInputActive && startInputText.size() < 2) {
                startInputText += static_cast<char>(text->unicode);
                startValue.setString(startInputText);
            }
            else if (endInputActive && endInputText.size() < 2) {
                endInputText += static_cast<char>(text->unicode);
                endValue.setString(endInputText);
            }
        }
        // Удаление через Backspace
        if (text->unicode == 8) {
            if (startInputActive && !startInputText.empty()) {
                startInputText.pop_back();
                startValue.setString(startInputText);
            }
            else if (endInputActive && !endInputText.empty()) {
                endInputText.pop_back();
                endValue.setString(endInputText);
            }
        }
    }
}


void Game::setGraph(Graph& graph)
{
    this->graph = &graph;
    dungeonInfoText.setString(this->graph->PrintGraph());
}