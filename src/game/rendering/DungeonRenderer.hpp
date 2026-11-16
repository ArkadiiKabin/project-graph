#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../Graph.hpp"

using namespace std;

class DungeonRenderer {
  private:
    sf::Font& font;

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

  public:
    explicit DungeonRenderer(sf::Font& font);

    void draw(sf::RenderWindow& window, const Graph& graph, const vector<int>& bfsPath,
              const vector<int>& dfsPath, const vector<int>& dijPath) const;

    void fitToView(const Graph& graph, sf::View& camera) const;
};