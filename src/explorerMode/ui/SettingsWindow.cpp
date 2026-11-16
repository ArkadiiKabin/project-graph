#include "SettingsWindow.hpp"

using namespace std;

static void centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });

    sf::Vector2f buttonPosition = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    text.setPosition({
        buttonPosition.x + buttonSize.x / 2.f,
        buttonPosition.y + buttonSize.y / 2.f
    });
}

void SettingsWindow::updateTextLayout()
{
    auto titleBounds = title.getLocalBounds();

    title.setOrigin({
        titleBounds.position.x + titleBounds.size.x / 2.f,
        titleBounds.position.y
    });

    centerTextInButton(applyText, applyButton);
    centerTextInButton(cancelText, cancelButton);
    centerTextInButton(resetText, resetButton);
}

SettingsWindow::SettingsWindow(sf::Font& font)
    : font(font),
      title(font),
      applyText(font),
      cancelText(font),
      resetText(font),
      minRoomLabel(font),
      maxRoomLabel(font),
      minRoomValue(font),
      maxRoomValue(font),
      minWeightLabel(font),
      maxWeightLabel(font),
      minWeightValue(font),
      maxWeightValue(font),
      densityLabel(font),
      densityValue(font) {

    // Фон затемнения
    background.setSize({1100.f, 950.f});
    background.setPosition({0.f, 0.f});
    background.setFillColor(sf::Color(0, 0, 0, 180));

    // Основная панель настроек
    panel.setSize({500.f, 550.f});
    panel.setPosition({310.f, 190.f});
    panel.setFillColor(sf::Color(35, 35, 45));
    panel.setOutlineThickness(3.f);
    panel.setOutlineColor(sf::Color(120, 100, 75));

    // Заголовок настроек
    title.setString("GENERATION SETTINGS");
    title.setCharacterSize(28);

    title.setPosition({560.f, 220.f});
    title.setFillColor(sf::Color(230, 210, 170));

    // Кнопка APPLY
    applyButton.setSize({180.f, 45.f});
    applyButton.setPosition({370.f, 650.f});
    applyButton.setFillColor(sf::Color(65, 110, 75));

    applyText.setString("APPLY");
    applyText.setCharacterSize(20);
    applyText.setFillColor(sf::Color::White);

    // Кнопка CANCEL
    cancelButton.setSize({180.f, 45.f});
    cancelButton.setPosition({570.f, 650.f});
    cancelButton.setFillColor(sf::Color(120, 65, 65));

    cancelText.setString("CANCEL");
    cancelText.setCharacterSize(20);
    cancelText.setFillColor(sf::Color::White);

    // Кнопка RESET
    resetButton.setSize({140.f, 35.f});
    resetButton.setPosition({490.f, 700.f});
    resetButton.setFillColor(sf::Color(85, 85, 100));

    resetText.setString("RESET");
    resetText.setCharacterSize(17);
    resetText.setFillColor(sf::Color::White);

    // Количество комнат
    minRoomInput.setSize({65.f, 30.f});
    maxRoomInput.setSize({65.f, 30.f});

    minRoomInput.setFillColor(sf::Color(45, 45, 55));
    maxRoomInput.setFillColor(sf::Color(45, 45, 55));

    minRoomLabel.setString("Min:");
    maxRoomLabel.setString("Max:");

    minRoomLabel.setPosition({360.f, 300.f});
    maxRoomLabel.setPosition({560.f, 300.f});

    minRoomInput.setPosition({360.f, 325.f});
    maxRoomInput.setPosition({560.f, 325.f});

    minRoomValue.setPosition({380.f, 330.f});
    maxRoomValue.setPosition({580.f, 330.f});

    minRoomLabel.setCharacterSize(13);
    maxRoomLabel.setCharacterSize(13);
    minRoomValue.setCharacterSize(15);
    maxRoomValue.setCharacterSize(15);

    minRoomLabel.setFillColor(sf::Color::White);
    maxRoomLabel.setFillColor(sf::Color::White);
    minRoomValue.setFillColor(sf::Color::White);
    maxRoomValue.setFillColor(sf::Color::White);

    // Вес коридоров
    minWeightInput.setSize({65.f, 30.f});
    maxWeightInput.setSize({65.f, 30.f});

    minWeightInput.setFillColor(sf::Color(45, 45, 55));
    maxWeightInput.setFillColor(sf::Color(45, 45, 55));

    minWeightLabel.setString("Min:");
    maxWeightLabel.setString("Max:");

    minWeightLabel.setPosition({360.f, 400.f});
    maxWeightLabel.setPosition({560.f, 400.f});

    minWeightInput.setPosition({360.f, 425.f});
    maxWeightInput.setPosition({560.f, 425.f});

    minWeightValue.setPosition({380.f, 430.f});
    maxWeightValue.setPosition({580.f, 430.f});

    minWeightLabel.setCharacterSize(13);
    maxWeightLabel.setCharacterSize(13);
    minWeightValue.setCharacterSize(15);
    maxWeightValue.setCharacterSize(15);

    minWeightLabel.setFillColor(sf::Color::White);
    maxWeightLabel.setFillColor(sf::Color::White);
    minWeightValue.setFillColor(sf::Color::White);
    maxWeightValue.setFillColor(sf::Color::White);

    // Плотность соединений
    densityInput.setSize({155.f, 30.f});
    densityInput.setPosition({360.f, 525.f});
    densityInput.setFillColor(sf::Color(45, 45, 55));

    densityLabel.setString("Density:");
    densityLabel.setPosition({360.f, 500.f});
    densityLabel.setCharacterSize(13);
    densityLabel.setFillColor(sf::Color::White);

    densityValue.setCharacterSize(15);
    densityValue.setPosition({375.f, 530.f});
    densityValue.setFillColor(sf::Color::White);

    // Пункты выпадающего списка плотности
    for (int i = 0; i < 3; i++) {
        densityOptions[i].setSize({110.f, 28.f});
        densityOptions[i].setFillColor(sf::Color(55, 55, 65));
        densityOptions[i].setOutlineThickness(1.f);
        densityOptions[i].setOutlineColor(sf::Color(100, 100, 110));

        densityOptions[i].setPosition({
            densityInput.getPosition().x,
            densityInput.getPosition().y + 30.f + i * 28.f
        });
    }

    updateValues();
}

