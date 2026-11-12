#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Graph.hpp"

class Game {
   private:
    sf::Font font;

    // Текстуры дорог
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
    // Текстуры комнаты
    sf::Texture roomTexture;
    sf::Texture topTexture;
    sf::Texture leftTexture;
    sf::Texture downTexture;
    sf::Texture rightTexture;
    // Текстуры второго набора
    sf::Texture roomTexture2;
    sf::Texture topTexture2;
    sf::Texture leftTexture2;
    sf::Texture downTexture2;
    sf::Texture rightTexture2;

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

    bool roomTextures2Loaded = false;
    // Проверка загрузки текстур
    bool roomTexturesLoaded = false;
    bool roomTextureLoaded = false;
    Graph* graph = nullptr;
    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;
    sf::RectangleShape controlPanel;
    sf::RectangleShape bfsButton;
    sf::RectangleShape dfsButton;
    sf::RectangleShape dijkstraButton;
    sf::RenderWindow infoWindow;
    bool showInfoWindow = false;
    sf::RectangleShape infoPanel;
    float dungeonInfoScroll = 0.f;
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
    sf::RectangleShape fullInfoButton;

   public:
    Game();
    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
    void setGraph(Graph& graph);
};