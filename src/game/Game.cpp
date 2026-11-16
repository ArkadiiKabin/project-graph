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

// подгоняет камеру под размеры подземелья
void fitDungeonToView(const Graph& graph, sf::View& camera) {
    if (graph.getRoomCount() == 0) { return; }

    const float roomSize = 128.f;
    const float padding = 40.f;

    const Room& firstRoom = graph.getRoom(0);

    float minX = firstRoom.position.x;
    float maxX = firstRoom.position.x;
    float minY = firstRoom.position.y;
    float maxY = firstRoom.position.y;

    for (int i = 1; i < graph.getRoomCount(); ++i) {
        const Room& room = graph.getRoom(i);

        minX = min(minX, room.position.x);
        maxX = max(maxX, room.position.x);
        minY = min(minY, room.position.y);
        maxY = max(maxY, room.position.y);
    }

    float width = maxX - minX + roomSize + padding * 2.f;
    float height = maxY - minY + roomSize + padding * 2.f;

    float viewSize = max(width, height);

    camera.setSize({viewSize, viewSize});
    camera.setCenter({(minX + maxX) / 2.f, (minY + maxY) / 2.f});
}

// конструктор игрового интерфейса
Game::Game()
    : infoTitle(font), startLabel(font), endLabel(font), startValue(font), endValue(font), dungeonInfoText(font),
      minCountRoomLabel(font), maxCountRoomLabel(font), minCountRoomValue(font), maxCountRoomValue(font),
      minWeightLabel(font), maxWeightLabel(font), minWeightValue(font), maxWeightValue(font), densityLabel(font),
      densityValue(font), settingsTitle(font), settingsApplyText(font), settingsCancelText(font),
      settingsResetText(font), settingsButtonText(font) {

    // загрузка шрифта
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) { return; }

    // загрузка текстур дорог
    roadTexturesLoaded = blueRoadTexture.loadFromFile("assets/textures/roads/blue_road.png") &&
                         greenRoadTexture.loadFromFile("assets/textures/roads/green_road.png") &&
                         brownRoadTexture.loadFromFile("assets/textures/roads/brown_road.png") &&
                         purpleRoadTexture.loadFromFile("assets/textures/roads/purple_road.png") &&
                         grayRoadTexture.loadFromFile("assets/textures/roads/gray_road.png");

    // отключение сглаживания текстур дорог
    blueRoadTexture.setSmooth(false);
    greenRoadTexture.setSmooth(false);
    brownRoadTexture.setSmooth(false);
    purpleRoadTexture.setSmooth(false);
    grayRoadTexture.setSmooth(false);

    // загрузка текстур маршрутов
    pathTexturesLoaded = yellowPathTexture.loadFromFile("assets/textures/roads/yellow_path.png") &&
                         redPathTexture.loadFromFile("assets/textures/roads/red_path.png") &&
                         orangePathTexture.loadFromFile("assets/textures/roads/orange_path.png");

    yellowPathTexture.setSmooth(false);
    redPathTexture.setSmooth(false);
    orangePathTexture.setSmooth(false);

    // загрузка первого набора текстур комнат
    roomTexturesLoaded = roomTexture.loadFromFile("assets/textures/room_1/room.png") &&
                         topTexture.loadFromFile("assets/textures/room_1/top.png") &&
                         leftTexture.loadFromFile("assets/textures/room_1/left.png") &&
                         downTexture.loadFromFile("assets/textures/room_1/down.png") &&
                         rightTexture.loadFromFile("assets/textures/room_1/right.png");

    // загрузка второго набора текстур комнат
    roomTextures2Loaded = roomTexture2.loadFromFile("assets/textures/room_2/room.png") &&
                          topTexture2.loadFromFile("assets/textures/room_2/top.png") &&
                          leftTexture2.loadFromFile("assets/textures/room_2/left.png") &&
                          downTexture2.loadFromFile("assets/textures/room_2/down.png") &&
                          rightTexture2.loadFromFile("assets/textures/room_2/right.png");

    // загрузка третьего набора текстур комнат
    roomTextures3Loaded = roomTexture3.loadFromFile("assets/textures/room_3/room.png") &&
                          topTexture3.loadFromFile("assets/textures/room_3/top.png") &&
                          leftTexture3.loadFromFile("assets/textures/room_3/left.png") &&
                          downTexture3.loadFromFile("assets/textures/room_3/down.png") &&
                          rightTexture3.loadFromFile("assets/textures/room_3/right.png");

    // загрузка четвёртого набора текстур комнат
    roomTextures4Loaded = roomTexture4.loadFromFile("assets/textures/room_4/room.png") &&
                          topTexture4.loadFromFile("assets/textures/room_4/top.png") &&
                          leftTexture4.loadFromFile("assets/textures/room_4/left.png") &&
                          downTexture4.loadFromFile("assets/textures/room_4/down.png") &&
                          rightTexture4.loadFromFile("assets/textures/room_4/right.png");

    // загрузка пятого набора текстур комнат
    roomTextures5Loaded = roomTexture5.loadFromFile("assets/textures/room_5/room.png") &&
                          topTexture5.loadFromFile("assets/textures/room_5/top.png") &&
                          leftTexture5.loadFromFile("assets/textures/room_5/left.png") &&
                          downTexture5.loadFromFile("assets/textures/room_5/down.png") &&
                          rightTexture5.loadFromFile("assets/textures/room_5/right.png");

    // отключение сглаживания текстур комнат
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

    // количество комнат
    minCountRoomInput.setSize({65.f, 30.f});
    maxCountRoomInput.setSize({65.f, 30.f});

    minCountRoomInput.setFillColor(sf::Color(45, 45, 55));
    maxCountRoomInput.setFillColor(sf::Color(45, 45, 55));

    minCountRoomLabel.setString("Min:");
    maxCountRoomLabel.setString("Max:");

    minCountRoomValue.setString(minCountRoomText);
    maxCountRoomValue.setString(maxCountRoomText);

    minCountRoomLabel.setPosition({360.f, 300.f});
    maxCountRoomLabel.setPosition({560.f, 300.f});

    minCountRoomInput.setPosition({360.f, 325.f});
    maxCountRoomInput.setPosition({560.f, 325.f});

    minCountRoomValue.setPosition({380.f, 330.f});
    maxCountRoomValue.setPosition({580.f, 330.f});

    minCountRoomLabel.setCharacterSize(13);
    maxCountRoomLabel.setCharacterSize(13);
    minCountRoomValue.setCharacterSize(15);
    maxCountRoomValue.setCharacterSize(15);

    minCountRoomLabel.setFillColor(sf::Color::White);
    maxCountRoomLabel.setFillColor(sf::Color::White);
    minCountRoomValue.setFillColor(sf::Color::White);
    maxCountRoomValue.setFillColor(sf::Color::White);

    // вес коридоров
    minWeightInput.setSize({65.f, 30.f});
    maxWeightInput.setSize({65.f, 30.f});

    minWeightInput.setFillColor(sf::Color(45, 45, 55));
    maxWeightInput.setFillColor(sf::Color(45, 45, 55));

    minWeightLabel.setString("Min:");
    maxWeightLabel.setString("Max:");

    minWeightValue.setString(minWeightText);
    maxWeightValue.setString(maxWeightText);

    minWeightLabel.setPosition({360.f, 400.f});
    maxWeightLabel.setPosition({560.f, 400.f});

    minWeightInput.setPosition({360.f, 425.f});
    maxWeightInput.setPosition({560.f, 425.f});

    minWeightValue.setPosition({380.f, 430.f});
    maxWeightValue.setPosition({580.f, 430.f});

    minWeightLabel.setCharacterSize(13);
    maxWeightLabel.setCharacterSize(13);
    minWeightValue.setCharacterSize(15);
    maxWeightValue.setCharacterSize(15);

    minWeightLabel.setFillColor(sf::Color::White);
    maxWeightLabel.setFillColor(sf::Color::White);
    minWeightValue.setFillColor(sf::Color::White);
    maxWeightValue.setFillColor(sf::Color::White);

    // плотность соединений
    densityInput.setSize({155.f, 30.f});
    densityInput.setFillColor(sf::Color(45, 45, 55));

    densityLabel.setString("Density:");
    switch (density) {
    case Density::Low: densityValue.setString("Low"); break;
    case Density::Medium: densityValue.setString("Medium"); break;
    case Density::High: densityValue.setString("High"); break;
    }

    densityLabel.setPosition({360.f, 500.f});
    densityInput.setPosition({360.f, 525.f});
    densityValue.setPosition({375.f, 530.f});

    densityLabel.setCharacterSize(13);
    densityValue.setCharacterSize(15);

    densityLabel.setFillColor(sf::Color::White);
    densityValue.setFillColor(sf::Color::White);

    // настройка пунктов списка плотности
    for (int i = 0; i < 3; i++) {
        densityOptions[i].setSize({110.f, 28.f});
        densityOptions[i].setFillColor(sf::Color(55, 55, 65));
        densityOptions[i].setOutlineThickness(1.f);
        densityOptions[i].setOutlineColor(sf::Color(100, 100, 110));

        densityOptions[i].setPosition({densityInput.getPosition().x, densityInput.getPosition().y + 30.f + i * 28.f});
    }

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

    // фон затемнения
    settingsBackground.setSize({1080.f, 950.f});
    settingsBackground.setPosition({0.f, 0.f});
    settingsBackground.setFillColor(sf::Color(0, 0, 0, 180));

    // основная панель настроек
    settingsPanel.setSize({500.f, 550.f});
    settingsPanel.setPosition({310.f, 190.f});
    settingsPanel.setFillColor(sf::Color(35, 35, 45));
    settingsPanel.setOutlineThickness(3.f);
    settingsPanel.setOutlineColor(sf::Color(120, 100, 75));

    // заголовок настроек
    settingsTitle.setString("GENERATION SETTINGS");
    settingsTitle.setCharacterSize(28);

    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y});
    settingsTitle.setPosition({560.f, 220.f});
    settingsTitle.setFillColor(sf::Color(230, 210, 170));

    // кнопка применения настроек
    settingsApplyButton.setSize({180.f, 45.f});
    settingsApplyButton.setPosition({370.f, 650.f});
    settingsApplyButton.setFillColor(sf::Color(65, 110, 75));

    settingsApplyText.setString("APPLY");
    settingsApplyText.setCharacterSize(20);
    settingsApplyText.setFillColor(sf::Color::White);
    settingsApplyText.setPosition({430.f, 660.f});

    // кнопка отмены настроек
    settingsCancelButton.setSize({180.f, 45.f});
    settingsCancelButton.setPosition({570.f, 650.f});
    settingsCancelButton.setFillColor(sf::Color(120, 65, 65));

    // кнопка сброса настроек
    settingsResetButton.setSize({140.f, 35.f});
    settingsResetButton.setPosition({490.f, 700.f});
    settingsResetButton.setFillColor(sf::Color(85, 85, 100));

    settingsResetText.setString("RESET");
    settingsResetText.setCharacterSize(17);
    settingsResetText.setFillColor(sf::Color::White);
    settingsResetText.setPosition({530.f, 707.f});

    settingsCancelText.setString("CANCEL");
    settingsCancelText.setCharacterSize(20);
    settingsCancelText.setFillColor(sf::Color::White);
    settingsCancelText.setPosition({615.f, 660.f});

    // кнопка настроек
    settingsButton.setSize({200.f, 45.f});
    settingsButton.setPosition({860.f, 750.f});
    settingsButton.setFillColor(sf::Color(65, 65, 80));

    settingsButtonText.setString("SETTINGS");
    settingsButtonText.setCharacterSize(18);
    settingsButtonText.setFillColor(sf::Color::White);
    settingsButtonText.setPosition({915.f, 760.f});

    centerTextInButton(settingsApplyText, settingsApplyButton);
    centerTextInButton(settingsCancelText, settingsCancelButton);
    centerTextInButton(settingsResetText, settingsResetButton);
    centerTextInButton(settingsButtonText, settingsButton);
}

