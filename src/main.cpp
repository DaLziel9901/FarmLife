#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

//header files
#include "Crop.h"
#include "player.h"
#include "FarmPlot.h"
#include "GameUI.h"

using namespace FarmGlobals;

int main()
{
	// ---------------------------------- INIT WINDOW ----------------------------------
	sf::RenderWindow window(sf::VideoMode(900, 650), "SFML Farming Game");
	window.setFramerateLimit(60);

	GameUI::init(window); // Khởi tạo ImGui

	// ---------------------------------- INIT WINDOW ----------------------------------

	// ---------------------------------- INIT OBJ  ----------------------------------
	Player gamePlayer(200, "Carrot"); 
	std::vector<FarmPlot> gameFarmPlots;
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
		for (auto& plot : gameFarmPlots)
		{	plot.update(deltaTimeSeconds);	}
		GameUI::render(gamePlayer, gameFarmPlots);
		// ---------------------------------- UPDATE ----------------------------------

		// ---------------------------------- DRAW ----------------------------------
		window.clear(sf::Color(100, 200, 100));
		for (const auto& plot : gameFarmPlots)
		{	window.draw(plot);	}
		window.draw(gamePlayer);

		GameUI::renderImGui(window);

		window.display();
		// ---------------------------------- DRAW ----------------------------------
	}
	GameUI::shutdown();
	return 0;
}