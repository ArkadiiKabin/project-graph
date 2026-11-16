#include <gtest/gtest.h>
#include "Graph.hpp"
#include <algorithm>
#include <climits>
#include <string>
#include <vector>

// =====================================================
// БЛОК 1. СОЗДАНИЕ И ХРАНЕНИЕ КОМНАТ — 10 ТЕСТОВ
// =====================================================

TEST(Block1_Rooms, AddOneRoom) {
    Graph g;
    g.addRoom(0);
    EXPECT_EQ(g.getRoomCount(), 1);
}

TEST(Block1_Rooms, AddFiveRooms) {
    Graph g;
    for (int i = 0; i < 5; ++i) g.addRoom(i);
    EXPECT_EQ(g.getRoomCount(), 5);
}

TEST(Block1_Rooms, AddTenRooms) {
    Graph g;
    for (int i = 0; i < 10; ++i) g.addRoom(i);
    EXPECT_EQ(g.getRoomCount(), 10);
}

TEST(Block1_Rooms, FirstRoomHasCorrectId) {
    Graph g;
    g.addRoom(7);
    EXPECT_EQ(g.getRoom(0).id, 7);
}

TEST(Block1_Rooms, LastRoomHasCorrectId) {
    Graph g;
    for (int i = 0; i < 5; ++i) g.addRoom(i);
    EXPECT_EQ(g.getRoom(4).id, 4);
}

TEST(Block1_Rooms, DuplicateIdsAreStored) {
    Graph g;
    g.addRoom(1);
    g.addRoom(1);
    EXPECT_EQ(g.getRoomCount(), 2);
}

TEST(Block1_Rooms, NegativeIdCanBeStored) {
    Graph g;
    g.addRoom(-5);
    EXPECT_EQ(g.getRoom(0).id, -5);
}

TEST(Block1_Rooms, EmptyGraphHasZeroRooms) {
    Graph g;
    EXPECT_EQ(g.getRoomCount(), 0);
}

TEST(Block1_Rooms, RoomsAreStoredInInsertionOrder) {
    Graph g;
    g.addRoom(5);
    g.addRoom(2);
    g.addRoom(9);
    EXPECT_EQ(g.getRoom(0).id, 5);
    EXPECT_EQ(g.getRoom(1).id, 2);
    EXPECT_EQ(g.getRoom(2).id, 9);
}

TEST(Block1_Rooms, RoomPositionDefaultsToZero) {
    Graph g;
    g.addRoom(0);
    EXPECT_FLOAT_EQ(g.getRoom(0).position.x, 0.f);
    EXPECT_FLOAT_EQ(g.getRoom(0).position.y, 0.f);
}


// =====================================================
// БЛОК 2. СОЕДИНЕНИЯ КОМНАТ — 10 ТЕСТОВ
// =====================================================

TEST(Block2_Connections, ConnectTwoRooms) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 5);
    EXPECT_EQ(g.getRoom(0).connectedRooms.size(), 1);
}

TEST(Block2_Connections, ConnectionIsSymmetric) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 5);
    EXPECT_EQ(g.getRoom(1).connectedRooms[0].roomId, 0);
}

TEST(Block2_Connections, WeightIsStored) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 7);
    EXPECT_EQ(g.getRoom(0).connectedRooms[0].weight, 7);
}

TEST(Block2_Connections, ZeroWeightIsAccepted) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 0);
    EXPECT_EQ(g.getRoom(0).connectedRooms[0].weight, 0);
}

TEST(Block2_Connections, NegativeWeightIsAccepted) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, -3);
    EXPECT_EQ(g.getRoom(0).connectedRooms[0].weight, -3);
}

TEST(Block2_Connections, MultipleConnections) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 2);
    g.connectRooms(0, 2, 3);
    EXPECT_EQ(g.getRoom(0).connectedRooms.size(), 2);
}

TEST(Block2_Connections, DuplicateConnectionIsStored) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 2);
    g.connectRooms(0, 1, 3);
    EXPECT_EQ(g.getRoom(0).connectedRooms.size(), 2);
}

TEST(Block2_Connections, SelfConnectionIsStored) {
    Graph g;
    g.addRoom(0);
    g.connectRooms(0, 0, 4);
    EXPECT_EQ(g.getRoom(0).connectedRooms.size(), 2);
}

