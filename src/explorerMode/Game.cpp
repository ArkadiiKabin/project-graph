#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

#include "Game.hpp"

using namespace std;

void centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});

    sf::Vector2f buttonPosition = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    text.setPosition({buttonPosition.x + buttonSize.x / 2.f, buttonPosition.y + buttonSize.y / 2.f});
}

// конструктор игрового интерфейса
Game::Game()
    : settingsWindow(font), startLabel(font), endLabel(font), startValue(font), endValue(font),
      infoTitle(font), settingsButtonText(font), dungeonInfoText(font), dungeonRenderer(font) {

    // загрузка шрифта
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) { return; }
    settingsWindow.updateTextLayout();

    // левая панель
    dungeonArea.setSize({800.f, 800.f});
    dungeonArea.setPosition({20.f, 20.f});
    dungeonArea.setFillColor(sf::Color(35, 35, 40));

    // правая панель
    controlPanel.setSize({240.f, 800.f});
    controlPanel.setPosition({840.f, 20.f});
    controlPanel.setFillColor(sf::Color(35, 35, 45));

    // кнопка bfs
    bfsButton.setSize({155.f, 45.f});
    bfsButton.setPosition({882.f, 130.f});
    bfsButton.setFillColor(sf::Color(65, 65, 80));

    // кнопка dfs
    dfsButton.setSize({155.f, 45.f});
    dfsButton.setPosition({882.f, 195.f});
    dfsButton.setFillColor(sf::Color(65, 65, 80));

    // кнопка алгоритма дейкстры
    dijkstraButton.setSize({155.f, 45.f});
    dijkstraButton.setPosition({882.f, 260.f});
    dijkstraButton.setFillColor(sf::Color(65, 65, 80));

    // поле начальной комнаты
    startInput.setSize({155.f, 35.f});
    startInput.setPosition({882.f, 345.f});
    startInput.setFillColor(sf::Color(50, 50, 65));

    // поле конечной комнаты
    endInput.setSize({155.f, 35.f});
    endInput.setPosition({882.f, 420.f});
    endInput.setFillColor(sf::Color(50, 50, 65));

    // кнопка генерации подземелья
    generateButton.setSize({155.f, 40.f});
    generateButton.setPosition({882.f, 495.f});
    generateButton.setFillColor(sf::Color(65, 100, 80));

    // кнопка полной информации
    fullInfoButton.setSize({155.f, 40.f});
    fullInfoButton.setPosition({882.f, 565.f});
    fullInfoButton.setFillColor(sf::Color(65, 85, 105));

    // подписи полей ввода
    startLabel.setString("Start room:");
    startLabel.setCharacterSize(14);
    startLabel.setPosition({882.f, 320.f});

    endLabel.setString("End room:");
    endLabel.setCharacterSize(14);
    endLabel.setPosition({882.f, 395.f});

    // значения полей ввода
    startValue.setString(startInputText);
    startValue.setCharacterSize(16);
    startValue.setPosition({892.f, 350.f});

    endValue.setString(endInputText);
    endValue.setCharacterSize(16);
    endValue.setPosition({892.f, 425.f});

    // нижняя информационная панель
    infoPanel.setSize({1060.f, 100.f});
    infoPanel.setPosition({20.f, 830.f});
    infoPanel.setFillColor(sf::Color(35, 35, 45));

    // заголовок информационной панели
    infoTitle.setCharacterSize(18);
    infoTitle.setFillColor(sf::Color::White);
    infoTitle.setPosition({35.f, 840.f});
    infoTitle.setString("ALGORITHM RESULTS");

    // кнопка переключения информационной панели
    infoToggleButton.setSize({140.f, 28.f});
    infoToggleButton.setPosition({900.f, 835.f});
    infoToggleButton.setFillColor(sf::Color(65, 85, 105));

    dungeonInfoText.setCharacterSize(11);
    dungeonInfoText.setFillColor(sf::Color(200, 200, 210));
    dungeonInfoText.setPosition({35.f, 925.f});

    // настройка камеры подземелья
    dungeonCamera.setSize({800.f, 800.f});
    dungeonCamera.setCenter({420.f, 420.f});
    dungeonCamera.setViewport(sf::FloatRect({0.01818f, 0.02105f}, {0.72727f, 0.8421f}));
    // кнопка настроек
    settingsButton.setSize({155.f, 40.f});
    settingsButton.setPosition({882.f, 65.f});
    settingsButton.setFillColor(sf::Color(85, 85, 100));

    settingsButtonText.setString("SETTINGS");
    settingsButtonText.setCharacterSize(13);
    settingsButtonText.setFillColor(sf::Color::White);
    centerTextInButton(settingsButtonText, settingsButton);
}

