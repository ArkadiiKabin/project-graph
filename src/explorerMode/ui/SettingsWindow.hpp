#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../Graph.hpp"

using namespace std;

class SettingsWindow {
  private:
    sf::Font& font;

    bool isWindowOpen = false;

    sf::RectangleShape background;
    sf::RectangleShape panel;

    sf::RectangleShape applyButton;
    sf::RectangleShape cancelButton;
    sf::RectangleShape resetButton;

    sf::Text title;
    sf::Text applyText;
    sf::Text cancelText;
    sf::Text resetText;

    sf::RectangleShape minRoomInput;
    sf::RectangleShape maxRoomInput;
    sf::Text minRoomLabel;
    sf::Text maxRoomLabel;
    sf::Text minRoomValue;
    sf::Text maxRoomValue;

    sf::RectangleShape minWeightInput;
    sf::RectangleShape maxWeightInput;
    sf::Text minWeightLabel;
    sf::Text maxWeightLabel;
    sf::Text minWeightValue;
    sf::Text maxWeightValue;

    sf::RectangleShape densityInput;
    sf::Text densityLabel;
    sf::Text densityValue;
    sf::RectangleShape densityOptions[3];

    Settings draftSettings;

    string minRoomText = "8";
    string maxRoomText = "15";
    string minWeightText = "1";
    string maxWeightText = "10";

    bool minRoomActive = false;
    bool maxRoomActive = false;
    bool minWeightActive = false;
    bool maxWeightActive = false;
    int densityDraft = 2;
    bool densityListOpen = false;

    void reset();
    void updateValues();

  public:
    void updateTextLayout();
    explicit SettingsWindow(sf::Font& font);

    void open();
    void close();
    bool isOpen() const;

    // возвращает true, если настройки успешно применены
    bool handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void draw(sf::RenderWindow& window);

    const Settings& getSettings() const;
};