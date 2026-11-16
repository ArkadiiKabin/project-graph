#include <algorithm>
#include <cmath>
#include <string>

#include "DungeonRenderer.hpp"

using namespace std;

// конструктор рендерера подземелья
DungeonRenderer::DungeonRenderer(sf::Font& font) : font(font) {
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
}

// подгоняет камеру под размеры подземелья
void DungeonRenderer::fitToView(const Graph& graph, sf::View& camera) const {
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

// отрисовка подземелья
void DungeonRenderer::draw(sf::RenderWindow& window, const Graph& graph, const vector<int>& bfsPath,
                           const vector<int>& dfsPath, const vector<int>& dijPath) const {
    // отрисовка дорог между комнатами
    for (int i = 0; i < graph.getRoomCount(); i++) {
        const Room& room = graph.getRoom(i);

        for (const Connection& connection : room.connectedRooms) {
            if (connection.roomId <= room.id) { continue; }

            const Room& neighbor = graph.getRoom(connection.roomId);

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
    for (int i = 0; i < graph.getRoomCount(); i++) {
        const Room& room = graph.getRoom(i);

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
                const Room& neighbor = graph.getRoom(connection.roomId);

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
}