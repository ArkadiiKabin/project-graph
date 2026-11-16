#include <algorithm>
#include <climits>
#include <cstdlib>
#include <functional>
#include <queue>
#include <random>
#include <unordered_set>

#include "Graph.hpp"

void Graph::addRoom(int id) {
    Room newRoom;
    newRoom.id = id;

    allRooms.push_back(newRoom);
}

void Graph::GenerationRooms() {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dist(settings.minRoomCount, settings.maxRoomCount);
    int roomCount = dist(gen);

    const int cellSize = 180;

    uniform_int_distribution<> textureDist(1, 5);

    vector<sf::Vector2i> positions;
    positions.push_back({0, 0});

    // случайное расположение комнат
    while (positions.size() < roomCount) {
        vector<int> availableRooms;

        for (int i = 0; i < positions.size(); ++i) {
            for (const auto& dir : vector<sf::Vector2i>{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}) {
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

        uniform_int_distribution<> roomDist(0, availableRooms.size() - 1);
        int index = availableRooms[roomDist(gen)];

        vector<sf::Vector2i> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        shuffle(directions.begin(), directions.end(), gen);

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

    // определение границ подземелья
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

    // центрирование подземелья
    float dungeonSize = 800.f;
    float roomRadius = 14.f;

    float offsetX = (dungeonSize - (maxX - minX) * cellSize) / 2.f;
    float offsetY = (dungeonSize - (maxY - minY) * cellSize) / 2.f;

    offsetX = max(offsetX, roomRadius);
    offsetY = max(offsetY, roomRadius);

    // создание комнат
    for (int i = 0; i < roomCount; ++i) {
        addRoom(i);

        allRooms[i].textureSet = textureDist(gen);
        allRooms[i].gridPosition = positions[i];

        allRooms[i].position = {20.f + offsetX + (positions[i].x - minX) * cellSize,
                                20.f + offsetY + (positions[i].y - minY) * cellSize};
    }
}

void Graph::connectRooms(int room1, int room2, int weight) {
    allRooms[room1].connectedRooms.push_back({room2, weight});
    allRooms[room2].connectedRooms.push_back({room1, weight});
}

void Graph::generateConnections() {
    if (allRooms.empty()) { return; }

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> weightDist(settings.minWeightPath, settings.maxWeightPath);

    // проверка ортогонального соседства
    auto areNeighbors = [&](int i, int j) {
        int dx = abs(allRooms[i].gridPosition.x - allRooms[j].gridPosition.x);
        int dy = abs(allRooms[i].gridPosition.y - allRooms[j].gridPosition.y);

        return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
    };

    // проверка существующего соединения
    auto alreadyConnected = [&](int i, int j) {
        for (const auto& connection : allRooms[i].connectedRooms) {
            if (connection.roomId == j) { return true; }
        }

        return false;
    };

    // создание обязательных соединений для связности
    for (int i = 1; i < allRooms.size(); ++i) {
        int bestNeighbor = -1;
        int minConnections = INT_MAX;

        for (int j = 0; j < i; ++j) {
            if (areNeighbors(i, j)) {
                int connections = allRooms[j].connectedRooms.size();

                if (connections < minConnections) {
                    minConnections = connections;
                    bestNeighbor = j;
                }
            }
        }

        if (bestNeighbor != -1) { connectRooms(i, bestNeighbor, weightDist(gen)); }
    }

    // добавление дополнительных соединений
    int maxConnections = settings.density + 1;

    for (int i = 0; i < allRooms.size(); ++i) {
        for (int j = i + 1; j < allRooms.size(); ++j) {
            if (!areNeighbors(i, j)) { continue; }

            if (alreadyConnected(i, j)) { continue; }

            if (allRooms[i].connectedRooms.size() < maxConnections &&
                allRooms[j].connectedRooms.size() < maxConnections) {
                connectRooms(i, j, weightDist(gen));
            }
        }
    }
}

int Graph::getRoomCount() const {
    return allRooms.size();
}

const Room& Graph::getRoom(int id) const {
    return allRooms[id];
}

string Graph::PrintGraph() const {
    string result;

    for (int i = 0; i < allRooms.size(); ++i) {
        result += "Room " + to_string(allRooms[i].id) + " is connected to: ";

        for (const Connection& connection : allRooms[i].connectedRooms) {
            result += to_string(connection.roomId);
            result += "(weight: " + to_string(connection.weight) + ") ";
        }

        result += "\n";
    }

    return result;
}

vector<int> Graph::BFS(int startRoomId, int endRoomId) {
    lastVisitedCount = 0;

    // проверка ID комнат
    if (startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()) {
        return {};
    }

    queue<int> roomsQueue;
    unordered_set<int> visited;

    vector<int> parent(allRooms.size(), -1);

    visited.insert(startRoomId);
    lastVisitedCount++;

    roomsQueue.push(startRoomId);

    // обход графа в ширину
    while (!roomsQueue.empty()) {
        int currentRoomId = roomsQueue.front();
        roomsQueue.pop();

        if (currentRoomId == endRoomId) { break; }

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

    if (visited.find(endRoomId) == visited.end()) { return {}; }

    // восстановление маршрута
    vector<int> path;
    int current = endRoomId;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

vector<int> Graph::DFS(int startRoomId, int endRoomId) {
    lastVisitedCount = 0;

    // проверка ID комнат
    if (startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()) {
        return {};
    }

    visited.assign(allRooms.size(), false);
    targetRoomId = endRoomId;

    vector<int> path;
    DFSRecursive(startRoomId, path);

    if (path.empty() || path.back() != endRoomId) { return {}; }

    return path;
}

bool Graph::DFSRecursive(int roomId, vector<int>& path) {
    visited[roomId] = true;
    lastVisitedCount++;

    path.push_back(roomId);

    if (roomId == targetRoomId) { return true; }

    const Room& currentRoom = getRoom(roomId);

    // обход соседних комнат
    for (const Connection& connection : currentRoom.connectedRooms) {
        int neighborId = connection.roomId;

        if (!visited[neighborId]) {
            if (DFSRecursive(neighborId, path)) { return true; }
        }
    }

    path.pop_back();

    return false;
}

vector<int> Graph::Dijkstra(int startRoomId, int endRoomId) {
    // проверка ID комнат
    if (startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()) {
        return {};
    }

    vector<int> distance(allRooms.size(), INT_MAX);
    distance[startRoomId] = 0;

    vector<int> parent(allRooms.size(), -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({distance[startRoomId], startRoomId});

    // обход графа с учётом весов
    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentRoomId = pq.top().second;

        pq.pop();

        if (currentDistance > distance[currentRoomId]) { continue; }

        const Room& currentRoom = getRoom(currentRoomId);

        for (const Connection& connection : currentRoom.connectedRooms) {
            int neighborId = connection.roomId;
            int newDist = currentDistance + connection.weight;

            if (newDist < distance[neighborId]) {
                distance[neighborId] = newDist;
                parent[neighborId] = currentRoomId;

                pq.push({newDist, neighborId});
            }
        }
    }

    if (distance[endRoomId] == INT_MAX) { return {}; }

    // восстановление маршрута
    vector<int> path;
    int current = endRoomId;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

void Graph::clear() {
    allRooms.clear();
    visited.clear();
    targetRoomId = -1;
}

int Graph::getLastVisitedCount() const {
    return lastVisitedCount;
}

void Graph::setSettings(const Settings& newSettings) {
    settings = newSettings;
}