void SettingsWindow::open() {
    isWindowOpen = true;
}

void SettingsWindow::close() {
    isWindowOpen = false;
    densityListOpen = false;

    minRoomActive = false;
    maxRoomActive = false;
    minWeightActive = false;
    maxWeightActive = false;
}

bool SettingsWindow::isOpen() const {
    return isWindowOpen;
}

void SettingsWindow::reset() {
    draftSettings.minRoomCount = 8;
    draftSettings.maxRoomCount = 15;
    draftSettings.minWeightPath = 1;
    draftSettings.maxWeightPath = 10;

    densityDraft = 2;

    minRoomText = "8";
    maxRoomText = "15";
    minWeightText = "1";
    maxWeightText = "10";

    densityListOpen = false;

    updateValues();
}

void SettingsWindow::updateValues() {
    minRoomValue.setString(minRoomText);
    maxRoomValue.setString(maxRoomText);

    minWeightValue.setString(minWeightText);
    maxWeightValue.setString(maxWeightText);

    switch (densityDraft) {
        case 1:
            densityValue.setString("Low");
            break;

        case 2:
            densityValue.setString("Medium");
            break;

        case 3:
            densityValue.setString("High");
            break;
    }
}

bool SettingsWindow::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    if (!isWindowOpen) {
        return false;
    }

    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button != sf::Mouse::Button::Left) {
            return false;
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(
            mouse->position,
            window.getDefaultView()
        );

        minRoomActive = false;
        maxRoomActive = false;
        minWeightActive = false;
        maxWeightActive = false;

        // Закрытие без применения
        if (cancelButton.getGlobalBounds().contains(mousePos)) {
            close();
            return false;
        }

        // Сброс настроек
        if (resetButton.getGlobalBounds().contains(mousePos)) {
            reset();
            return false;
        }

        // Применение настроек
        if (applyButton.getGlobalBounds().contains(mousePos)) {
            if (minRoomText.empty() || maxRoomText.empty() ||
                minWeightText.empty() || maxWeightText.empty()) {
                return false;
            }

            int minRooms = stoi(minRoomText);
            int maxRooms = stoi(maxRoomText);
            int minWeight = stoi(minWeightText);
            int maxWeight = stoi(maxWeightText);

            if (minRooms < 1 || maxRooms > 99 ||
                minWeight < 0 || maxWeight > 999 ||
                minRooms > maxRooms || minWeight > maxWeight) {
                return false;
            }

            draftSettings.minRoomCount = minRooms;
            draftSettings.maxRoomCount = maxRooms;
            draftSettings.minWeightPath = minWeight;
            draftSettings.maxWeightPath = maxWeight;
            draftSettings.density = densityDraft;

            close();
            return true;
        }

        // Открытие списка плотности
        if (densityInput.getGlobalBounds().contains(mousePos)) {
            densityListOpen = !densityListOpen;
            return false;
        }

        // Выбор плотности
        if (densityListOpen) {
            for (int i = 0; i < 3; i++) {
                if (densityOptions[i].getGlobalBounds().contains(mousePos)) {
                    densityDraft = i + 1;
                    densityListOpen = false;
                    updateValues();
                    return false;
                }
            }

            densityListOpen = false;
        }

        // Выбор активного поля
        minRoomActive = minRoomInput.getGlobalBounds().contains(mousePos);
        maxRoomActive = maxRoomInput.getGlobalBounds().contains(mousePos);

        minWeightActive = minWeightInput.getGlobalBounds().contains(mousePos);
        maxWeightActive = maxWeightInput.getGlobalBounds().contains(mousePos);
    }

    // Ввод текста
    if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
        if (text->unicode >= '0' && text->unicode <= '9') {
            char digit = static_cast<char>(text->unicode);

            if (minRoomActive && minRoomText.size() < 2) {
                minRoomText += digit;
            }
            else if (maxRoomActive && maxRoomText.size() < 2) {
                maxRoomText += digit;
            }
            else if (minWeightActive && minWeightText.size() < 3) {
                minWeightText += digit;
            }
            else if (maxWeightActive && maxWeightText.size() < 3) {
                maxWeightText += digit;
            }

            updateValues();
        }

        // Удаление символов клавишей Backspace
        if (text->unicode == 8) {
            if (minRoomActive && !minRoomText.empty()) {
                minRoomText.pop_back();
            }
            else if (maxRoomActive && !maxRoomText.empty()) {
                maxRoomText.pop_back();
            }
            else if (minWeightActive && !minWeightText.empty()) {
                minWeightText.pop_back();
            }
            else if (maxWeightActive && !maxWeightText.empty()) {
                maxWeightText.pop_back();
            }

            updateValues();
        }
    }

    return false;
}

