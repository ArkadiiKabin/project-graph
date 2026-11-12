#include <algorithm>
#include <climits>
#include <functional>
#include <queue>
#include <random>
#include <unordered_set>

#include "Graph.hpp"

// Добавление комнаты в граф.
void Graph::addRoom(int id) {
    Room newRoom;
    newRoom.id = id;

    allRooms.push_back(newRoom);
}

// Процедурная генерация комнат.
void Graph::GenerationRooms() {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dist(8, 15);
    int roomCount = dist(gen);

    const int cellSize = 180;

    uniform_int_distribution<> textureDist(1, 5);

    vector<sf::Vector2i> positions;
    positions.push_back({0, 0});

    // Формирование случайного расположения комнат.
    while (positions.size() < roomCount) {
        vector<int> availableRooms;

        for (int i = 0; i < positions.size(); ++i) {
            for (const auto& dir : vector<sf::Vector2i>{
                {0, -1}, {0, 1}, {-1, 0}, {1, 0}
            }) {
                sf::Vector2i newPos = positions[i] + dir;

                bool occupied = false;

                for (const auto& pos : positions) {
                    if (pos == newPos) {
                        occupied = true;
                        break;
                    }
                }

                if (!occupied) {
                    availableRooms.push_back(i);
                    break;
                }
            }
        }

        // Выбор случайной существующей комнаты.
        uniform_int_distribution<> roomDist(
            0,
            availableRooms.size() - 1
        );

        int index = availableRooms[roomDist(gen)];

        // Перемешивание направлений.
        vector<sf::Vector2i> directions = {
            {0, -1},
            {0, 1},
            {-1, 0},
            {1, 0}
        };

        shuffle(directions.begin(), directions.end(), gen);

        // Поиск свободной соседней клетки.
        for (const auto& dir : directions) {
            sf::Vector2i newPos = positions[index] + dir;

            bool occupied = false;

            for (const auto& pos : positions) {
                if (pos == newPos) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied) {
                positions.push_back(newPos);
                break;
            }
        }
    }

    // Определение границ расположения комнат.
    int minX = positions[0].x;
    int maxX = positions[0].x;
    int minY = positions[0].y;
    int maxY = positions[0].y;

    for (const auto& pos : positions) {
        minX = min(minX, pos.x);
        maxX = max(maxX, pos.x);
        minY = min(minY, pos.y);
        maxY = max(maxY, pos.y);
    }

    // Расчёт смещения для центрирования подземелья.
    float dungeonSize = 800.f;
    float roomRadius = 14.f;

    float offsetX = (dungeonSize - (maxX - minX) * cellSize) / 2.f;
    float offsetY = (dungeonSize - (maxY - minY) * cellSize) / 2.f;

    offsetX = max(offsetX, roomRadius);
    offsetY = max(offsetY, roomRadius);

    // Создание комнат и назначение их координат.
    for (int i = 0; i < roomCount; ++i) {
        addRoom(i);

        allRooms[i].textureSet = textureDist(gen);
        allRooms[i].gridPosition = positions[i];

        allRooms[i].position = {
            20.f + offsetX + (positions[i].x - minX) * cellSize,
            20.f + offsetY + (positions[i].y - minY) * cellSize
        };
    }
}

// Создание двустороннего соединения между комнатами.
void Graph::connectRooms(int room1, int room2, int weight) {
    allRooms[room1].connectedRooms.push_back({room2, weight});
    allRooms[room2].connectedRooms.push_back({room1, weight});
}

// Генерация соединений между соседними комнатами.
void Graph::generateConnections() {
    if (allRooms.empty()) {
        return;
    }

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> weightDist(1, 10);

    for (int i = 0; i < allRooms.size(); ++i) {
        for (int j = i + 1; j < allRooms.size(); ++j) {
            int dx = allRooms[i].gridPosition.x - allRooms[j].gridPosition.x;
            int dy = allRooms[i].gridPosition.y - allRooms[j].gridPosition.y;

            // Соединение только ортогонально соседних комнат.
            if (
                ((dx == 1 || dx == -1) && dy == 0) ||
                ((dy == -1 || dy == 1) && dx == 0)
            ) {
                connectRooms(i, j, weightDist(gen));
            }
        }
    }
}

// Получение количества комнат.
int Graph::getRoomCount() const {
    return allRooms.size();
}

// Получение комнаты по её ID.
const Room& Graph::getRoom(int id) const {
    return allRooms[id];
}

// Формирование текстового представления графа.
std::string Graph::PrintGraph() const {
    std::string result;

    for (int i = 0; i < allRooms.size(); i++) {
        result +=
            "Room " + std::to_string(allRooms[i].id) +
            " is connected to: ";

        for (const Connection& connection : allRooms[i].connectedRooms) {
            result += std::to_string(connection.roomId);
            result += "(weight: " + std::to_string(connection.weight) + ") ";
        }

        result += "\n";
    }

    return result;
}