// отрисовка игрового интерфейса и подземелья
void Game::draw(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(dungeonArea);

    window.setView(dungeonCamera);

    if (graph == nullptr) { return; }

    // отрисовка дорог между комнатами
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);

        for (const Connection& connection : room.connectedRooms) {
            if (connection.roomId <= room.id) { continue; }

            const Room& neighbor = graph->getRoom(connection.roomId);

            // выбор текстуры дороги по набору комнаты
            const sf::Texture* roadTexture = nullptr;

            if (room.textureSet == 1) {
                roadTexture = &purpleRoadTexture;
            } else if (room.textureSet == 2) {
                roadTexture = &greenRoadTexture;
            } else if (room.textureSet == 3) {
                roadTexture = &blueRoadTexture;
            } else if (room.textureSet == 4) {
                roadTexture = &grayRoadTexture;
            } else {
                roadTexture = &brownRoadTexture;
            }

            float dx = neighbor.position.x - room.position.x;
            float dy = neighbor.position.y - room.position.y;

            float length = sqrt(dx * dx + dy * dy);
            float angle = atan2(dy, dx) * 180.f / 3.14159265f;

            // отрисовка обычной дороги
            if (roadTexturesLoaded) {
                sf::Sprite roadSprite(*roadTexture);

                roadSprite.setOrigin({64.f, 64.f});
                roadSprite.setPosition(
                    {(room.position.x + neighbor.position.x) / 2.f, (room.position.y + neighbor.position.y) / 2.f});

                roadSprite.setRotation(sf::degrees(angle + 90.f));
                roadSprite.setScale({1.f, length / 128.f});

                window.draw(roadSprite);
            }

            // проверка принадлежности соединения к маршрутам
            bool isBfsEdge = false;
            bool isDfsEdge = false;
            bool isDijkstraEdge = false;

            for (size_t j = 0; j + 1 < bfsPath.size(); j++) {
                if ((bfsPath[j] == room.id && bfsPath[j + 1] == neighbor.id) ||
                    (bfsPath[j] == neighbor.id && bfsPath[j + 1] == room.id)) {
                    isBfsEdge = true;
                    break;
                }
            }

            for (size_t j = 0; j + 1 < dfsPath.size(); j++) {
                if ((dfsPath[j] == room.id && dfsPath[j + 1] == neighbor.id) ||
                    (dfsPath[j] == neighbor.id && dfsPath[j + 1] == room.id)) {
                    isDfsEdge = true;
                    break;
                }
            }

            for (size_t j = 0; j + 1 < dijPath.size(); j++) {
                if ((dijPath[j] == room.id && dijPath[j + 1] == neighbor.id) ||
                    (dijPath[j] == neighbor.id && dijPath[j + 1] == room.id)) {
                    isDijkstraEdge = true;
                    break;
                }
            }

            // выбор текстуры найденного маршрута
            const sf::Texture* pathTexture = nullptr;

            if (isBfsEdge) {
                pathTexture = &yellowPathTexture;
            } else if (isDfsEdge) {
                pathTexture = &redPathTexture;
            } else if (isDijkstraEdge) {
                pathTexture = &orangePathTexture;
            }

            // отрисовка маршрута поверх обычной дороги
            if (pathTexturesLoaded && pathTexture != nullptr) {
                sf::Sprite pathSprite(*pathTexture);

                pathSprite.setOrigin({64.f, 64.f});
                pathSprite.setPosition(
                    {(room.position.x + neighbor.position.x) / 2.f, (room.position.y + neighbor.position.y) / 2.f});

                pathSprite.setRotation(sf::degrees(angle + 90.f));
                pathSprite.setScale({1.f, length / 128.f});

                window.draw(pathSprite);
            }

            // отрисовка веса соединения
            sf::Text weightText(font, to_string(connection.weight), 12);
            weightText.setFillColor(sf::Color::White);
            weightText.setOrigin(weightText.getLocalBounds().size / 2.f);

            weightText.setPosition(
                {(room.position.x + neighbor.position.x) / 2.f, (room.position.y + neighbor.position.y) / 2.f});

            window.draw(weightText);
        }
    }

    // отрисовка комнат
    for (int i = 0; i < graph->getRoomCount(); i++) {
        const Room& room = graph->getRoom(i);

        const sf::Texture* selectedRoom;
        const sf::Texture* selectedTop;
        const sf::Texture* selectedLeft;
        const sf::Texture* selectedDown;
        const sf::Texture* selectedRight;

        bool texturesLoaded;

        // выбор набора текстур комнаты
        if (room.textureSet == 1) {
            selectedRoom = &roomTexture;
            selectedTop = &topTexture;
            selectedLeft = &leftTexture;
            selectedDown = &downTexture;
            selectedRight = &rightTexture;
            texturesLoaded = roomTexturesLoaded;
        } else if (room.textureSet == 2) {
            selectedRoom = &roomTexture2;
            selectedTop = &topTexture2;
            selectedLeft = &leftTexture2;
            selectedDown = &downTexture2;
            selectedRight = &rightTexture2;
            texturesLoaded = roomTextures2Loaded;
        } else if (room.textureSet == 3) {
            selectedRoom = &roomTexture3;
            selectedTop = &topTexture3;
            selectedLeft = &leftTexture3;
            selectedDown = &downTexture3;
            selectedRight = &rightTexture3;
            texturesLoaded = roomTextures3Loaded;
        } else if (room.textureSet == 4) {
            selectedRoom = &roomTexture4;
            selectedTop = &topTexture4;
            selectedLeft = &leftTexture4;
            selectedDown = &downTexture4;
            selectedRight = &rightTexture4;
            texturesLoaded = roomTextures4Loaded;
        } else {
            selectedRoom = &roomTexture5;
            selectedTop = &topTexture5;
            selectedLeft = &leftTexture5;
            selectedDown = &downTexture5;
            selectedRight = &rightTexture5;
            texturesLoaded = roomTextures5Loaded;
        }

        // проверка загрузки текстур комнаты
        if (texturesLoaded) {
            sf::Sprite roomSprite(*selectedRoom);

            roomSprite.setOrigin({64.f, 64.f});
            roomSprite.setPosition(room.position);
            roomSprite.setColor(sf::Color::White);

            window.draw(roomSprite);

            // отрисовка проходов в соседние комнаты
            for (const Connection& connection : room.connectedRooms) {
                const Room& neighbor = graph->getRoom(connection.roomId);

                float dx = neighbor.position.x - room.position.x;
                float dy = neighbor.position.y - room.position.y;

                const sf::Texture* doorTexture = nullptr;

                if (dx > 0) {
                    doorTexture = selectedRight;
                } else if (dx < 0) {
                    doorTexture = selectedLeft;
                } else if (dy > 0) {
                    doorTexture = selectedDown;
                } else if (dy < 0) {
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

        // отрисовка номера комнаты
        sf::Text number(font, to_string(room.id), 20);

        number.setFillColor(sf::Color::White);
        number.setOrigin(number.getLocalBounds().size / 2.f);
        number.setPosition(room.position);

        window.draw(number);
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

    minCountRoomInput.setOutlineThickness(minCountRoomActive ? 2.f : 0.f);
    minCountRoomInput.setOutlineColor(sf::Color::Cyan);

    minWeightInput.setOutlineThickness(minWeightActive ? 2.f : 0.f);
    minWeightInput.setOutlineColor(sf::Color::Cyan);

    maxCountRoomInput.setOutlineThickness(maxCountRoomActive ? 2.f : 0.f);
    maxCountRoomInput.setOutlineColor(sf::Color::Cyan);

    maxWeightInput.setOutlineThickness(maxWeightActive ? 2.f : 0.f);
    maxWeightInput.setOutlineColor(sf::Color::Cyan);

    densityInput.setOutlineThickness(densityListOpen ? 2.f : 0.f);
    densityInput.setOutlineColor(sf::Color::Cyan);

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
    if (settingsWindowOpen) {
        window.draw(settingsBackground);
        window.draw(settingsPanel);
        window.draw(settingsTitle);

        window.draw(settingsApplyButton);
        window.draw(settingsApplyText);

        window.draw(settingsCancelButton);
        window.draw(settingsCancelText);
        window.draw(settingsResetButton);
        window.draw(settingsResetText);

        window.draw(minCountRoomInput);
        window.draw(maxCountRoomInput);
        window.draw(minCountRoomLabel);
        window.draw(maxCountRoomLabel);
        window.draw(minCountRoomValue);
        window.draw(maxCountRoomValue);

        window.draw(minWeightInput);
        window.draw(maxWeightInput);
        window.draw(minWeightLabel);
        window.draw(maxWeightLabel);
        window.draw(minWeightValue);
        window.draw(maxWeightValue);

        window.draw(densityInput);
        window.draw(densityLabel);

        switch (density) {
        case Density::Low: densityValue.setString("Low"); break;
        case Density::Medium: densityValue.setString("Medium"); break;
        case Density::High: densityValue.setString("High"); break;
        }

        window.draw(densityValue);

        // выпадающий список плотности
        if (densityListOpen) {
            const string options[3] = {"Low", "Medium", "High"};

            for (int i = 0; i < 3; i++) {
                window.draw(densityOptions[i]);

                sf::Text optionText(font, options[i], 14);
                optionText.setFillColor(sf::Color::White);
                optionText.setPosition(
                    {densityOptions[i].getPosition().x + 10.f, densityOptions[i].getPosition().y + 5.f});

                window.draw(optionText);
            }
        }
    }
}

// обработка событий окна
void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    // масштабирование подземелья колесиком мыши
    if (!settingsWindowOpen) {
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
    }

    // обработка нажатия кнопки мыши
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position, window.getDefaultView());

            // открытие окна настроек
            if (settingsButton.getGlobalBounds().contains(mousePos)) {
                settingsWindowOpen = true;
                return;
            }

            // обработка открытого окна настроек
            if (settingsWindowOpen) {
                startInputActive = false;
                endInputActive = false;

                minCountRoomActive = false;
                maxCountRoomActive = false;
                minWeightActive = false;
                maxWeightActive = false;

                // закрытие без применения
                if (settingsCancelButton.getGlobalBounds().contains(mousePos)) {
                    settingsWindowOpen = false;
                    return;
                }

                // сброс настроек
                if (settingsResetButton.getGlobalBounds().contains(mousePos)) {
                    minCountRoomText = "8";
                    maxCountRoomText = "15";

                    minWeightText = "1";
                    maxWeightText = "10";

                    density = Density::Medium;

                    minCountRoomValue.setString(minCountRoomText);
                    maxCountRoomValue.setString(maxCountRoomText);

                    minWeightValue.setString(minWeightText);
                    maxWeightValue.setString(maxWeightText);

                    densityValue.setString("Medium");
                    densityListOpen = false;

                    return;
                }

                // применение настроек
                if (settingsApplyButton.getGlobalBounds().contains(mousePos)) {
                    if (minCountRoomText.empty() || maxCountRoomText.empty() || minWeightText.empty() ||
                        maxWeightText.empty()) {
                        return;
                    }

                    int minRooms = stoi(minCountRoomText);
                    int maxRooms = stoi(maxCountRoomText);
                    int minWeight = stoi(minWeightText);
                    int maxWeight = stoi(maxWeightText);

                    // проверка допустимых значений
                    if (minRooms < 1 || maxRooms > 99 || minWeight < 0 || maxWeight > 999 || minRooms > maxRooms ||
                        minWeight > maxWeight) {
                        return;
                    }

                    generationSettings.minRoomCount = minRooms;
                    generationSettings.maxRoomCount = maxRooms;
                    generationSettings.minWeightPath = minWeight;
                    generationSettings.maxWeightPath = maxWeight;
                    generationSettings.density = static_cast<int>(density);

                    if (graph != nullptr) { graph->setSettings(generationSettings); }

                    settingsWindowOpen = false;
                    return;
                }

                // открытие списка плотности
                if (densityInput.getGlobalBounds().contains(mousePos)) {
                    densityListOpen = !densityListOpen;
                    return;
                }

                // выбор плотности
                if (densityListOpen) {
                    for (int i = 0; i < 3; i++) {
                        if (densityOptions[i].getGlobalBounds().contains(mousePos)) {
                            density = static_cast<Density>(i + 1);
                            densityListOpen = false;
                            return;
                        }
                    }

                    densityListOpen = false;
                }

                // выбор активного поля
                minCountRoomActive = minCountRoomInput.getGlobalBounds().contains(mousePos);
                maxCountRoomActive = maxCountRoomInput.getGlobalBounds().contains(mousePos);
                minWeightActive = minWeightInput.getGlobalBounds().contains(mousePos);
                maxWeightActive = maxWeightInput.getGlobalBounds().contains(mousePos);

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

            minCountRoomActive = minCountRoomInput.getGlobalBounds().contains(mousePos);
            maxCountRoomActive = maxCountRoomInput.getGlobalBounds().contains(mousePos);

            minWeightActive = minWeightInput.getGlobalBounds().contains(mousePos);
            maxWeightActive = maxWeightInput.getGlobalBounds().contains(mousePos);

            if (graph != nullptr) {
                // генерация нового подземелья
                if (generateButton.getGlobalBounds().contains(mousePos)) {
                    graph->clear();
                    graph->setSettings(generationSettings);
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
            } else if (minCountRoomActive && minCountRoomText.size() < 2) {
                minCountRoomText += digit;
                minCountRoomValue.setString(minCountRoomText);
            } else if (maxCountRoomActive && maxCountRoomText.size() < 2) {
                maxCountRoomText += digit;
                maxCountRoomValue.setString(maxCountRoomText);
            } else if (minWeightActive && minWeightText.size() < 3) {
                minWeightText += digit;
                minWeightValue.setString(minWeightText);
            } else if (maxWeightActive && maxWeightText.size() < 3) {
                maxWeightText += digit;
                maxWeightValue.setString(maxWeightText);
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
            } else if (minCountRoomActive && !minCountRoomText.empty()) {
                minCountRoomText.pop_back();
                minCountRoomValue.setString(minCountRoomText);
            } else if (maxCountRoomActive && !maxCountRoomText.empty()) {
                maxCountRoomText.pop_back();
                maxCountRoomValue.setString(maxCountRoomText);
            } else if (minWeightActive && !minWeightText.empty()) {
                minWeightText.pop_back();
                minWeightValue.setString(minWeightText);
            } else if (maxWeightActive && !maxWeightText.empty()) {
                maxWeightText.pop_back();
                maxWeightValue.setString(maxWeightText);
            }
        }
    }
}

// передача графа игровому классу
void Game::setGraph(Graph& graph) {
    this->graph = &graph;

    dungeonInfoText.setString(this->graph->PrintGraph());

    fitDungeonToView(*this->graph, dungeonCamera);
}