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
	// ---------------------------------- INIT WINDOW ----------------------------------
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

	Map gameMap;
	Player gamePlayer(200, "Cà rốt");
	Camera camera(900.0f, 600.0f);
	camera.setZoom(0.5f);
	std::vector<FarmPlot> gameFarmPlots;

	// ---------------------------------- LOAD MAP & RESOURCES ----------------------------------
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

	//Load soundeffects
	auto& audio = AudioManager::getInstance();
	audio.loadSound("step", RESOURCES_PATH"Audio/Footstep.wav");
	audio.loadSound("water", RESOURCES_PATH"Audio/Spray.wav");
	audio.loadSound("harvest", RESOURCES_PATH"Audio/break.wav");

	audio.playMusic(RESOURCES_PATH "Audio/OMORI_OST005.wav");


	// ---------------------------------- INIT OBJ  ----------------------------------


	sf::Clock clock;
	while (window.isOpen())
	{
		// ---------------------------------- UPDATE ----------------------------------
		sf::Event event;
		while (window.pollEvent(event))
		{
			GameUI::processEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (!GameUI::isMouseCaptured())
					handlePlotInteraction(window, event.mouseButton, gamePlayer, gameFarmPlots, camera.getView());
			}
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::E)
                {
                    GameUI::toggleInventory();
                }
                if (event.key.code == sf::Keyboard::F3)
                {
                    GameUI::toggleFarmDebug();
                }
            }
            // ------------------------------ MENU ---------------------------------------
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                audio.pauseMusic();

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

                    if (resumeGame) {
                        backToMenu = false;      // quay lại game
                        audio.resumeMusic();     // tiếp tục nhạc
                    }
                    if (quitGame) {
                        audio.stopMusic();       // dừng hẳn nhạc khi thoát game
                        window.close();
                        return 0;
                    }

                    window.clear();
                    pauseMenu.draw(window);
                    window.display();
                }
            }

		}

        // delta time
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

		sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel, camera.getView());

		for (auto& plot : gameFarmPlots)
		{
			bool isHover = plot.getGlobalBounds().contains(mouseWorld);
			plot.setHighlight(isHover);
		}
		//static sf::Clock debugClock;
		//if (debugClock.getElapsedTime().asSeconds() > 0.5f) // in mỗi 0.5 giây
		//{
		//	sf::Vector2f playerPos = gamePlayer.getSprite().getPosition();
		//	sf::Vector2f camCenter = camera.getCenter();
		//	sf::Vector2f camSize = camera.getSize();

		//	std::cout << "Player at (" << playerPos.x << ", " << playerPos.y << ")";
		//	std::cout << "Camera center: (" << camCenter.x << ", " << camCenter.y << ")";
		//	std::cout << "View size : (" << camSize.x << ", " << camSize.y << ")\n";

		//	debugClock.restart();
		//}

		GameUI::render(gamePlayer, gameFarmPlots);
		// ---------------------------------- UPDATE ----------------------------------
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
			std::cout << "Watering all farm plots.\n" << std::endl;
			for (auto& plot : gameFarmPlots)
			{
				plot.water();

				auto pos = plot.getGlobalBounds();
				std::cout << "Watered plot at: ("
					<< pos.left << ", " << pos.top << ")\n";
			}
		}		
		// ---------------------------------- DRAW ------------------------------------
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
