#include "Game.h"
Game::Game(){
    dungeonArea.setSize({550.f, 500.f});
    dungeonArea.setPosition({20.f, 20.f});
    dungeonArea.setFillColor(sf::Color(35, 35, 40));

    dungeonCamera.setSize({550.f, 500.f});
    dungeonCamera.setCenter({295.f, 270.f});
    dungeonCamera.setViewport(
    sf::FloatRect(
        {0.025f, 0.0333f},
        {0.6875f, 0.8333f}
    )
);
}

void Game::draw(sf::RenderWindow& window){
    window.setView(dungeonCamera);
    window.draw(dungeonArea);
}

void Game::handleEvent(sf::RenderWindow& window, const sf::Event& event){
    if(const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()){
        sf::Vector2i mousePos = mouse->position;
        sf::Vector2f beforePos = window.mapPixelToCoords(mousePos, dungeonCamera);
        if(mouse->delta > 0) dungeonCamera.zoom(0.9f);
        else dungeonCamera.zoom(1.1f);
        sf::Vector2f afterPos = window.mapPixelToCoords(mousePos, dungeonCamera);
        dungeonCamera.move(beforePos - afterPos);
    }
}