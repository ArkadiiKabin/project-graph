#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Graph.h"

class Game{
    private:
    sf::Font font;
    Graph* graph = nullptr;
    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;
    sf::RectangleShape controlPanel;
    sf::RectangleShape bfsButton;
    sf::RectangleShape dfsButton;
    sf::RectangleShape dijkstraButton;
    sf::RectangleShape infoPanel;
    sf::Text infoTitle;
    sf::Text infoContent;
    sf::RectangleShape infoToggleButton;
    bool showDungeonInfo = false;
    sf::RectangleShape dungeonInfoPanel;
    sf::Text dungeonInfoText;
    sf::RectangleShape startInput;
    sf::RectangleShape endInput;
    sf::Text startLabel;
    sf::Text endLabel;
    sf::Text startValue;
    sf::Text endValue;
    std::string startInputText = "0";
    std::string endInputText = "8";
    sf::RectangleShape generateButton;
    bool startInputActive = false;
    bool endInputActive = false;
    vector<int> bfsPath;
    vector<int> dfsPath;
    vector<int> dijPath;
    std::string bfsResult = "Not calculated";
    std::string dfsResult = "Not calculated";
    std::string dijkstraResult = "Not calculated";
    public:
    Game();
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
    void setGraph(Graph& graph);
};