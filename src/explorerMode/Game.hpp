#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Graph.hpp"
#include "rendering/DungeonRenderer.hpp"
#include "ui/SettingsWindow.hpp"

using namespace std;

class Game {
  private:
    sf::Font font;

    SettingsWindow settingsWindow;

    Graph* graph = nullptr;
    Settings generationSettings;

    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;

    sf::RectangleShape controlPanel;

    sf::RectangleShape bfsButton;
    sf::RectangleShape dfsButton;
    sf::RectangleShape dijkstraButton;

    sf::RectangleShape generateButton;
    sf::RectangleShape fullInfoButton;

    sf::RectangleShape startInput;
    sf::RectangleShape endInput;

    sf::Text startLabel;
    sf::Text endLabel;

    sf::Text startValue;
    sf::Text endValue;

    string startInputText = "0";
    string endInputText = "8";

    bool startInputActive = false;
    bool endInputActive = false;

    vector<int> bfsPath;
    vector<int> dfsPath;
    vector<int> dijPath;

    string bfsResult = "Not calculated";
    string dfsResult = "Not calculated";
    string dijkstraResult = "Not calculated";

    sf::RectangleShape settingsButton;
    sf::Text settingsButtonText;

    sf::RectangleShape infoPanel;
    sf::Text infoTitle;

    sf::RectangleShape infoToggleButton;
    bool showDungeonInfo = false;

    sf::Text dungeonInfoText;

    sf::RenderWindow infoWindow;
    bool showInfoWindow = false;

    DungeonRenderer dungeonRenderer;

  public:
    Game();

    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void setGraph(Graph& graph);
};