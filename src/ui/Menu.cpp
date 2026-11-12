#include <stdexcept>

#include "Menu.hpp"

Menu::Menu()

    : title(font), startButtonText(font), exitButtonText(font) {
    if (!font.openFromFile("assets/fonts/HS_LunaObscura.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }

    // заголовок
    title.setString("Dungeon Explorer");
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::White);
    title.setPosition({170.f, 100.f});

    // кнопка старта
    startButton.setSize({250.f, 50.f});
    startButton.setPosition({275.f, 200.f});
    startButton.setFillColor(sf::Color(70, 70, 70));

    startButtonText.setString("START GAME");
    startButtonText.setCharacterSize(20);
    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setPosition({300.f, 210.f});

    // кнопка выхода
    exitButton.setSize({250.f, 50.f});
    exitButton.setPosition({275.f, 370.f});
    exitButton.setFillColor(sf::Color(70, 70, 70));

    exitButtonText.setString("EXIT GAME");
    exitButtonText.setCharacterSize(20);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setPosition({300.f, 380.f});
}

// Отрисовка главного меню
void Menu::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(startButton);
    window.draw(startButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

// Ловим наведение мышки на кнопку и меняем цвета
void Menu::update(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (startButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
        startButton.setFillColor(sf::Color::Red);
    else
        startButton.setFillColor(sf::Color(70, 70, 70));

    if (exitButton.getGlobalBounds().contains(sf::Vector2f(mousePos)))
        exitButton.setFillColor(sf::Color::Red);
    else
        exitButton.setFillColor(sf::Color(70, 70, 70));
}

// Нажатия кнопок меню
void Menu::handleEvent(sf::RenderWindow& window, const sf::Event& event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(mouse->position);
            if (startButton.getGlobalBounds().contains(mousePos)) {
                startGame = true;
            }

            if (exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }
    }
}