TEST(Block2_Connections, ThreeRoomsCanBeConnected) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 2);
    EXPECT_EQ(g.getRoom(1).connectedRooms.size(), 2);
}

TEST(Block2_Connections, LargeWeightIsStored) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 100000);
    EXPECT_EQ(g.getRoom(0).connectedRooms[0].weight, 100000);
}


// =====================================================
// БЛОК 3. НАСТРОЙКИ — 10 ТЕСТОВ
// =====================================================

TEST(Block3_Settings, DefaultMinimumRoomCount) {
    Settings s;
    EXPECT_EQ(s.minRoomCount, 8);
}

TEST(Block3_Settings, DefaultMaximumRoomCount) {
    Settings s;
    EXPECT_EQ(s.maxRoomCount, 15);
}

TEST(Block3_Settings, DefaultMinimumWeight) {
    Settings s;
    EXPECT_EQ(s.minWeightPath, 1);
}

TEST(Block3_Settings, DefaultMaximumWeight) {
    Settings s;
    EXPECT_EQ(s.maxWeightPath, 10);
}

TEST(Block3_Settings, DefaultDensity) {
    Settings s;
    EXPECT_EQ(s.density, 2);
}

TEST(Block3_Settings, CustomRoomSettings) {
    Graph g;
    Settings s;
    s.minRoomCount = 4;
    s.maxRoomCount = 4;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 4);
}

TEST(Block3_Settings, MinimumRoomCountSetting) {
    Graph g;
    Settings s;
    s.minRoomCount = 1;
    s.maxRoomCount = 1;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 1);
}

TEST(Block3_Settings, MaximumRoomCountSetting) {
    Graph g;
    Settings s;
    s.minRoomCount = 20;
    s.maxRoomCount = 20;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 20);
}

TEST(Block3_Settings, CustomWeightSettings) {
    Graph g;
    Settings s;
    s.minWeightPath = 4;
    s.maxWeightPath = 4;
    g.setSettings(s);
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, s.minWeightPath);
    EXPECT_EQ(g.getRoom(0).connectedRooms[0].weight, 4);
}

TEST(Block3_Settings, CustomDensitySetting) {
    Settings s;
    s.density = 3;
    EXPECT_EQ(s.density, 3);
}


// =====================================================
// БЛОК 4. ГЕНЕРАЦИЯ КОМНАТ — 10 ТЕСТОВ
// =====================================================

TEST(Block4_Generation, GeneratesOneRoom) {
    Graph g;
    Settings s;
    s.minRoomCount = 1;
    s.maxRoomCount = 1;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 1);
}

TEST(Block4_Generation, GeneratesFiveRooms) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 5);
}

TEST(Block4_Generation, GeneratesTenRooms) {
    Graph g;
    Settings s;
    s.minRoomCount = 10;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 10);
}

TEST(Block4_Generation, GeneratesRoomsWithinRange) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();
    EXPECT_GE(g.getRoomCount(), 5);
    EXPECT_LE(g.getRoomCount(), 10);
}

TEST(Block4_Generation, GeneratedIdsAreSequential) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    g.setSettings(s);
    g.GenerationRooms();
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(g.getRoom(i).id, i);
}

TEST(Block4_Generation, TextureSetsAreValid) {
    Graph g;
    Settings s;
    s.minRoomCount = 10;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();
    for (int i = 0; i < 10; ++i) {
        EXPECT_GE(g.getRoom(i).textureSet, 1);
        EXPECT_LE(g.getRoom(i).textureSet, 5);
    }
}

TEST(Block4_Generation, GridPositionsAreUnique) {
    Graph g;
    Settings s;
    s.minRoomCount = 10;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();

    for (int i = 0; i < 10; ++i) {
        for (int j = i + 1; j < 10; ++j) {
            EXPECT_FALSE(g.getRoom(i).gridPosition ==
                         g.getRoom(j).gridPosition);
        }
    }
}

TEST(Block4_Generation, GeneratedPositionsArePositive) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    g.setSettings(s);
    g.GenerationRooms();

    for (int i = 0; i < 5; ++i) {
        EXPECT_GT(g.getRoom(i).position.x, 0);
        EXPECT_GT(g.getRoom(i).position.y, 0);
    }
}

