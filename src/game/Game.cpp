#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

#include "Game.hpp"

// Подгоняет камеру под размеры подземелья.
void fitDungeonToView(const Graph& graph, sf::View& camera) {
    if (graph.getRoomCount() == 0) {
        return;
    }

    const float roomSize = 128.f;
    const float padding = 40.f;

    const Room& firstRoom = graph.getRoom(0);

    float minX = firstRoom.position.x;
    float maxX = firstRoom.position.x;
    float minY = firstRoom.position.y;
    float maxY = firstRoom.position.y;

    for (int i = 1; i < graph.getRoomCount(); ++i) {
        const Room& room = graph.getRoom(i);

        minX = std::min(minX, room.position.x);
        maxX = std::max(maxX, room.position.x);
        minY = std::min(minY, room.position.y);
        maxY = std::max(maxY, room.position.y);
    }

    float width = maxX - minX + roomSize + padding * 2.f;
    float height = maxY - minY + roomSize + padding * 2.f;

    float viewSize = std::max(width, height);

    camera.setSize({viewSize, viewSize});
    camera.setCenter({
        (minX + maxX) / 2.f,
        (minY + maxY) / 2.f
    });
}

// Конструктор игрового интерфейса.
Game::Game()
    : infoTitle(font),
      infoContent(font),
      startLabel(font),
      endLabel(font),
      startValue(font),
      endValue(font),
      dungeonInfoText(font) {

    // Загрузка шрифта.
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) {
        return;
    }

    // Загрузка текстур дорог.
    roadTexturesLoaded =
        blueRoadTexture.loadFromFile("assets/textures/roads/blue_road.png") &&
        greenRoadTexture.loadFromFile("assets/textures/roads/green_road.png") &&
        brownRoadTexture.loadFromFile("assets/textures/roads/brown_road.png") &&
        purpleRoadTexture.loadFromFile("assets/textures/roads/purple_road.png") &&
        grayRoadTexture.loadFromFile("assets/textures/roads/gray_road.png");

    // Отключение сглаживания текстур дорог.
    blueRoadTexture.setSmooth(false);
    greenRoadTexture.setSmooth(false);
    brownRoadTexture.setSmooth(false);
    purpleRoadTexture.setSmooth(false);
    grayRoadTexture.setSmooth(false);

    // Загрузка текстур маршрутов.
    pathTexturesLoaded =
        yellowPathTexture.loadFromFile("assets/textures/roads/yellow_path.png") &&
        redPathTexture.loadFromFile("assets/textures/roads/red_path.png") &&
        orangePathTexture.loadFromFile("assets/textures/roads/orange_path.png");

    yellowPathTexture.setSmooth(false);
    redPathTexture.setSmooth(false);
    orangePathTexture.setSmooth(false);

    // Загрузка первого набора текстур комнат.
    roomTexturesLoaded =
        roomTexture.loadFromFile("assets/textures/room_1/room.png") &&
        topTexture.loadFromFile("assets/textures/room_1/top.png") &&
        leftTexture.loadFromFile("assets/textures/room_1/left.png") &&
        downTexture.loadFromFile("assets/textures/room_1/down.png") &&
        rightTexture.loadFromFile("assets/textures/room_1/right.png");

    // Загрузка второго набора текстур комнат.
    roomTextures2Loaded =
        roomTexture2.loadFromFile("assets/textures/room_2/room.png") &&
        topTexture2.loadFromFile("assets/textures/room_2/top.png") &&
        leftTexture2.loadFromFile("assets/textures/room_2/left.png") &&
        downTexture2.loadFromFile("assets/textures/room_2/down.png") &&
        rightTexture2.loadFromFile("assets/textures/room_2/right.png");

    // Загрузка третьего набора текстур комнат.
    roomTextures3Loaded =
        roomTexture3.loadFromFile("assets/textures/room_3/room.png") &&
        topTexture3.loadFromFile("assets/textures/room_3/top.png") &&
        leftTexture3.loadFromFile("assets/textures/room_3/left.png") &&
        downTexture3.loadFromFile("assets/textures/room_3/down.png") &&
        rightTexture3.loadFromFile("assets/textures/room_3/right.png");

    // Загрузка четвёртого набора текстур комнат.
    roomTextures4Loaded =
        roomTexture4.loadFromFile("assets/textures/room_4/room.png") &&
        topTexture4.loadFromFile("assets/textures/room_4/top.png") &&
        leftTexture4.loadFromFile("assets/textures/room_4/left.png") &&
        downTexture4.loadFromFile("assets/textures/room_4/down.png") &&
        rightTexture4.loadFromFile("assets/textures/room_4/right.png");

    // Загрузка пятого набора текстур комнат.
    roomTextures5Loaded =
        roomTexture5.loadFromFile("assets/textures/room_5/room.png") &&
        topTexture5.loadFromFile("assets/textures/room_5/top.png") &&
        leftTexture5.loadFromFile("assets/textures/room_5/left.png") &&
        downTexture5.loadFromFile("assets/textures/room_5/down.png") &&
        rightTexture5.loadFromFile("assets/textures/room_5/right.png");

    // Отключение сглаживания текстур комнат.
    roomTexture.setSmooth(false);
    topTexture.setSmooth(false);
    leftTexture.setSmooth(false);
    downTexture.setSmooth(false);
    rightTexture.setSmooth(false);

    roomTexture2.setSmooth(false);
    topTexture2.setSmooth(false);
    leftTexture2.setSmooth(false);
    downTexture2.setSmooth(false);
    rightTexture2.setSmooth(false);

    roomTexture3.setSmooth(false);
    topTexture3.setSmooth(false);
    leftTexture3.setSmooth(false);
    downTexture3.setSmooth(false);
    rightTexture3.setSmooth(false);

    roomTexture4.setSmooth(false);
    topTexture4.setSmooth(false);
    leftTexture4.setSmooth(false);
    downTexture4.setSmooth(false);
    rightTexture4.setSmooth(false);

    roomTexture5.setSmooth(false);
    topTexture5.setSmooth(false);
    leftTexture5.setSmooth(false);
    downTexture5.setSmooth(false);
    rightTexture5.setSmooth(false);

    // Левая панель.
    dungeonArea.setSize({800.f, 800.f});
    dungeonArea.setPosition({20.f, 20.f});
    dungeonArea.setFillColor(sf::Color(35, 35, 40));

    // Правая панель.
    controlPanel.setSize({240.f, 800.f});
    controlPanel.setPosition({840.f, 20.f});
    controlPanel.setFillColor(sf::Color(35, 35, 45));

    // Кнопка BFS.
    bfsButton.setSize({155.f, 45.f});
    bfsButton.setPosition({882.f, 130.f});
    bfsButton.setFillColor(sf::Color(65, 65, 80));

    // Кнопка DFS.
    dfsButton.setSize({155.f, 45.f});
    dfsButton.setPosition({882.f, 195.f});
    dfsButton.setFillColor(sf::Color(65, 65, 80));

    // Кнопка алгоритма Дейкстры.
    dijkstraButton.setSize({155.f, 45.f});
    dijkstraButton.setPosition({882.f, 260.f});
    dijkstraButton.setFillColor(sf::Color(65, 65, 80));

    // Поле начальной комнаты.
    startInput.setSize({155.f, 35.f});
    startInput.setPosition({882.f, 345.f});
    startInput.setFillColor(sf::Color(50, 50, 65));

    // Поле конечной комнаты.
    endInput.setSize({155.f, 35.f});
    endInput.setPosition({882.f, 420.f});
    endInput.setFillColor(sf::Color(50, 50, 65));

    // Кнопка генерации подземелья.
    generateButton.setSize({155.f, 40.f});
    generateButton.setPosition({882.f, 495.f});
    generateButton.setFillColor(sf::Color(65, 100, 80));

    // Кнопка полной информации.
    fullInfoButton.setSize({155.f, 40.f});
    fullInfoButton.setPosition({882.f, 565.f});
    fullInfoButton.setFillColor(sf::Color(65, 85, 105));

    // Подписи полей ввода.
    startLabel.setString("Start room:");
    startLabel.setCharacterSize(14);
    startLabel.setPosition({882.f, 320.f});

    endLabel.setString("End room:");
    endLabel.setCharacterSize(14);
    endLabel.setPosition({882.f, 395.f});

    // Значения полей ввода.
    startValue.setString(startInputText);
    startValue.setCharacterSize(16);
    startValue.setPosition({892.f, 350.f});

    endValue.setString(endInputText);
    endValue.setCharacterSize(16);
    endValue.setPosition({892.f, 425.f});

    // Нижняя информационная панель.
    infoPanel.setSize({1060.f, 100.f});
    infoPanel.setPosition({20.f, 830.f});
    infoPanel.setFillColor(sf::Color(35, 35, 45));

    // Заголовок информационной панели.
    infoTitle.setCharacterSize(18);
    infoTitle.setFillColor(sf::Color::White);
    infoTitle.setPosition({35.f, 840.f});
    infoTitle.setString("ALGORITHM RESULTS");

    // Содержимое информационной панели.
    infoContent.setCharacterSize(14);
    infoContent.setFillColor(sf::Color(200, 200, 210));
    infoContent.setPosition({35.f, 870.f});
    infoContent.setString("Select an algorithm to see its results");

    // Кнопка переключения информационной панели.
    infoToggleButton.setSize({140.f, 28.f});
    infoToggleButton.setPosition({900.f, 835.f});
    infoToggleButton.setFillColor(sf::Color(65, 85, 105));

    // Панель информации о подземелье.
    dungeonInfoPanel.setSize({1060.f, 280.f});
    dungeonInfoPanel.setPosition({20.f, 915.f});
    dungeonInfoPanel.setFillColor(sf::Color(35, 35, 45));

    dungeonInfoText.setCharacterSize(11);
    dungeonInfoText.setFillColor(sf::Color(200, 200, 210));
    dungeonInfoText.setPosition({35.f, 925.f});

    // Настройка камеры подземелья.
    dungeonCamera.setSize({800.f, 800.f});
    dungeonCamera.setCenter({420.f, 420.f});

    dungeonCamera.setViewport(
        sf::FloatRect({0.01818f, 0.02105f}, {0.72727f, 0.8421f})
    );
}

