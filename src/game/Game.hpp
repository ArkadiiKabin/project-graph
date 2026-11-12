#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Graph.hpp"

class Game {
private:
    sf::Font font;

    // Текстуры дорог.
    sf::Texture blueRoadTexture;
    sf::Texture greenRoadTexture;
    sf::Texture brownRoadTexture;
    sf::Texture purpleRoadTexture;
    sf::Texture grayRoadTexture;

    bool roadTexturesLoaded = false;

    // Текстуры маршрутов.
    sf::Texture yellowPathTexture;
    sf::Texture redPathTexture;
    sf::Texture orangePathTexture;

    bool pathTexturesLoaded = false;

    // Первый набор текстур комнат.
    sf::Texture roomTexture;
    sf::Texture topTexture;
    sf::Texture leftTexture;
    sf::Texture downTexture;
    sf::Texture rightTexture;

    bool roomTexturesLoaded = false;

    // Второй набор текстур комнат.
    sf::Texture roomTexture2;
    sf::Texture topTexture2;
    sf::Texture leftTexture2;
    sf::Texture downTexture2;
    sf::Texture rightTexture2;

    bool roomTextures2Loaded = false;

    // Третий набор текстур комнат.
    sf::Texture roomTexture3;
    sf::Texture topTexture3;
    sf::Texture leftTexture3;
    sf::Texture downTexture3;
    sf::Texture rightTexture3;

    bool roomTextures3Loaded = false;

    // Четвёртый набор текстур комнат.
    sf::Texture roomTexture4;
    sf::Texture topTexture4;
    sf::Texture leftTexture4;
    sf::Texture downTexture4;
    sf::Texture rightTexture4;

    bool roomTextures4Loaded = false;

    // Пятый набор текстур комнат.
    sf::Texture roomTexture5;
    sf::Texture topTexture5;
    sf::Texture leftTexture5;
    sf::Texture downTexture5;
    sf::Texture rightTexture5;

    bool roomTextures5Loaded = false;

    // Граф и игровая область.
    Graph* graph = nullptr;

    sf::RectangleShape dungeonArea;
    sf::View dungeonCamera;

    // Правая панель управления.
    sf::RectangleShape controlPanel;

    sf::RectangleShape bfsButton;
    sf::RectangleShape dfsButton;
    sf::RectangleShape dijkstraButton;

    sf::RectangleShape generateButton;
    sf::RectangleShape fullInfoButton;

    // Поля ввода комнат.
    sf::RectangleShape startInput;
    sf::RectangleShape endInput;

    sf::Text startLabel;
    sf::Text endLabel;

    sf::Text startValue;
    sf::Text endValue;

    std::string startInputText = "0";
    std::string endInputText = "8";

    bool startInputActive = false;
    bool endInputActive = false;

    // Найденные маршруты.
    vector<int> bfsPath;
    vector<int> dfsPath;
    vector<int> dijPath;

    // Результаты алгоритмов.
    std::string bfsResult = "Not calculated";
    std::string dfsResult = "Not calculated";
    std::string dijkstraResult = "Not calculated";

    // Нижняя информационная панель.
    sf::RectangleShape infoPanel;

    sf::Text infoTitle;
    sf::Text infoContent;

    sf::RectangleShape infoToggleButton;
    bool showDungeonInfo = false;

    // Информация о подземелье.
    sf::RectangleShape dungeonInfoPanel;
    sf::Text dungeonInfoText;

    float dungeonInfoScroll = 0.f;

    // Окно полной информации.
    sf::RenderWindow infoWindow;
    bool showInfoWindow = false;

public:
    Game();

    void draw(sf::RenderWindow& window);
    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void setGraph(Graph& graph);
};