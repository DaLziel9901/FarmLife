#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

class Player;
class FarmPlot;

enum class CropStage;
std::string getCropStageName(CropStage stage);

namespace GameUI
{
    void init(sf::RenderWindow& window);
    void processEvent(const sf::Event& event);
    void update(sf::RenderWindow& window, sf::Time deltaTime);
    void render(Player& player, const std::vector<FarmPlot>& plots);
    void renderImGui(sf::RenderWindow& window);
    void shutdown();

    bool isMouseCaptured();
    bool isKeyboardCaptured();

    void toggleInventory();
    void toggleFarmDebug();
}

void handlePlotInteraction(sf::RenderWindow& window, 
    const sf::Event::MouseButtonEvent& mouse, 
    Player& player, 
    std::vector<FarmPlot>& plots, 
    const sf::View& camera);

