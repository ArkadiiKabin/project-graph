#include "Graph.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <climits>

void Graph::addRoom(int id){
    Room newRoom;
    newRoom.id = id;
    allRooms.push_back(newRoom);
}

void Graph::GenerationRooms(){
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dist(10, 20);
    int roomCount = dist(gen);

    // Определяем количество столбцов и строк
    int columns = static_cast<int>(ceil(sqrt(roomCount)));
    int rows = (roomCount + columns - 1) / columns;

    // Размер одной ячейки
    float cellWidth = 700.f / columns;
    float cellHeight = 700.f / rows;

    for(int i = 0; i < roomCount; ++i){
        addRoom(i);

        int row = i / columns;
        int column = i % columns;

        // Чередуем направление заполнения строк
        if(row % 2 == 1){
            column = columns - 1 - column;
        }

        // Случайное положение внутри ячейки
        uniform_real_distribution<float> xDist(20.f, cellWidth - 20.f);
        uniform_real_distribution<float> yDist(20.f, cellHeight - 20.f);

        float x = 50.f + column * cellWidth + xDist(gen);
        float y = 50.f + row * cellHeight + yDist(gen);

        allRooms[i].position = {x, y};
    }
}

void Graph::connectRooms(int room1, int room2, int weight){
    allRooms[room1].connectedRooms.push_back({room2, weight});
    allRooms[room2].connectedRooms.push_back({room1, weight});
}

void Graph::generateConnections(){
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> chanceDist(0, 1);
    uniform_int_distribution<> weightDist(1, 10);
    for(int i = 0; i < allRooms.size() - 1;++i){
        connectRooms(i, i + 1, weightDist(gen));
    }
    for(int i = 0; i < allRooms.size(); ++i){
        for(int j = i + 1; j < allRooms.size(); ++j){
            if(allRooms[i].connectedRooms.size() >= 3) break;
            if(allRooms[j].connectedRooms.size() >= 3) continue;
            if(j == i + 1) continue;
            int connectionChance = chanceDist(gen);
            if(connectionChance == 1) connectRooms(i, j, weightDist(gen));
        }
    }
    arrangeRooms();
}

void Graph::arrangeRooms() {
    int roomCount = allRooms.size();

    if (roomCount == 0) return;

    // Создаём слои комнат
    vector<vector<int>> layers;
    vector<int> distances(roomCount, -1);
    queue<int> q;

    // Начинаем обход с комнаты 0
    distances[0] = 0;
    q.push(0);

    // Распределяем комнаты по слоям с помощью BFS
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        int layer = distances[current];

        if (layers.size() <= layer) {
            layers.resize(layer + 1);
        }

        layers[layer].push_back(current);

        for (const Connection& connection : allRooms[current].connectedRooms) {
            int neighbor = connection.roomId;

            if (distances[neighbor] == -1) {
                distances[neighbor] = layer + 1;
                q.push(neighbor);
            }
        }
    }

    // Находим количество слоёв
    int layerCount = layers.size();

    // Расставляем комнаты
    for (int i = 0; i < layerCount; i++) {
        int roomsInLayer = layers[i].size();

        float x = 70.f + i * (660.f / max(1, layerCount - 1));

        for (int j = 0; j < roomsInLayer; j++) {
            float y = 70.f + (j + 1) * (660.f / (roomsInLayer + 1));
            int roomId = layers[i][j];

            allRooms[roomId].position = {x, y};
        }
    }
}

int Graph::getRoomCount() const{
    return allRooms.size();
}

const Room& Graph::getRoom(int id) const{
    return allRooms[id];
}

std::string Graph::PrintGraph() const {
    std::string result;

    for (int i = 0; i < allRooms.size(); i++) {
        result += "Room " + std::to_string(allRooms[i].id) + " is connected to: ";

        for (const Connection& connection : allRooms[i].connectedRooms) {
            result += std::to_string(connection.roomId);
            result += "(weight: " + std::to_string(connection.weight) + ") ";
        }

        result += "\n";
    }

    return result;
}

vector<int> Graph::BFS(int startRoomId, int endRoomId) const {
    if(startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return {};
    }
    queue<int> roomsQueue;
    unordered_set<int> visited;
    vector<int> parent(allRooms.size(), -1);

    visited.insert(startRoomId);
    roomsQueue.push(startRoomId);

    while(!roomsQueue.empty()){
        int currentRoomId = roomsQueue.front();
        roomsQueue.pop();
        cout << "Visited room: " << currentRoomId << endl;
        if(currentRoomId == endRoomId){
            break;
        }
        const Room& currentRoom = getRoom(currentRoomId);
        for(const Connection& connection : currentRoom.connectedRooms){
            int neighborId = connection.roomId;
            if (visited.insert(neighborId).second) {
                parent[neighborId] = currentRoomId;
                roomsQueue.push(neighborId);
            }
        }
    }

    if(visited.find(endRoomId) == visited.end()){
        cout << "Path not found!" << endl;
        return {};
    }
    vector<int> path;
    int current = endRoomId;
    while(current != -1){
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    cout << "Shortest path: ";
    for(int roomId : path){
        cout << roomId << " ";
    }
    cout << endl;
    return path;
}

vector<int> Graph::DFS(int startRoomId, int endRoomId){
    if(startRoomId < 0 || startRoomId >= allRooms.size() ||
       endRoomId < 0 || endRoomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return {};
    }

    visited.assign(allRooms.size(), false);
    targetRoomId = endRoomId;

    vector<int> path;
    DFSRecursive(startRoomId, path);

    if(path.empty() || path.back() != endRoomId){
        cout << "Path not found!" << endl;
        return {};
    }

    return path;
}

bool Graph::DFSRecursive(int roomId, vector<int>& path){
    visited[roomId] = true;
    path.push_back(roomId);

    cout << "Visited room: " << roomId << endl;

    if(roomId == targetRoomId){
        return true;
    }

    const Room& currentRoom = getRoom(roomId);

    for(const Connection& connection : currentRoom.connectedRooms){
        int neighborId = connection.roomId;

        if(!visited[neighborId]){
            if(DFSRecursive(neighborId, path)){
                return true;
            }
        }
    }

    path.pop_back();
    return false;
}

std::vector<int> Graph::Dijkstra(int startRoomId, int endRoomId){
    if(startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return {};
    }
    vector<int> distance (allRooms.size(), INT_MAX);
    distance[startRoomId] = 0;
    vector<int> parent(allRooms.size(), -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({distance[startRoomId], startRoomId});
    while(!pq.empty()){
        int currentDistance = pq.top().first;
        int currentRoomId = pq.top().second;
        pq.pop();
        if(currentDistance > distance[currentRoomId]){
            continue;
        }
        const Room& currentRoom = getRoom(currentRoomId);
        for(const Connection& connection : currentRoom.connectedRooms){
            int neighborId = connection.roomId;
            int newDist = currentDistance + connection.weight;
            if(newDist < distance[neighborId]){
                distance[neighborId] = newDist;
                parent[neighborId] = currentRoomId;
                pq.push({newDist, neighborId});
            }
        }
    }
    if(distance[endRoomId] == INT_MAX) {cout << "not found"; return {};}
    vector<int> path;
    int current = endRoomId;
    while(current != -1){
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    cout << "Shortest path: ";
    for(int roomId : path){
        cout << roomId << " ";
    }
    cout << "Total cost: " << distance[endRoomId] << "\n\n";
    return path;
}

void Graph::clear(){
    allRooms.clear();
    visited.clear();
    targetRoomId = -1;
}