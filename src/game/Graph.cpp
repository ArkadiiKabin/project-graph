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
    uniform_int_distribution<> xDist(50, 540);
    uniform_int_distribution<> yDist(50, 490);
    int roomCount = dist(gen);
    for(int i = 0; i < roomCount; ++i){
        addRoom(i);
        bool validPosition = false;
        while(!validPosition){
            float x = static_cast<float>(xDist(gen));
            float y = static_cast<float>(yDist(gen));
            validPosition = true;
            for(int j = 0; j < i; ++j){
                float dx = x - allRooms[j].position.x;
                float dy = y - allRooms[j].position.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                if(distance < 40.f){
                    validPosition = false;
                    break;
                }
            }
            if(validPosition){
                allRooms[i].position = {x, y};
            }
        }
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
            if(allRooms[i].connectedRooms.size() >= 4) break;
            if(allRooms[j].connectedRooms.size() >= 4) continue;
            if(j == i + 1) continue;
            int connectionChance = chanceDist(gen);
            if(connectionChance == 1) connectRooms(i, j, weightDist(gen));
        }
    }
}

int Graph::getRoomCount() const{
    return allRooms.size();
}

const Room& Graph::getRoom(int id) const{
    return allRooms[id];
}

void Graph::PrintGraph() const{
    for(int i = 0; i < allRooms.size(); i++){
        cout << "Room " << allRooms[i].id << " is connected to: ";
        for (const Connection& connection : allRooms[i].connectedRooms) {
            cout << connection.roomId << "(weight: " << connection.weight << ") ";
        }
        cout << endl;
    }
}

void Graph::BFS(int startRoomId, int endRoomId) const {
    if(startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return;
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
        return;
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
}

void Graph::DFS(int roomId){
    if(roomId < 0 || roomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return;
    }
    visited.assign(allRooms.size(), false);
    DFSRecursive(roomId);
}

void Graph::DFSRecursive(int roomId){
    visited[roomId] = true;
    cout << "Visited room: " << roomId << endl;
    const Room& currentRoom = getRoom(roomId);
    for (const Connection& connection : currentRoom.connectedRooms){
        int neighborId = connection.roomId;
        if (!visited[neighborId]){
            DFSRecursive(neighborId);
        }
    }
}

void Graph::Dijkstra(int startRoomId, int endRoomId){
    if(startRoomId < 0 || startRoomId >= allRooms.size() || endRoomId < 0 || endRoomId >= allRooms.size()){
        cout << "Invalid room ID!" << endl;
        return;
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
    if(distance[endRoomId] == INT_MAX) {cout << "not found"; return;}
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
}