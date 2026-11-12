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
    void arrangeRooms();
    int targetRoomId = -1;
    bool DFSRecursive(int roomId, vector<int>& path);
    public:
    void addRoom(int id);
    void GenerationRooms();
    void connectRooms(int room1, int room2, int weight);
    void generateConnections();
    int getRoomCount() const;
    const Room& getRoom(int id) const;
    string PrintGraph() const;
    vector<int> BFS(int startRoomId, int endRoomId) const;
    vector<int> DFS(int startRoomId, int endRoomId);
    vector<int> Dijkstra(int startRoomId, int endRoomId);
    void clear();
};