// отрисовка игрового интерфейса и подземелья
void Game::draw(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(dungeonArea);

    window.setView(dungeonCamera);

    // отрисовка подземелья
    if (graph != nullptr) {
        dungeonRenderer.draw(window, *graph, bfsPath, dfsPath, dijPath);
    }
    
    // возврат к стандартному виду окна
    window.setView(window.getDefaultView());

    // отрисовка правой панели
    window.draw(controlPanel);
    window.draw(bfsButton);
    window.draw(dfsButton);
    window.draw(dijkstraButton);
    window.draw(generateButton);
    window.draw(fullInfoButton);

    // получение положения мыши
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

    // изменение цвета полей ввода при наведении
    startInput.setFillColor(startInput.getGlobalBounds().contains(mousePos) ? sf::Color(75, 75, 95)
                                                                            : sf::Color(50, 50, 65));

    endInput.setFillColor(endInput.getGlobalBounds().contains(mousePos) ? sf::Color(75, 75, 95)
                                                                        : sf::Color(50, 50, 65));

    // подсветка активного поля ввода
    startInput.setOutlineThickness(startInputActive ? 2.f : 0.f);
    startInput.setOutlineColor(sf::Color::Cyan);

    endInput.setOutlineThickness(endInputActive ? 2.f : 0.f);
    endInput.setOutlineColor(sf::Color::Cyan);

    // отрисовка полей ввода и подписей
    window.draw(startInput);
    window.draw(endInput);
    window.draw(startLabel);
    window.draw(endLabel);
    window.draw(startValue);
    window.draw(endValue);

    // отрисовка нижней информационной панели
    window.draw(infoPanel);

    if (showDungeonInfo) {
        // отображение информации о подземелье
        sf::Text panelTitle(font, "DUNGEON INFORMATION", 18);

        panelTitle.setFillColor(sf::Color::White);
        panelTitle.setPosition({35.f, 840.f});

        window.draw(panelTitle);

        if (graph != nullptr) {
            int roomCount = graph->getRoomCount();
            int connectionCount = 0;

            for (int i = 0; i < roomCount; i++) { connectionCount += graph->getRoom(i).connectedRooms.size(); }

            connectionCount /= 2;

            float averageDegree = 0.f;

            if (roomCount > 0) { averageDegree = 2.f * connectionCount / roomCount; }

            ostringstream text;

            text << "Rooms: " << roomCount << "    Connections: " << connectionCount
                 << "    Average degree: " << averageDegree;

            sf::Text dungeonSummary(font, text.str(), 14);

            dungeonSummary.setFillColor(sf::Color(200, 200, 210));
            dungeonSummary.setPosition({35.f, 875.f});

            window.draw(dungeonSummary);
        }
    } else {
        // отображение результатов алгоритмов
        infoTitle.setString("ALGORITHM RESULTS");
        window.draw(infoTitle);

        sf::Text bfsInfo(font, "BFS: " + bfsResult, 11);
        bfsInfo.setFillColor(sf::Color::Yellow);
        bfsInfo.setPosition({35.f, 865.f});
        window.draw(bfsInfo);

        sf::Text dfsInfo(font, "DFS: " + dfsResult, 11);
        dfsInfo.setFillColor(sf::Color::Red);
        dfsInfo.setPosition({35.f, 882.f});
        window.draw(dfsInfo);

        sf::Text dijkstraInfo(font, "DIJKSTRA: " + dijkstraResult, 11);
        dijkstraInfo.setFillColor(sf::Color(255, 140, 0));
        dijkstraInfo.setPosition({35.f, 899.f});
        window.draw(dijkstraInfo);
    }

    // кнопка переключения информационной панели
    window.draw(infoToggleButton);

    sf::Text toggleText(font, showDungeonInfo ? "ALGORITHMS" : "DUNGEON INFO", 11);
    toggleText.setFillColor(sf::Color::White);
    toggleText.setOrigin(toggleText.getLocalBounds().size / 2.f);
    toggleText.setPosition({970.f, 849.f});
    window.draw(toggleText);

    // заголовок правой панели
    sf::Text title(font, "ALGORITHMS", 16);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().size / 2.f);
    title.setPosition({959.5f, 75.f});
    window.draw(title);

    // подпись кнопки bfs
    sf::Text bfsText(font, "BFS", 16);
    bfsText.setFillColor(sf::Color::White);
    bfsText.setOrigin(bfsText.getLocalBounds().size / 2.f);
    bfsText.setPosition({959.5f, 152.5f});
    window.draw(bfsText);

    // подпись кнопки dfs
    sf::Text dfsText(font, "DFS", 16);
    dfsText.setFillColor(sf::Color::White);
    dfsText.setOrigin(dfsText.getLocalBounds().size / 2.f);
    dfsText.setPosition({959.5f, 217.5f});
    window.draw(dfsText);

    // подпись кнопки дейкстры
    sf::Text dijkstraText(font, "DIJKSTRA", 14);
    dijkstraText.setFillColor(sf::Color::White);
    dijkstraText.setOrigin(dijkstraText.getLocalBounds().size / 2.f);
    dijkstraText.setPosition({959.5f, 282.5f});
    window.draw(dijkstraText);

    // подпись кнопки генерации
    sf::Text generateText(font, "NEW DUNGEON", 12);
    generateText.setFillColor(sf::Color::White);
    generateText.setOrigin(generateText.getLocalBounds().size / 2.f);
    generateText.setPosition({959.5f, 515.f});
    window.draw(generateText);

    // подпись кнопки полной информации
    sf::Text fullInfoText(font, "FULL INFO", 12);
    fullInfoText.setFillColor(sf::Color::White);
    fullInfoText.setOrigin(fullInfoText.getLocalBounds().size / 2.f);
    fullInfoText.setPosition({959.5f, 585.f});
    window.draw(fullInfoText);

    // легенда цветов маршрутов
    sf::Text legendTitle(font, "PATH COLORS", 14);
    legendTitle.setFillColor(sf::Color::White);
    legendTitle.setPosition({885.f, 630.f});
    window.draw(legendTitle);

    // цвет маршрута bfs
    sf::CircleShape bfsLegend(5.f);
    bfsLegend.setFillColor(sf::Color::Yellow);
    bfsLegend.setPosition({885.f, 660.f});
    window.draw(bfsLegend);

    sf::Text bfsLegendText(font, "BFS", 12);
    bfsLegendText.setFillColor(sf::Color::White);
    bfsLegendText.setPosition({905.f, 656.f});
    window.draw(bfsLegendText);

    // цвет маршрута dfs
    sf::CircleShape dfsLegend(5.f);
    dfsLegend.setFillColor(sf::Color::Red);
    dfsLegend.setPosition({885.f, 685.f});
    window.draw(dfsLegend);

    sf::Text dfsLegendText(font, "DFS", 12);
    dfsLegendText.setFillColor(sf::Color::White);
    dfsLegendText.setPosition({905.f, 681.f});
    window.draw(dfsLegendText);

    // цвет маршрута дейкстры
    sf::CircleShape dijkstraLegend(5.f);
    dijkstraLegend.setFillColor(sf::Color(255, 140, 0));
    dijkstraLegend.setPosition({885.f, 710.f});
    window.draw(dijkstraLegend);

    sf::Text dijkstraLegendText(font, "DIJKSTRA", 12);
    dijkstraLegendText.setFillColor(sf::Color::White);
    dijkstraLegendText.setPosition({905.f, 706.f});
    window.draw(dijkstraLegendText);

    // окно полной информации
    if (showInfoWindow && infoWindow.isOpen()) {
        // обработка событий отдельного окна
        while (auto event = infoWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                infoWindow.close();
                showInfoWindow = false;
            }
        }

        infoWindow.clear(sf::Color(35, 35, 45));

        // заголовок окна
        sf::Text infoWindowTitle(font, "DUNGEON INFORMATION", 22);
        infoWindowTitle.setFillColor(sf::Color::White);
        infoWindowTitle.setPosition({20.f, 20.f});
        infoWindow.draw(infoWindowTitle);

        // информация о комнатах
        dungeonInfoText.setPosition({20.f, 70.f});
        infoWindow.draw(dungeonInfoText);

        infoWindow.display();
    }

    window.draw(settingsButton);
    window.draw(settingsButtonText);

    // окно настроек
    settingsWindow.draw(window);
}