void SettingsWindow::draw(sf::RenderWindow& window) {
    if (!isWindowOpen) {
        return;
    }
    sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    // Подсветка активных полей
    minRoomInput.setOutlineThickness(minRoomActive ? 2.f : 0.f);
    minRoomInput.setOutlineColor(sf::Color::Cyan);

    maxRoomInput.setOutlineThickness(maxRoomActive ? 2.f : 0.f);
    maxRoomInput.setOutlineColor(sf::Color::Cyan);

    minWeightInput.setOutlineThickness(minWeightActive ? 2.f : 0.f);
    minWeightInput.setOutlineColor(sf::Color::Cyan);

    maxWeightInput.setOutlineThickness(maxWeightActive ? 2.f : 0.f);
    maxWeightInput.setOutlineColor(sf::Color::Cyan);

    densityInput.setOutlineThickness(densityListOpen ? 2.f : 0.f);
    densityInput.setOutlineColor(sf::Color::Cyan);

    // Фон и панель
    window.draw(background);
    window.draw(panel);
    window.draw(title);

    // Кнопки
    window.draw(applyButton);
    window.draw(applyText);

    window.draw(cancelButton);
    window.draw(cancelText);

    window.draw(resetButton);
    window.draw(resetText);

    // Количество комнат
    window.draw(minRoomInput);
    window.draw(maxRoomInput);

    window.draw(minRoomLabel);
    window.draw(maxRoomLabel);

    window.draw(minRoomValue);
    window.draw(maxRoomValue);

    // Вес коридоров
    window.draw(minWeightInput);
    window.draw(maxWeightInput);

    window.draw(minWeightLabel);
    window.draw(maxWeightLabel);

    window.draw(minWeightValue);
    window.draw(maxWeightValue);

    // Плотность соединений
    window.draw(densityInput);
    window.draw(densityLabel);
    window.draw(densityValue);

    // Выпадающий список плотности
    if (densityListOpen) {
        const string options[3] = {
            "Low",
            "Medium",
            "High"
        };

        for (int i = 0; i < 3; i++) {
            window.draw(densityOptions[i]);

            sf::Text optionText(font, options[i], 14);
            optionText.setFillColor(sf::Color::White);

            optionText.setPosition({
                densityOptions[i].getPosition().x + 10.f,
                densityOptions[i].getPosition().y + 5.f
            });

            window.draw(optionText);
        }
    }
    // Возвращаем предыдущую камеру
    window.setView(previousView);
}

const Settings& SettingsWindow::getSettings() const {
    return draftSettings;
}