// Поиск в ширину (Breadth-First Search).
vector<int> Graph::BFS(int startRoomId, int endRoomId) {
    lastVisitedCount = 0;

    // Проверка корректности ID комнат.
    if (
        startRoomId < 0 ||
        startRoomId >= allRooms.size() ||
        endRoomId < 0 ||
        endRoomId >= allRooms.size()
    ) {
        return {};
    }

    queue<int> roomsQueue;
    unordered_set<int> visited;

    vector<int> parent(allRooms.size(), -1);

    visited.insert(startRoomId);
    lastVisitedCount++;

    roomsQueue.push(startRoomId);

    // Обход графа в ширину.
    while (!roomsQueue.empty()) {
        int currentRoomId = roomsQueue.front();
        roomsQueue.pop();

        if (currentRoomId == endRoomId) {
            break;
        }

        const Room& currentRoom = getRoom(currentRoomId);

        for (const Connection& connection : currentRoom.connectedRooms) {
            int neighborId = connection.roomId;

            if (visited.insert(neighborId).second) {
                parent[neighborId] = currentRoomId;
                lastVisitedCount++;

                roomsQueue.push(neighborId);
            }
        }
    }

    // Проверка наличия маршрута.
    if (visited.find(endRoomId) == visited.end()) {
        return {};
    }

    // Восстановление найденного маршрута.
    vector<int> path;

    int current = endRoomId;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

// Поиск в глубину (Depth-First Search).
vector<int> Graph::DFS(int startRoomId, int endRoomId) {
    lastVisitedCount = 0;

    // Проверка корректности ID комнат.
    if (
        startRoomId < 0 ||
        startRoomId >= allRooms.size() ||
        endRoomId < 0 ||
        endRoomId >= allRooms.size()
    ) {
        return {};
    }

    visited.assign(allRooms.size(), false);
    targetRoomId = endRoomId;

    vector<int> path;

    DFSRecursive(startRoomId, path);

    // Проверка результата поиска.
    if (path.empty() || path.back() != endRoomId) {
        return {};
    }

    return path;
}

// Рекурсивный обход графа в глубину.
bool Graph::DFSRecursive(int roomId, vector<int>& path) {
    visited[roomId] = true;
    lastVisitedCount++;

    path.push_back(roomId);

    // Проверка достижения целевой комнаты.
    if (roomId == targetRoomId) {
        return true;
    }

    const Room& currentRoom = getRoom(roomId);

    // Рекурсивный обход соседних комнат.
    for (const Connection& connection : currentRoom.connectedRooms) {
        int neighborId = connection.roomId;

        if (!visited[neighborId]) {
            if (DFSRecursive(neighborId, path)) {
                return true;
            }
        }
    }

    // Возврат при отсутствии подходящего пути.
    path.pop_back();

    return false;
}

// Поиск кратчайшего маршрута алгоритмом Дейкстры.
std::vector<int> Graph::Dijkstra(int startRoomId, int endRoomId) {
    // Проверка корректности ID комнат.
    if (
        startRoomId < 0 ||
        startRoomId >= allRooms.size() ||
        endRoomId < 0 ||
        endRoomId >= allRooms.size()
    ) {
        return {};
    }

    vector<int> distance(allRooms.size(), INT_MAX);
    distance[startRoomId] = 0;

    vector<int> parent(allRooms.size(), -1);

    // Очередь с приоритетом для выбора вершины
    // с минимальным текущим расстоянием.
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    pq.push({distance[startRoomId], startRoomId});

    // Обход графа с учётом весов соединений.
    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentRoomId = pq.top().second;

        pq.pop();

        // Пропуск устаревшего значения расстояния.
        if (currentDistance > distance[currentRoomId]) {
            continue;
        }

        const Room& currentRoom = getRoom(currentRoomId);

        for (const Connection& connection : currentRoom.connectedRooms) {
            int neighborId = connection.roomId;
            int newDist = currentDistance + connection.weight;

            // Обновление расстояния до соседней комнаты.
            if (newDist < distance[neighborId]) {
                distance[neighborId] = newDist;
                parent[neighborId] = currentRoomId;

                pq.push({newDist, neighborId});
            }
        }
    }

    // Проверка наличия маршрута.
    if (distance[endRoomId] == INT_MAX) {
        return {};
    }

    // Восстановление найденного маршрута.
    vector<int> path;

    int current = endRoomId;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

// Очистка графа.
void Graph::clear() {
    allRooms.clear();
    visited.clear();
    targetRoomId = -1;
}

// Получение количества посещённых вершин.
int Graph::getLastVisitedCount() const {
    return lastVisitedCount;
}