// обработка событий окна
void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    // обработка событий окна настроек
    if (settingsWindow.isOpen()) {
        if (settingsWindow.handleEvent(window, event)) {
            generationSettings = settingsWindow.getSettings();

            if (graph != nullptr) {
                graph->setSettings(generationSettings);
            }
        }

        return;
    }
    // масштабирование подземелья колесиком мыши
    if (const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position, window.getDefaultView());

        // масштабирование только над областью подземелья
        if (dungeonArea.getGlobalBounds().contains(mousePos)) {
            sf::Vector2f beforePos = window.mapPixelToCoords(mouse->position, dungeonCamera);
            sf::Vector2f cameraSize = dungeonCamera.getSize();

            if (mouse->delta > 0 && cameraSize.x > 275.f) {
                dungeonCamera.zoom(0.9f);
            } else if (mouse->delta < 0 && cameraSize.x < 3000.f) {
                dungeonCamera.zoom(1.1f);
            }

            sf::Vector2f afterPos = window.mapPixelToCoords(mouse->position, dungeonCamera);
            dungeonCamera.move(beforePos - afterPos);
        }
    }

    // обработка нажатия кнопки мыши
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position, window.getDefaultView());

            // открытие окна настроек
            if (settingsButton.getGlobalBounds().contains(mousePos)) {
                settingsWindow.open();
                return;
            }

            // переключение нижней информационной панели
            if (infoToggleButton.getGlobalBounds().contains(mousePos)) {
                showDungeonInfo = !showDungeonInfo;
                return;
            }

            // открытие окна полной информации
            if (fullInfoButton.getGlobalBounds().contains(mousePos)) {
                showInfoWindow = true;
                infoWindow.create(sf::VideoMode({600, 700}), "Dungeon Information");
                return;
            }

            // определение активного поля ввода
            startInputActive = startInput.getGlobalBounds().contains(mousePos);
            endInputActive = endInput.getGlobalBounds().contains(mousePos);

            if (graph != nullptr) {
                // генерация нового подземелья
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    graph->clear();
                    graph->setSettings(generationSettings);
                    graph->GenerationRooms();
                    graph->generateConnections();

                    dungeonRenderer.fitToView(*graph, dungeonCamera);
                    dungeonInfoText.setString(graph->PrintGraph());

                    bfsPath.clear();
                    dfsPath.clear();
                    dijPath.clear();

                    bfsResult = "Not calculated";
                    dfsResult = "Not calculated";
                    dijkstraResult = "Not calculated";

                    infoTitle.setString("NEW DUNGEON");
                }

                // проверка нажатия кнопки алгоритма
                bool algorithmButtonPressed = bfsButton.getGlobalBounds().contains(mousePos) ||
                                              dfsButton.getGlobalBounds().contains(mousePos) ||
                                              dijkstraButton.getGlobalBounds().contains(mousePos);

                if (algorithmButtonPressed) {
                    // проверка заполнения полей ввода
                    if (startInputText.empty() || endInputText.empty()) {
                        infoTitle.setString("Input error");
                        return;
                    }

                    int startRoom = stoi(startInputText);
                    int endRoom = stoi(endInputText);

                    // формирование строки маршрута
                    auto makePathString = [](const vector<int>& path) {
                        ostringstream result;

                        for (size_t i = 0; i < path.size(); i++) {
                            if (i > 0) { result << " -> "; }
                            result << path[i];
                        }

                        if (path.empty()) { return string("Path not found"); }

                        return result.str();
                    };

                    // проверка существования комнат
                    if (startRoom < 0 || endRoom < 0 || startRoom >= graph->getRoomCount() ||
                        endRoom >= graph->getRoomCount()) {
                        bfsResult = "Room ID does not exist";
                        dfsResult = "Room ID does not exist";
                        dijkstraResult = "Room ID does not exist";
                        return;
                    }

                    // поиск в ширину
                    if (bfsButton.getGlobalBounds().contains(mousePos)) {
                        bfsPath = graph->BFS(startRoom, endRoom);

                        if (bfsPath.empty()) {
                            bfsResult = "No path found";
                        } else {
                            bfsResult = makePathString(bfsPath);
                            bfsResult += " | Rooms: " + to_string(bfsPath.size()) +
                                         " | Visited: " + to_string(graph->getLastVisitedCount());
                        }

                        dfsPath.clear();
                        dijPath.clear();
                    }

                    // поиск в глубину
                    if (dfsButton.getGlobalBounds().contains(mousePos)) {
                        dfsPath = graph->DFS(startRoom, endRoom);

                        if (dfsPath.empty()) {
                            dfsResult = "No path found";
                        } else {
                            dfsResult = makePathString(dfsPath);
                            dfsResult += " | Rooms: " + to_string(dfsPath.size()) +
                                         " | Visited: " + to_string(graph->getLastVisitedCount());
                        }

                        bfsPath.clear();
                        dijPath.clear();
                    }

                    // алгоритм дейкстры
                    if (dijkstraButton.getGlobalBounds().contains(mousePos)) {
                        dijPath = graph->Dijkstra(startRoom, endRoom);

                        if (dijPath.empty()) {
                            dijkstraResult = "No path found";
                        } else {
                            int totalCost = 0;

                            // подсчёт стоимости маршрута
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
                            dijkstraResult += " | Cost: " + to_string(totalCost);
                            dijkstraResult += " | Rooms: " + to_string(dijPath.size());
                        }

                        bfsPath.clear();
                        dfsPath.clear();
                    }
                }
            }
        }
    }

    // обработка ввода текста с клавиатуры
    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        // ввод числовых значений
        if (text->unicode >= '0' && text->unicode <= '9') {
            char digit = static_cast<char>(text->unicode);

            if (startInputActive && startInputText.size() < 2) {
                startInputText += digit;
                startValue.setString(startInputText);
            } else if (endInputActive && endInputText.size() < 2) {
                endInputText += digit;
                endValue.setString(endInputText);
            }
        }

        // удаление символов клавишей backspace
        if (text->unicode == 8) {
            if (startInputActive && !startInputText.empty()) {
                startInputText.pop_back();
                startValue.setString(startInputText);
            } else if (endInputActive && !endInputText.empty()) {
                endInputText.pop_back();
                endValue.setString(endInputText);
            }
        }
    }
}

// передача графа игровому классу
void Game::setGraph(Graph& graph) {
    this->graph = &graph;

    dungeonInfoText.setString(this->graph->PrintGraph());

    dungeonRenderer.fitToView(*this->graph, dungeonCamera);
}