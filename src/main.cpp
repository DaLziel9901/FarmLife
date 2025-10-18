#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

// ... (Imgui includes)
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

//header files
#include "Crop.h"
#include "player.h"
#include "FarmPlot.h"

// Kích thước khu đất và số lượng ô (GIỮ NGUYÊN GLOBALS/CONSTANTS)
const float PLOT_SIZE = 70.f;
const int NUM_PLOTS_X = 5;
const int NUM_PLOTS_Y = 5;

std::string getCropStageName(CropStage stage); 

// --- TRIỂN KHAI CÁC HÀM ---

// *********** Hàm initializeFarmPlots ***********
void initializeFarmPlots(std::vector<FarmPlot>& plots, int startX, int startY, float plotSize)
{
	for (int y = 0; y < NUM_PLOTS_Y; ++y)
	{
		for (int x = 0; x < NUM_PLOTS_X; ++x)
		{
			plots.emplace_back(startX + x * plotSize, startY + y * plotSize, plotSize);
		}
	}
}

// *********** Hàm handlePlotInteraction ***********
void handlePlotInteraction(sf::RenderWindow& window, const sf::Event::MouseButtonEvent& mouse, Player& player, std::vector<FarmPlot>& plots)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(mouse.x, mouse.y));

	for (auto& plot : plots)
	{
		if (plot.getGlobalBounds().contains(mousePos))
		{
			if (mouse.button == sf::Mouse::Left)
			{
				const std::string& selectedSeed = player.getSelectedSeed();

				if (plot.getStage() == CropStage::Harvestable)
				{
					// Thu hoạch
					std::string cropName = plot.harvest();
					if (cropName != "")
					{
						player.addHarvestedCrop(cropName);
					}
				}
				else if (plot.isEmpty() && !selectedSeed.empty())
				{
					// Trồng cây
					if (player.tryPlantSelectedSeed(selectedSeed))
					{
						plot.plant(selectedSeed);
					}
				}
			}
			break;
		}
	}
}

// *********** Hàm renderGameUI ***********
void renderGameUI(Player& player, const std::vector<FarmPlot>& plots)
{
	// Nội dung hàm renderGameUI: Giữ nguyên logic sử dụng player.getMoney(), player.getSelectedSeed(), ...
	// ... (Code renderGameUI đã cung cấp trước đó)
	ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_Once);
	ImGui::Begin("Shop & Inventory");

	ImGui::Text("💰 Money: %lld", player.getMoney());
	ImGui::Separator();

	// --- SEED SELECTION ---
	const std::string& selectedSeed = player.getSelectedSeed();
	ImGui::Text("Current Seed:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", selectedSeed.empty() ? "NONE" : selectedSeed.c_str());

	if (ImGui::CollapsingHeader("Seed Selection"))
	{
		const auto& inventory = player.getInventory();
		for (auto const& [name, count] : inventory)
		{
			if (count > 0 && CropDatabase.count(name))
			{
				std::string display = name + " Seeds (" + std::to_string(count) + ")";
				if (ImGui::Button(display.c_str()))
				{
					player.setSelectedSeed(name);
				}
			}
		}
	}

	ImGui::Separator();

	// --- BUY SEEDS (Shop) ---
	if (ImGui::CollapsingHeader("Buy Seeds"))
	{
		for (auto const& [name, data] : CropDatabase)
		{
			ImGui::Text("%s - Price: %d", data.name.c_str(), data.seedPrice);
			ImGui::SameLine();
			std::string buttonId = "Buy 1x##" + name;
			if (ImGui::Button(buttonId.c_str()))
			{
				player.buySeed(name, 1);
			}
		}
	}

	ImGui::Separator();

	// --- SELL CROPS ---
	if (ImGui::CollapsingHeader("Sell Crops"))
	{
		const auto& inventory = player.getInventory();
		for (auto const& [name, data] : CropDatabase)
		{
			if (inventory.count(name) && inventory.at(name) > 0)
			{
				if (CropDatabase.count(name))
				{
					int count = inventory.at(name);
					ImGui::Text("%s: %d in stock | Sell: %d", data.name.c_str(), count, data.sellPrice);
					ImGui::SameLine();
					std::string buttonId = "Sell All##" + name;
					if (ImGui::Button(buttonId.c_str()))
					{
						player.sellCrop(name, count);
					}
				}
			}
		}
	}

	ImGui::End();

	// **Farm Tilemap Debug Window**
	ImGui::SetNextWindowPos(ImVec2(550, 400), ImGuiCond_Once);
	ImGui::Begin("Farm Tilemap Debug");
	ImGui::Text("Total Plots: %d", (int)plots.size());

	if (ImGui::BeginTable("Plots", 3))
	{
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Crop");
		ImGui::TableSetupColumn("Status");
		ImGui::TableHeadersRow();

		for (int i = 0; i < plots.size(); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", i);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%s", plots[i].getCropName() == "" ? "Empty" : plots[i].getCropName().c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%s", getCropStageName(plots[i].getStage()).c_str());
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

// *********** Hàm main ***********
int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 650), "SFML Farming Game");
	window.setFramerateLimit(60);

#pragma region imgui
	ImGui::SFML::Init(window);
	imguiThemes::red();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 1.5f;
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg].w = 0.9f;
#pragma endregion

	// KHỞI TẠO CÁC ĐỐI TƯỢNG GAME CHÍNH TRONG HÀM MAIN
	Player gamePlayer(200, "Carrot"); // <--- Khởi tạo Player
	std::vector<FarmPlot> gameFarmPlots; // <--- Khai báo vector

	// Khởi tạo khu đất nông trại
	initializeFarmPlots(gameFarmPlots, 50, 50, PLOT_SIZE);

	

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

#pragma region imgui
			ImGui::SFML::ProcessEvent(window, event);
#pragma endregion

			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				// Truyền Player và FarmPlots vào hàm
				if (!ImGui::GetIO().WantCaptureMouse)
					handlePlotInteraction(window, event.mouseButton, gamePlayer, gameFarmPlots);
			}
		}

		// Tính toán Delta Time
		sf::Time deltaTime = clock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);

#pragma region imgui
		ImGui::SFML::Update(window, deltaTime);
		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
#pragma endregion

		// Xử lý input người chơi
		if (!ImGui::GetIO().WantCaptureKeyboard)
			gamePlayer.handleInput();

		gamePlayer.update(deltaTimeSeconds);

		// 1. Cập nhật logic game
		for (auto& plot : gameFarmPlots)
		{
			plot.update(deltaTimeSeconds);
		}

		// 2. Render UI
		// Truyền Player và FarmPlots vào hàm
		renderGameUI(gamePlayer, gameFarmPlots);
		

		// 3. Render SFML
		window.clear(sf::Color(100, 200, 100));

		// Vẽ các ô đất
		for (const auto& plot : gameFarmPlots)
		{
			window.draw(plot);
		}

		// Vẽ người chơi
		window.draw(gamePlayer);

#pragma region imgui
		ImGui::SFML::Render(window);
#pragma endregion

		window.display();
	}

#pragma region imgui
	ImGui::SFML::Shutdown();
#pragma endregion

	return 0;
}