TEST(Block4_Generation, ClearThenGenerate) {
    Graph g;
    Settings s;
    s.minRoomCount = 3;
    s.maxRoomCount = 3;
    g.setSettings(s);
    g.GenerationRooms();
    g.clear();
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 3);
}

TEST(Block4_Generation, RepeatedGenerationAddsRooms) {
    Graph g;
    Settings s;
    s.minRoomCount = 2;
    s.maxRoomCount = 2;
    g.setSettings(s);
    g.GenerationRooms();
    g.GenerationRooms();
    EXPECT_EQ(g.getRoomCount(), 4);
}


// =====================================================
// БЛОК 5. ГЕНЕРАЦИЯ СОЕДИНЕНИЙ — 10 ТЕСТОВ
// =====================================================

TEST(Block5_GenerateConnections, EmptyGraphDoesNotCrash) {
    Graph g;
    EXPECT_NO_THROW(g.generateConnections());
}

TEST(Block5_GenerateConnections, SingleRoomHasNoConnections) {
    Graph g;
    g.addRoom(0);
    g.generateConnections();
    EXPECT_TRUE(g.getRoom(0).connectedRooms.empty());
}

TEST(Block5_GenerateConnections, GeneratedDungeonHasConnections) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    int total = 0;
    for (int i = 0; i < 5; ++i)
        total += g.getRoom(i).connectedRooms.size();

    EXPECT_GT(total, 0);
}

TEST(Block5_GenerateConnections, GeneratedWeightsArePositive) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    s.minWeightPath = 1;
    s.maxWeightPath = 10;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 5; ++i)
        for (const auto& c : g.getRoom(i).connectedRooms)
            EXPECT_GE(c.weight, 1);
}

TEST(Block5_GenerateConnections, GeneratedWeightsDoNotExceedMaximum) {
    Graph g;
    Settings s;
    s.minRoomCount = 5;
    s.maxRoomCount = 5;
    s.minWeightPath = 2;
    s.maxWeightPath = 4;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 5; ++i)
        for (const auto& c : g.getRoom(i).connectedRooms)
            EXPECT_LE(c.weight, 4);
}

TEST(Block5_GenerateConnections, GeneratedConnectionsAreSymmetric) {
    Graph g;
    Settings s;
    s.minRoomCount = 8;
    s.maxRoomCount = 8;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 8; ++i) {
        for (const auto& c : g.getRoom(i).connectedRooms) {
            bool found = false;
            for (const auto& reverse : g.getRoom(c.roomId).connectedRooms) {
                if (reverse.roomId == i && reverse.weight == c.weight)
                    found = true;
            }
            EXPECT_TRUE(found);
        }
    }
}

TEST(Block5_GenerateConnections, ConnectionsUseValidRoomIds) {
    Graph g;
    Settings s;
    s.minRoomCount = 10;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 10; ++i)
        for (const auto& c : g.getRoom(i).connectedRooms) {
            EXPECT_GE(c.roomId, 0);
            EXPECT_LT(c.roomId, 10);
        }
}

TEST(Block5_GenerateConnections, GeneratedConnectionsJoinNeighbors) {
    Graph g;
    Settings s;
    s.minRoomCount = 8;
    s.maxRoomCount = 8;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 8; ++i) {
        for (const auto& c : g.getRoom(i).connectedRooms) {
            int dx = std::abs(g.getRoom(i).gridPosition.x -
                              g.getRoom(c.roomId).gridPosition.x);
            int dy = std::abs(g.getRoom(i).gridPosition.y -
                              g.getRoom(c.roomId).gridPosition.y);
            EXPECT_TRUE((dx == 1 && dy == 0) ||
                        (dx == 0 && dy == 1));
        }
    }
}

TEST(Block5_GenerateConnections, GeneratedDungeonIsReachable) {
    Graph g;
    Settings s;
    s.minRoomCount = 10;
    s.maxRoomCount = 10;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    EXPECT_FALSE(g.BFS(0, 9).empty());
}

TEST(Block5_GenerateConnections, RepeatedGenerationDoesNotAddDuplicateEdges) {
    Graph g;
    Settings s;
    s.minRoomCount = 8;
    s.maxRoomCount = 8;
    g.setSettings(s);
    g.GenerationRooms();
    g.generateConnections();

    for (int i = 0; i < 8; ++i) {
        for (const auto& c : g.getRoom(i).connectedRooms) {
            int occurrences = 0;
            for (const auto& other : g.getRoom(i).connectedRooms)
                if (other.roomId == c.roomId) ++occurrences;
            EXPECT_EQ(occurrences, 1);
        }
    }
}


