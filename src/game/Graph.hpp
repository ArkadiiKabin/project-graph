#pragma once

#include <SFML/Graphics.hpp>
#include <string>
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

struct Settings {
    int minRoomCount = 8;
    int maxRoomCount = 15;
    int minWeightPath = 1;
    int maxWeightPath = 10;
    int density = 2; // 1 - низкая, 2 - средняя, 3 - высокая
};

class Graph {
  private:
    vector<bool> visited;
    vector<Room> allRooms;

    int lastVisitedCount = 0;
    int targetRoomId = -1;

    Settings settings;

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
    vector<int> Dijkstra(int startRoomId, int endRoomId);

    int getLastVisitedCount() const;

    void setSettings(const Settings& newSettings);
    void clear();
};