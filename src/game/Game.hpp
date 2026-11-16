#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Graph.hpp"

using namespace std;

enum class Density { Low = 1, Medium = 2, High = 3 };

class Game {
  private:
    bool settingsWindowOpen = false;

    sf::RectangleShape settingsBackground;
    sf::RectangleShape settingsPanel;
    sf::RectangleShape settingsApplyButton;
    sf::RectangleShape settingsCancelButton;
    sf::RectangleShape settingsResetButton;

    sf::Text settingsResetText;
    sf::Text settingsTitle;
    sf::Text settingsApplyText;
    sf::Text settingsCancelText;

    sf::Font font;

    sf::Texture blueRoadTexture;
    sf::Texture greenRoadTexture;
    sf::Texture brownRoadTexture;
    sf::Texture purpleRoadTexture;
    sf::Texture grayRoadTexture;

    bool roadTexturesLoaded = false;

    sf::Texture yellowPathTexture;
    sf::Texture redPathTexture;
    sf::Texture orangePathTexture;

    bool pathTexturesLoaded = false;

    sf::Texture roomTexture;
    sf::Texture topTexture;
    sf::Texture leftTexture;
    sf::Texture downTexture;
    sf::Texture rightTexture;

    bool roomTexturesLoaded = false;

    sf::Texture roomTexture2;
    sf::Texture topTexture2;
    sf::Texture leftTexture2;
    sf::Texture downTexture2;
    sf::Texture rightTexture2;

    bool roomTextures2Loaded = false;

    sf::Texture roomTexture3;
    sf::Texture topTexture3;
    sf::Texture leftTexture3;
    sf::Texture downTexture3;
    sf::Texture rightTexture3;

    bool roomTextures3Loaded = false;

    sf::Texture roomTexture4;
    sf::Texture topTexture4;
    sf::Texture leftTexture4;
    sf::Texture downTexture4;
    sf::Texture rightTexture4;

    bool roomTextures4Loaded = false;

    sf::Texture roomTexture5;
    sf::Texture topTexture5;
    sf::Texture leftTexture5;
    sf::Texture downTexture5;
    sf::Texture rightTexture5;

    bool roomTextures5Loaded = false;

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

    sf::RectangleShape minCountRoomInput;
    sf::RectangleShape maxCountRoomInput;

    sf::Text minCountRoomLabel;
    sf::Text maxCountRoomLabel;
    sf::Text minCountRoomValue;
    sf::Text maxCountRoomValue;

    string minCountRoomText = "8";
    string maxCountRoomText = "15";

    bool minCountRoomActive = false;
    bool maxCountRoomActive = false;

    sf::RectangleShape minWeightInput;
    sf::RectangleShape maxWeightInput;

    sf::Text minWeightLabel;
    sf::Text maxWeightLabel;
    sf::Text minWeightValue;
    sf::Text maxWeightValue;

    string minWeightText = "1";
    string maxWeightText = "10";

    bool minWeightActive = false;
    bool maxWeightActive = false;

    sf::RectangleShape densityInput;
    sf::Text densityLabel;
    sf::Text densityValue;

    Density density = Density::Medium;
    bool densityListOpen = false;
    sf::RectangleShape densityOptions[3];

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

  public:
    Game();

    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void setGraph(Graph& graph);
};