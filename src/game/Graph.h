#pragma once
#include <vector>
using namespace std;

struct Room {
    int id;
    vector<int> connectedRooms;
};

class Graph{
    private:
    vector<bool> visited;
    vector<Room> allRooms;
    void DFSRecursive(int roomId);
    public:
    void addRoom(int id);
    void GenerationRooms();
    void connectRooms(int room1, int room2);
    void generateConnections();
    int getRoomCount() const;
    const Room& getRoom(int id) const;
    void PrintGraph() const;
    void BFS(int startRoomId, int endRoomId) const;
    void DFS(int roomId);
};