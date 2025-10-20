#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

// Header files
#include "Crop.h"
#include "player.h"
#include "FarmPlot.h"
#include "GameUI.h"
#include "Map.h"
#include "Camera.h"
#include "AudioManager.h"
#include "MainMenu.h"

using namespace FarmGlobals;

int main()
{
    // ---------------------------------- TẠO CỬA SỔ ----------------------------------
    sf::RenderWindow window(sf::VideoMode(900, 650), "SFML Farming Game");
    window.setFramerateLimit(60);
    GameUI::init(window);

    // ---------------------------------- MENU CHÍNH ----------------------------------
    MainMenu menu(window.getSize().x, window.getSize().y);
    bool inMenu = true, startGame = false, exitGame = false;

    while (inMenu && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            // Cho phép MainMenu xử lý click
            menu.handleMouseClick(window, startGame, exitGame);
        }

        if (startGame)
            inMenu = false;
        if (exitGame) {
            window.close();
            return 0;
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    // ---------------------------------- GAME CHÍNH ----------------------------------
    Map gameMap;
    Player gamePlayer(200, "Carrot");
    Camera camera(900.0f, 600.0f);
    camera.setZoom(0.5f);
    std::vector<FarmPlot> gameFarmPlots;

    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_WaterLevel.csv",
        RESOURCES_PATH "Tilesets/Terrain.png",
        sf::Vector2u(32, 32),
        "WaterLevel");
    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_Terrain.csv",
        RESOURCES_PATH "Tilesets/Terrain.png",
        sf::Vector2u(32, 32),
        "Terrain");
    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_Bridge.csv",
        RESOURCES_PATH "Tilesets/Decor.png",
        sf::Vector2u(32, 32),
        "Bridge");
    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_FarmPlot.csv",
        RESOURCES_PATH "Tilesets/Decor.png",
        sf::Vector2u(32, 32),
        "FarmPlot");
    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_Building.csv",
        RESOURCES_PATH "Tilesets/Building.png",
        sf::Vector2u(32, 32),
        "Building");
    gameMap.loadFromCSV(
        RESOURCES_PATH "GameMap/GameMap_Decor.csv",
        RESOURCES_PATH "Tilesets/Decor.png",
        sf::Vector2u(32, 32),
        "Decor");

    loadFarmPlotsFromCSV(
        RESOURCES_PATH "GameMap/GameMap_FarmPlot.csv",
        gameFarmPlots,
        32.f
    );

    FarmPlot::s_cropTexture.loadFromFile(RESOURCES_PATH "Tilesets/crops.png");

    // ---------------------------------- VÒNG LẶP GAME ----------------------------------
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            GameUI::processEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();

            // ======== NHẤN ESC: QUAY LẠI MENU TẠM DỪNG ========
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                MainMenu pauseMenu(window.getSize().x, window.getSize().y);
                bool backToMenu = true;
                bool resumeGame = false;
                bool quitGame = false;

                while (backToMenu && window.isOpen()) {
                    sf::Event e;
                    while (window.pollEvent(e)) {
                        if (e.type == sf::Event::Closed)
                            window.close();
                        pauseMenu.handleMouseClick(window, resumeGame, quitGame);
                    }

                    if (resumeGame)
                        backToMenu = false; // quay lại game
                    if (quitGame) {
                        window.close();
                        return 0;
                    }

                    window.clear();
                    pauseMenu.draw(window);
                    window.display();
                }
            }

            // ======== CLICK CHUỘT: TRỒNG/TƯỚI/THU HOẠCH ========
            // Chỉ xử lý khi chuột thả và ImGui không chiếm chuột
            if (event.type == sf::Event::MouseButtonReleased && !GameUI::isMouseCaptured())
            {
                // Hàm tiện ích đã có trong code của bạn:
                // handlePlotInteraction(window, event.mouseButton, gamePlayer, gameFarmPlots, camera.getView());
                handlePlotInteraction(window, event.mouseButton, gamePlayer, gameFarmPlots, camera.getView());
            }
        }

        // Cập nhật delta time
        sf::Time deltaTime = clock.restart();
        float deltaTimeSeconds = std::clamp(deltaTime.asSeconds(), 0.f, 1.f);

        GameUI::update(window, deltaTime);
        if (!GameUI::isKeyboardCaptured())
            gamePlayer.handleInput();
        gamePlayer.update(deltaTimeSeconds);

        camera.follow(gamePlayer.getSprite().getPosition(),
            gameMap.getWidth() * 32.f,
            gameMap.getHeight() * 32.f,
            true);

        for (auto& plot : gameFarmPlots)
            plot.update(deltaTimeSeconds);

        // ======== HOVER HIGHLIGHT Ô RUỘNG (tuỳ chọn, đẹp UX) ========
        {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, camera.getView());
            for (auto& plot : gameFarmPlots) {
                bool isHover = plot.getGlobalBounds().contains(mouseWorld);
                plot.setHighlight(isHover);
            }
        }

        // ======== RENDER UI GAME (cửa sổ Inventory/Shop/Debug) ========
        GameUI::render(gamePlayer, gameFarmPlots);

        // -------------------------------- DRAW --------------------------------
        window.clear();
        camera.applyTo(window);
        window.draw(gameMap);
        for (const auto& plot : gameFarmPlots)
            window.draw(plot);
        window.draw(gamePlayer);

        window.setView(window.getDefaultView());
        GameUI::renderImGui(window);
        window.display();
    }

    GameUI::shutdown();
    return 0;
}