// Отрисовка игрового интерфейса и подземелья.
void Game::draw(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(dungeonArea);

    window.setView(dungeonCamera);

    if (graph == nullptr) {
        return;
    }

    // Отрисовка дорог между комнатами.
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);

        for (const Connection& connection : room.connectedRooms) {
            if (connection.roomId <= room.id) {
                continue;
            }

            const Room& neighbor = graph->getRoom(connection.roomId);

            // Выбор текстуры дороги по набору комнаты.
            const sf::Texture* roadTexture = nullptr;

            if (room.textureSet == 1) {
                roadTexture = &purpleRoadTexture;
            }
            else if (room.textureSet == 2) {
                roadTexture = &greenRoadTexture;
            }
            else if (room.textureSet == 3) {
                roadTexture = &blueRoadTexture;
            }
            else if (room.textureSet == 4) {
                roadTexture = &grayRoadTexture;
            }
            else {
                roadTexture = &brownRoadTexture;
            }

            float dx = neighbor.position.x - room.position.x;
            float dy = neighbor.position.y - room.position.y;

            float length = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

            // Отрисовка обычной дороги.
            if (roadTexturesLoaded) {
                sf::Sprite roadSprite(*roadTexture);

                roadSprite.setOrigin({64.f, 64.f});

                roadSprite.setPosition({
                    (room.position.x + neighbor.position.x) / 2.f,
                    (room.position.y + neighbor.position.y) / 2.f
                });

                roadSprite.setRotation(sf::degrees(angle + 90.f));
                roadSprite.setScale({1.f, length / 128.f});

                window.draw(roadSprite);
            }

            // Проверка принадлежности соединения к маршрутам.
            bool isBfsEdge = false;
            bool isDfsEdge = false;
            bool isDijkstraEdge = false;

            for (size_t j = 0; j + 1 < bfsPath.size(); j++) {
                if (
                    (bfsPath[j] == room.id && bfsPath[j + 1] == neighbor.id) ||
                    (bfsPath[j] == neighbor.id && bfsPath[j + 1] == room.id)
                ) {
                    isBfsEdge = true;
                    break;
                }
            }

            for (size_t j = 0; j + 1 < dfsPath.size(); j++) {
                if (
                    (dfsPath[j] == room.id && dfsPath[j + 1] == neighbor.id) ||
                    (dfsPath[j] == neighbor.id && dfsPath[j + 1] == room.id)
                ) {
                    isDfsEdge = true;
                    break;
                }
            }

            for (size_t j = 0; j + 1 < dijPath.size(); j++) {
                if (
                    (dijPath[j] == room.id && dijPath[j + 1] == neighbor.id) ||
                    (dijPath[j] == neighbor.id && dijPath[j + 1] == room.id)
                ) {
                    isDijkstraEdge = true;
                    break;
                }
            }

            // Выбор текстуры найденного маршрута.
            const sf::Texture* pathTexture = nullptr;

            if (isBfsEdge) {
                pathTexture = &yellowPathTexture;
            }
            else if (isDfsEdge) {
                pathTexture = &redPathTexture;
            }
            else if (isDijkstraEdge) {
                pathTexture = &orangePathTexture;
            }

            // Отрисовка маршрута поверх обычной дороги.
            if (pathTexturesLoaded && pathTexture != nullptr) {
                sf::Sprite pathSprite(*pathTexture);

                pathSprite.setOrigin({64.f, 64.f});

                pathSprite.setPosition({
                    (room.position.x + neighbor.position.x) / 2.f,
                    (room.position.y + neighbor.position.y) / 2.f
                });

                pathSprite.setRotation(sf::degrees(angle + 90.f));
                pathSprite.setScale({1.f, length / 128.f});

                window.draw(pathSprite);
            }

            // Отрисовка веса соединения.
            sf::Text weightText(font, std::to_string(connection.weight), 12);
            weightText.setFillColor(sf::Color::White);
            weightText.setOrigin(weightText.getLocalBounds().size / 2.f);

            weightText.setPosition({
                (room.position.x + neighbor.position.x) / 2.f,
                (room.position.y + neighbor.position.y) / 2.f
            });

            window.draw(weightText);
        }
    }

    // Отрисовка комнат.
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);

        const sf::Texture* selectedRoom;
        const sf::Texture* selectedTop;
        const sf::Texture* selectedLeft;
        const sf::Texture* selectedDown;
        const sf::Texture* selectedRight;

        bool texturesLoaded;

        // Выбор набора текстур комнаты.
        if (room.textureSet == 1) {
            selectedRoom = &roomTexture;
            selectedTop = &topTexture;
            selectedLeft = &leftTexture;
            selectedDown = &downTexture;
            selectedRight = &rightTexture;

            texturesLoaded = roomTexturesLoaded;
        }
        else if (room.textureSet == 2) {
            selectedRoom = &roomTexture2;
            selectedTop = &topTexture2;
            selectedLeft = &leftTexture2;
            selectedDown = &downTexture2;
            selectedRight = &rightTexture2;

            texturesLoaded = roomTextures2Loaded;
        }
        else if (room.textureSet == 3) {
            selectedRoom = &roomTexture3;
            selectedTop = &topTexture3;
            selectedLeft = &leftTexture3;
            selectedDown = &downTexture3;
            selectedRight = &rightTexture3;

            texturesLoaded = roomTextures3Loaded;
        }
        else if (room.textureSet == 4) {
            selectedRoom = &roomTexture4;
            selectedTop = &topTexture4;
            selectedLeft = &leftTexture4;
            selectedDown = &downTexture4;
            selectedRight = &rightTexture4;

            texturesLoaded = roomTextures4Loaded;
        }
        else {
            selectedRoom = &roomTexture5;
            selectedTop = &topTexture5;
            selectedLeft = &leftTexture5;
            selectedDown = &downTexture5;
            selectedRight = &rightTexture5;

            texturesLoaded = roomTextures5Loaded;
        }

        // Проверка загрузки текстур комнаты.
        if (texturesLoaded) {
            sf::Sprite roomSprite(*selectedRoom);

            roomSprite.setOrigin({64.f, 64.f});
            roomSprite.setPosition(room.position);
            roomSprite.setColor(sf::Color::White);

            window.draw(roomSprite);

            // Отрисовка проходов в соседние комнаты.
            for (const Connection& connection : room.connectedRooms) {
                const Room& neighbor = graph->getRoom(connection.roomId);

                float dx = neighbor.position.x - room.position.x;
                float dy = neighbor.position.y - room.position.y;

                const sf::Texture* doorTexture = nullptr;

                if (dx > 0) {
                    doorTexture = selectedRight;
                }
                else if (dx < 0) {
                    doorTexture = selectedLeft;
                }
                else if (dy > 0) {
                    doorTexture = selectedDown;
                }
                else if (dy < 0) {
                    doorTexture = selectedTop;
                }

                if (doorTexture != nullptr) {
                    sf::Sprite doorSprite(*doorTexture);

                    doorSprite.setOrigin({64.f, 64.f});
                    doorSprite.setPosition(room.position);

                    window.draw(doorSprite);
                }
            }
        }

        // Отрисовка номера комнаты.
        sf::Text number(font, std::to_string(room.id), 20);

        number.setFillColor(sf::Color::White);
        number.setOrigin(number.getLocalBounds().size / 2.f);
        number.setPosition(room.position);

        window.draw(number);
    }

    // Возврат к стандартному виду окна.
    window.setView(window.getDefaultView());

    // Отрисовка элементов правой панели.
    window.draw(controlPanel);
    window.draw(bfsButton);
    window.draw(dfsButton);
    window.draw(dijkstraButton);
    window.draw(generateButton);
    window.draw(fullInfoButton);

    // Получение положения мыши.
    sf::Vector2f mousePos = window.mapPixelToCoords(
        sf::Mouse::getPosition(window),
        window.getDefaultView()
    );

    // Изменение цвета полей ввода при наведении.
    startInput.setFillColor(
        startInput.getGlobalBounds().contains(mousePos)
            ? sf::Color(75, 75, 95)
            : sf::Color(50, 50, 65)
    );

    endInput.setFillColor(
        endInput.getGlobalBounds().contains(mousePos)
            ? sf::Color(75, 75, 95)
            : sf::Color(50, 50, 65)
    );

    // Подсветка активного поля ввода.
    startInput.setOutlineThickness(startInputActive ? 2.f : 0.f);
    startInput.setOutlineColor(sf::Color::Cyan);

    endInput.setOutlineThickness(endInputActive ? 2.f : 0.f);
    endInput.setOutlineColor(sf::Color::Cyan);

    // Отрисовка полей ввода и подписей.
    window.draw(startInput);
    window.draw(endInput);

    window.draw(startLabel);
    window.draw(endLabel);

    window.draw(startValue);
    window.draw(endValue);

    // Отрисовка нижней информационной панели.
    window.draw(infoPanel);

    if (showDungeonInfo) {
        // Режим отображения информации о подземелье.
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
        // Режим отображения результатов алгоритмов.
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

    // Кнопка переключения информационной панели.
    window.draw(infoToggleButton);

    sf::Text toggleText(
        font,
        showDungeonInfo ? "ALGORITHMS" : "DUNGEON INFO",
        11
    );

    toggleText.setFillColor(sf::Color::White);
    toggleText.setOrigin(toggleText.getLocalBounds().size / 2.f);
    toggleText.setPosition({970.f, 849.f});

    window.draw(toggleText);

    // Заголовок правой панели.
    sf::Text title(font, "ALGORITHMS", 16);

    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().size / 2.f);
    title.setPosition({959.5f, 75.f});

    window.draw(title);

    // Подпись кнопки BFS.
    sf::Text bfsText(font, "BFS", 16);

    bfsText.setFillColor(sf::Color::White);
    bfsText.setOrigin(bfsText.getLocalBounds().size / 2.f);
    bfsText.setPosition({959.5f, 152.5f});

    window.draw(bfsText);

    // Подпись кнопки DFS.
    sf::Text dfsText(font, "DFS", 16);

    dfsText.setFillColor(sf::Color::White);
    dfsText.setOrigin(dfsText.getLocalBounds().size / 2.f);
    dfsText.setPosition({959.5f, 217.5f});

    window.draw(dfsText);

    // Подпись кнопки Дейкстры.
    sf::Text dijkstraText(font, "DIJKSTRA", 14);

    dijkstraText.setFillColor(sf::Color::White);
    dijkstraText.setOrigin(dijkstraText.getLocalBounds().size / 2.f);
    dijkstraText.setPosition({959.5f, 282.5f});

    window.draw(dijkstraText);

    // Подпись кнопки генерации.
    sf::Text generateText(font, "NEW DUNGEON", 12);

    generateText.setFillColor(sf::Color::White);
    generateText.setOrigin(generateText.getLocalBounds().size / 2.f);
    generateText.setPosition({959.5f, 515.f});

    window.draw(generateText);

    // Подпись кнопки полной информации.
    sf::Text fullInfoText(font, "FULL INFO", 12);

    fullInfoText.setFillColor(sf::Color::White);
    fullInfoText.setOrigin(fullInfoText.getLocalBounds().size / 2.f);
    fullInfoText.setPosition({959.5f, 585.f});

    window.draw(fullInfoText);

    // Легенда цветов маршрутов.
    sf::Text legendTitle(font, "PATH COLORS", 14);

    legendTitle.setFillColor(sf::Color::White);
    legendTitle.setPosition({885.f, 630.f});

    window.draw(legendTitle);

    // Цвет маршрута BFS.
    sf::CircleShape bfsLegend(5.f);

    bfsLegend.setFillColor(sf::Color::Yellow);
    bfsLegend.setPosition({885.f, 660.f});

    window.draw(bfsLegend);

    sf::Text bfsLegendText(font, "BFS", 12);

    bfsLegendText.setFillColor(sf::Color::White);
    bfsLegendText.setPosition({905.f, 656.f});

    window.draw(bfsLegendText);

    // Цвет маршрута DFS.
    sf::CircleShape dfsLegend(5.f);

    dfsLegend.setFillColor(sf::Color::Red);
    dfsLegend.setPosition({885.f, 685.f});

    window.draw(dfsLegend);

    sf::Text dfsLegendText(font, "DFS", 12);

    dfsLegendText.setFillColor(sf::Color::White);
    dfsLegendText.setPosition({905.f, 681.f});

    window.draw(dfsLegendText);

    // Цвет маршрута Дейкстры.
    sf::CircleShape dijkstraLegend(5.f);

    dijkstraLegend.setFillColor(sf::Color(255, 140, 0));
    dijkstraLegend.setPosition({885.f, 710.f});

    window.draw(dijkstraLegend);

    sf::Text dijkstraLegendText(font, "DIJKSTRA", 12);

    dijkstraLegendText.setFillColor(sf::Color::White);
    dijkstraLegendText.setPosition({905.f, 706.f});

    window.draw(dijkstraLegendText);

    // Отрисовка отдельного окна полной информации.
    if (showInfoWindow && infoWindow.isOpen()) {
        // Обработка событий отдельного окна.
        while (auto event = infoWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                infoWindow.close();
                showInfoWindow = false;
            }
        }

        infoWindow.clear(sf::Color(35, 35, 45));

        // Заголовок окна.
        sf::Text infoWindowTitle(font, "DUNGEON INFORMATION", 22);

        infoWindowTitle.setFillColor(sf::Color::White);
        infoWindowTitle.setPosition({20.f, 20.f});

        infoWindow.draw(infoWindowTitle);

        // Информация о комнатах.
        dungeonInfoText.setPosition({20.f, 70.f});

        infoWindow.draw(dungeonInfoText);

        infoWindow.display();
    }
}

