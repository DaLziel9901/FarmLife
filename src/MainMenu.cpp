#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(float width, float height)
{
    // background
    if (!backgroundTexture.loadFromFile(RESOURCES_PATH "Images/menu_background.png")) {
        std::cerr << "Không load được ảnh nền!" << std::endl;
    }
    else {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            width / backgroundTexture.getSize().x,
            height / backgroundTexture.getSize().y
        );
    }

    const std::vector<std::string> buttonNames = { "NewGame", "Continue", "Exit" };
    const float scale = 0.55f;
    const float spacing = 130.f;
    const float totalHeight = spacing * (buttonNames.size() - 1);
    const float startY = (height / 2.f) - (totalHeight / 2.f) - 40.f;

    buttons.clear();
    buttons.reserve(buttonNames.size());

    for (size_t i = 0; i < buttonNames.size(); ++i) {
        buttons.emplace_back();
        Button& btn = buttons.back();
        btn.name = buttonNames[i];

        std::string path = std::string(RESOURCES_PATH) + "Images/" + buttonNames[i] + ".png";
        if (!btn.texture.loadFromFile(path)) {
            std::cerr << "Không thể load ảnh nút: " << path << std::endl;
        }

        btn.sprite.setTexture(btn.texture);
        btn.sprite.setScale(scale, scale);

        float x = width / 2.f - btn.sprite.getGlobalBounds().width / 2.f;
        float y = startY + static_cast<float>(i) * spacing;
        btn.sprite.setPosition(x, y);

        btn.bounds = btn.sprite.getGlobalBounds();
    }
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.draw(backgroundSprite);
    for (auto& b : buttons)
        window.draw(b.sprite);
}

void MainMenu::handleMouseClick(sf::RenderWindow& window, bool& startGame, bool& exitGame)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto& btn : buttons) {
            if (!btn.bounds.contains(mouse)) continue;

            if (btn.name == "NewGame") {
                startGame = true;
            }
            else if (btn.name == "Continue") {
                // REMIND: VIẾT CHỨC NĂNG CONTINUE.
            }
            else if (btn.name == "Exit") {
                exitGame = true;
            }
        }
    }
}
