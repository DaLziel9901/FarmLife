#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

//header files
#include "Crop.h"
#include "player.h"
#include "FarmPlot.h"
#include "GameUI.h"
#include "Map.h"
#include "Camera.h"

using namespace FarmGlobals;

int main()
{
	// ---------------------------------- INIT WINDOW ----------------------------------
	sf::RenderWindow window(sf::VideoMode(900, 650), "SFML Farming Game");

	window.setFramerateLimit(60);

	GameUI::init(window);

	// ---------------------------------- INIT WINDOW ----------------------------------

	// ---------------------------------- INIT OBJ  ----------------------------------

	Map gameMap;
	Player gamePlayer(200, "Carrot");
	Camera camera(900.0f, 600.0f);
	camera.setZoom(0.5f);
	std::vector<FarmPlot> gameFarmPlots;

	// Load map từ file CSV
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


	initializeFarmPlots(gameFarmPlots, 50, 50, PLOT_SIZE);

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
					handlePlotInteraction(window, event.mouseButton, gamePlayer, gameFarmPlots);
			}
		}
		

		// Tính toán Delta Time
		sf::Time deltaTime = clock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);

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
		GameUI::render(gamePlayer, gameFarmPlots);
		// ---------------------------------- UPDATE ----------------------------------

		// ---------------------------------- DRAW ----------------------------------
		window.clear();

		camera.applyTo(window);
		window.draw(gameMap);
		/*for (const auto& plot : gameFarmPlots)
			window.draw(plot);	*/
		window.draw(gamePlayer);

		window.setView(window.getDefaultView());
		GameUI::renderImGui(window);

		window.display();
		// ---------------------------------- DRAW ----------------------------------
	}
	GameUI::shutdown();
	return 0;
}