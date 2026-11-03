#include "Graph.h"
#include <random>
#include <iostream>
#include <queue>
#include <unordered_set>

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
    for(int i = 0; i < roomCount; ++i){
        addRoom(i);
    }
}

void Graph::connectRooms(int room1, int room2){
    allRooms[room1].connectedRooms.push_back(room2);
    allRooms[room2].connectedRooms.push_back(room1);
}

void Graph::generateConnections(){
    for(int i = 0; i < allRooms.size() - 1;++i){
        connectRooms(i, i + 1);
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);
    for(int i = 0; i < allRooms.size(); ++i){
        for(int j = i + 1; j < allRooms.size(); ++j){
            if(j == i + 1) continue;
            int connectionChance = dist(gen);
            if(connectionChance == 1) connectRooms(i, j);
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
        for (int neighbor : allRooms[i].connectedRooms) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

void Graph::BFS(int startRoomId) const{
    queue<int> roomsQueue;
    unordered_set<int> visited;
    vector<int> parent(allRooms.size(), -1);
    visited.insert(startRoomId);
    roomsQueue.push(startRoomId);
    while(!roomsQueue.empty()){
        int currentRoomId = roomsQueue.front();
        roomsQueue.pop();
        cout << "Visited room: " << currentRoomId << endl;
        const Room& currentRoom = getRoom(currentRoomId);
        for(int neighborId : currentRoom.connectedRooms){
            if (visited.insert(neighborId).second) {
                parent[neighborId] = currentRoomId;
                roomsQueue.push(neighborId);
            }
        }
    }
}