// =====================================================
// БЛОК 6. BFS — 10 ТЕСТОВ
// =====================================================

TEST(Block6_BFS, FindsDirectPath) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 1);
    EXPECT_EQ(g.BFS(0, 1), (std::vector<int>{0, 1}));
}

TEST(Block6_BFS, FindsLongPath) {
    Graph g;
    for (int i = 0; i < 5; ++i) g.addRoom(i);
    for (int i = 0; i < 4; ++i) g.connectRooms(i, i + 1, 1);
    EXPECT_EQ(g.BFS(0, 4), (std::vector<int>{0, 1, 2, 3, 4}));
}

TEST(Block6_BFS, FindsShortestPath) {
    Graph g;
    for (int i = 0; i < 4; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 3, 1);
    g.connectRooms(0, 2, 1);
    g.connectRooms(2, 3, 1);
    EXPECT_EQ(g.BFS(0, 3).size(), 3);
}

TEST(Block6_BFS, UnreachableRoomReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    EXPECT_TRUE(g.BFS(0, 1).empty());
}

TEST(Block6_BFS, InvalidStartReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.BFS(-1, 0).empty());
}

TEST(Block6_BFS, InvalidEndReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.BFS(0, 1).empty());
}

TEST(Block6_BFS, SameStartAndEnd) {
    Graph g;
    g.addRoom(0);
    EXPECT_EQ(g.BFS(0, 0), (std::vector<int>{0}));
}

TEST(Block6_BFS, EmptyGraphReturnsEmpty) {
    Graph g;
    EXPECT_TRUE(g.BFS(0, 0).empty());
}

TEST(Block6_BFS, PathStartsAtRequestedRoom) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 1);
    auto path = g.BFS(2, 0);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 2);
}

TEST(Block6_BFS, VisitedCounterIsUpdated) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 1);
    g.BFS(0, 2);
    EXPECT_EQ(g.getLastVisitedCount(), 3);
}


// =====================================================
// БЛОК 7. DFS — 10 ТЕСТОВ
// =====================================================

TEST(Block7_DFS, FindsDirectPath) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 1);
    EXPECT_EQ(g.DFS(0, 1), (std::vector<int>{0, 1}));
}

TEST(Block7_DFS, FindsLongPath) {
    Graph g;
    for (int i = 0; i < 5; ++i) g.addRoom(i);
    for (int i = 0; i < 4; ++i) g.connectRooms(i, i + 1, 1);
    EXPECT_EQ(g.DFS(0, 4), (std::vector<int>{0, 1, 2, 3, 4}));
}

TEST(Block7_DFS, UnreachableRoomReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    EXPECT_TRUE(g.DFS(0, 1).empty());
}

TEST(Block7_DFS, InvalidStartReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.DFS(-1, 0).empty());
}

TEST(Block7_DFS, InvalidEndReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.DFS(0, 1).empty());
}

TEST(Block7_DFS, SameStartAndEnd) {
    Graph g;
    g.addRoom(0);
    EXPECT_EQ(g.DFS(0, 0), (std::vector<int>{0}));
}

TEST(Block7_DFS, EmptyGraphReturnsEmpty) {
    Graph g;
    EXPECT_TRUE(g.DFS(0, 0).empty());
}

TEST(Block7_DFS, PathStartsAtRequestedRoom) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 1);
    auto path = g.DFS(2, 0);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 2);
}

TEST(Block7_DFS, PathEndsAtRequestedRoom) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 1);
    auto path = g.DFS(0, 2);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.back(), 2);
}

TEST(Block7_DFS, VisitedCounterIsUpdated) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 1);
    g.connectRooms(1, 2, 1);
    g.DFS(0, 2);
    EXPECT_EQ(g.getLastVisitedCount(), 3);
}


// =====================================================
// БЛОК 8. DIJKSTRA — 10 ТЕСТОВ
// =====================================================

TEST(Block8_Dijkstra, FindsDirectPath) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 5);
    EXPECT_EQ(g.Dijkstra(0, 1), (std::vector<int>{0, 1}));
}

