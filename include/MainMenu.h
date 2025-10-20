#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class MainMenu
{
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    struct Button {
        std::string name;
        sf::Texture texture;
        sf::Sprite sprite;
        sf::FloatRect bounds;
    };

    std::vector<Button> buttons;

public:
    MainMenu(float width, float height);
    void draw(sf::RenderWindow& window);
    void handleMouseClick(sf::RenderWindow& window, bool& startGame, bool& exitGame);
};
