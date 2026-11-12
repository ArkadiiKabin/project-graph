#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

struct Connection {
    int roomId;
    int weight;
};

struct Room {
    int id;
    sf::Vector2f position;
    sf::Vector2i gridPosition;
    vector<Connection> connectedRooms;
    int textureSet = 1;
};

class Graph {
   private:
    vector<bool> visited;
    vector<Room> allRooms;
    int lastVisitedCount = 0;
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
    vector<int> BFS(int startRoomId, int endRoomId);
    vector<int> DFS(int startRoomId, int endRoomId);
    int getLastVisitedCount() const;
    vector<int> Dijkstra(int startRoomId, int endRoomId);
    void clear();
};