TEST(Block8_Dijkstra, FindsCheapestPath) {
    Graph g;
    for (int i = 0; i < 4; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 10);
    g.connectRooms(1, 3, 10);
    g.connectRooms(0, 2, 2);
    g.connectRooms(2, 3, 2);
    EXPECT_EQ(g.Dijkstra(0, 3), (std::vector<int>{0, 2, 3}));
}

TEST(Block8_Dijkstra, UnreachableRoomReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    EXPECT_TRUE(g.Dijkstra(0, 1).empty());
}

TEST(Block8_Dijkstra, InvalidStartReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.Dijkstra(-1, 0).empty());
}

TEST(Block8_Dijkstra, InvalidEndReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    EXPECT_TRUE(g.Dijkstra(0, 1).empty());
}

TEST(Block8_Dijkstra, SameStartAndEnd) {
    Graph g;
    g.addRoom(0);
    EXPECT_EQ(g.Dijkstra(0, 0), (std::vector<int>{0}));
}

TEST(Block8_Dijkstra, EmptyGraphReturnsEmpty) {
    Graph g;
    EXPECT_TRUE(g.Dijkstra(0, 0).empty());
}

TEST(Block8_Dijkstra, FindsPathWithDifferentWeights) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 3);
    g.connectRooms(1, 2, 4);
    EXPECT_EQ(g.Dijkstra(0, 2), (std::vector<int>{0, 1, 2}));
}

TEST(Block8_Dijkstra, PathStartsAtRequestedRoom) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 2);
    g.connectRooms(1, 2, 3);
    auto path = g.Dijkstra(2, 0);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 2);
}

TEST(Block8_Dijkstra, PathEndsAtRequestedRoom) {
    Graph g;
    for (int i = 0; i < 3; ++i) g.addRoom(i);
    g.connectRooms(0, 1, 2);
    g.connectRooms(1, 2, 3);
    auto path = g.Dijkstra(0, 2);
    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.back(), 2);
}


// =====================================================
// БЛОК 9. ОЧИСТКА, СЧЁТЧИКИ И ВЫВОД — 10 ТЕСТОВ
// =====================================================

TEST(Block9_State, ClearRemovesAllRooms) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.clear();
    EXPECT_EQ(g.getRoomCount(), 0);
}

TEST(Block9_State, ClearEmptiesPrintOutput) {
    Graph g;
    g.addRoom(0);
    g.clear();
    EXPECT_TRUE(g.PrintGraph().empty());
}

TEST(Block9_State, PrintGraphContainsRoomId) {
    Graph g;
    g.addRoom(5);
    EXPECT_NE(g.PrintGraph().find("Room 5"), std::string::npos);
}

TEST(Block9_State, PrintGraphContainsConnection) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 5);
    EXPECT_NE(g.PrintGraph().find("1(weight: 5)"), std::string::npos);
}

TEST(Block9_State, PrintGraphContainsAllRooms) {
    Graph g;
    for (int i = 0; i < 5; ++i) g.addRoom(i);
    std::string output = g.PrintGraph();

    for (int i = 0; i < 5; ++i)
        EXPECT_NE(output.find("Room " + std::to_string(i)),
                  std::string::npos);
}

TEST(Block9_State, ClearAllowsAddingRoomsAgain) {
    Graph g;
    g.addRoom(0);
    g.clear();
    g.addRoom(1);
    EXPECT_EQ(g.getRoomCount(), 1);
    EXPECT_EQ(g.getRoom(0).id, 1);
}

TEST(Block9_State, BfsCounterAfterInvalidInput) {
    Graph g;
    g.addRoom(0);
    g.BFS(-1, 0);
    EXPECT_EQ(g.getLastVisitedCount(), 0);
}

TEST(Block9_State, DijkstraDoesNotCrashOnSingleRoom) {
    Graph g;
    g.addRoom(0);
    EXPECT_EQ(g.Dijkstra(0, 0), (std::vector<int>{0}));
}

TEST(Block9_State, ClearAfterConnections) {
    Graph g;
    g.addRoom(0);
    g.addRoom(1);
    g.connectRooms(0, 1, 5);
    g.clear();
    EXPECT_EQ(g.getRoomCount(), 0);
}

TEST(Block9_State, ClearThenBfsReturnsEmpty) {
    Graph g;
    g.addRoom(0);
    g.clear();
    EXPECT_TRUE(g.BFS(0, 0).empty());
}