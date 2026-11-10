#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

struct Connection {
    int roomId;
    int weight;
};

struct Room {
    int id;
    sf::Vector2f position;
    vector<Connection> connectedRooms;
};

class Graph{
    private:
    vector<bool> visited;
    vector<Room> allRooms;
    void DFSRecursive(int roomId);
    public:
    void addRoom(int id);
    void GenerationRooms();
    void connectRooms(int room1, int room2, int weight);
    void generateConnections();
    int getRoomCount() const;
    const Room& getRoom(int id) const;
    void PrintGraph() const;
    void BFS(int startRoomId, int endRoomId) const;
    void DFS(int roomId);
    void Dijkstra(int startRoomId, int endRoomId);
};