// Обработка событий окна.
void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    // Масштабирование подземелья колесиком мыши.
    if (const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            mouse->position,
            window.getDefaultView()
        );

        // Масштабирование выполняется только над областью подземелья.
        if (dungeonArea.getGlobalBounds().contains(mousePos)) {
            sf::Vector2f beforePos = window.mapPixelToCoords(
                mouse->position,
                dungeonCamera
            );

            sf::Vector2f cameraSize = dungeonCamera.getSize();

            if (mouse->delta > 0 && cameraSize.x > 275.f) {
                dungeonCamera.zoom(0.9f);
            }
            else if (mouse->delta < 0 && cameraSize.x < 3000.f) {
                dungeonCamera.zoom(1.1f);
            }

            sf::Vector2f afterPos = window.mapPixelToCoords(
                mouse->position,
                dungeonCamera
            );

            dungeonCamera.move(beforePos - afterPos);
        }
    }

    // Обработка нажатия кнопки мыши.
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                mouse->position,
                window.getDefaultView()
            );

            // Переключение нижней информационной панели.
            if (infoToggleButton.getGlobalBounds().contains(mousePos)) {
                showDungeonInfo = !showDungeonInfo;
                return;
            }

            // Открытие окна полной информации.
            if (fullInfoButton.getGlobalBounds().contains(mousePos)) {
                showInfoWindow = true;

                infoWindow.create(
                    sf::VideoMode({600, 700}),
                    "Dungeon Information"
                );

                return;
            }

            // Определение активного поля ввода.
            startInputActive = startInput.getGlobalBounds().contains(mousePos);
            endInputActive = endInput.getGlobalBounds().contains(mousePos);

            if (graph != nullptr) {
                // Генерация нового подземелья.
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    graph->clear();
                    graph->GenerationRooms();
                    graph->generateConnections();

                    fitDungeonToView(*graph, dungeonCamera);

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

                // Проверка нажатия кнопки алгоритма.
                bool algorithmButtonPressed =
                    bfsButton.getGlobalBounds().contains(mousePos) ||
                    dfsButton.getGlobalBounds().contains(mousePos) ||
                    dijkstraButton.getGlobalBounds().contains(mousePos);

                if (algorithmButtonPressed) {
                    // Проверка заполнения полей ввода.
                    if (startInputText.empty() || endInputText.empty()) {
                        infoTitle.setString("Input error");
                        infoContent.setString("Please enter both room IDs");
                        return;
                    }

                    int startRoom = std::stoi(startInputText);
                    int endRoom = std::stoi(endInputText);

                    // Формирование строки найденного маршрута.
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

                    // Проверка существования указанных комнат.
                    if (
                        startRoom < 0 ||
                        endRoom < 0 ||
                        startRoom >= graph->getRoomCount() ||
                        endRoom >= graph->getRoomCount()
                    ) {
                        bfsResult = "Room ID does not exist";
                        dfsResult = "Room ID does not exist";
                        dijkstraResult = "Room ID does not exist";

                        return;
                    }

                    // Выполнение поиска в ширину.
                    if (bfsButton.getGlobalBounds().contains(mousePos)) {
                        bfsPath = graph->BFS(startRoom, endRoom);

                        if (bfsPath.empty()) {
                            bfsResult = "No path found";
                        }
                        else {
                            bfsResult = makePathString(bfsPath);

                            bfsResult +=
                                " | Rooms: " + std::to_string(bfsPath.size()) +
                                " | Visited: " +
                                std::to_string(graph->getLastVisitedCount());
                        }

                        dfsPath.clear();
                        dijPath.clear();
                    }

                    // Выполнение поиска в глубину.
                    if (dfsButton.getGlobalBounds().contains(mousePos)) {
                        dfsPath = graph->DFS(startRoom, endRoom);

                        if (dfsPath.empty()) {
                            dfsResult = "No path found";
                        }
                        else {
                            dfsResult = makePathString(dfsPath);

                            dfsResult +=
                                " | Rooms: " + std::to_string(dfsPath.size()) +
                                " | Visited: " +
                                std::to_string(graph->getLastVisitedCount());
                        }

                        bfsPath.clear();
                        dijPath.clear();
                    }

                    // Выполнение алгоритма Дейкстры.
                    if (dijkstraButton.getGlobalBounds().contains(mousePos)) {
                        dijPath = graph->Dijkstra(startRoom, endRoom);

                        if (dijPath.empty()) {
                            dijkstraResult = "No path found";
                        }
                        else {
                            int totalCost = 0;

                            // Подсчёт общей стоимости маршрута.
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

                            dijkstraResult +=
                                " | Cost: " + std::to_string(totalCost);

                            dijkstraResult +=
                                " | Rooms: " + std::to_string(dijPath.size());
                        }

                        bfsPath.clear();
                        dfsPath.clear();
                    }
                }
            }
        }
    }

    // Обработка ввода текста с клавиатуры.
    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        // Ввод числовых значений.
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

        // Удаление символов клавишей Backspace.
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

// Передача графа игровому классу.
void Game::setGraph(Graph& graph) {
    this->graph = &graph;

    dungeonInfoText.setString(this->graph->PrintGraph());

    fitDungeonToView(*this->graph, dungeonCamera);
}