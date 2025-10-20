#include "GameUI.h"
#include "player.h"  
#include "FarmPlot.h"
#include "Crop.h"

namespace GameUI
{

    bool showInventory = true;
    bool showFarmDebug = false;

    void toggleInventory()
    {
        showInventory = !showInventory;
    }

    void toggleFarmDebug()
    {
        showFarmDebug = !showFarmDebug;
    }

    void init(sf::RenderWindow& window)
    {
        ImGui::SFML::Init(window);
        imguiThemes::red();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.FontGlobalScale = 1.5f;
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg].w = 0.9f;
    }

    void processEvent(const sf::Event& event)
    {
        ImGui::SFML::ProcessEvent(event);
    }

    void update(sf::RenderWindow& window, sf::Time deltaTime)
    {
        ImGui::SFML::Update(window, deltaTime);
        //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

    void render(Player& player, const std::vector<FarmPlot>& plots)
    {
        if (showInventory)
        {
            ImGui::SetNextWindowPos(ImVec2(550, 50), ImGuiCond_Once);
            ImGui::Begin("Shop & Inventory");

            ImGui::Text(" Money: %lld", player.getMoney());
            ImGui::Separator();


            // --- SEED SELECTION ---
            const std::string& selectedSeed = player.getSelectedSeed();
            std::string displaySeed = "NONE";
            const auto& inventory = player.getInventory();

            // Nếu còn hạt thì hiển thị tên, nếu hết thì reset về NONE
            if (!selectedSeed.empty())
            {
                auto it = inventory.find(selectedSeed);
                if (it != inventory.end() && it->second > 0)
                {
                    displaySeed = selectedSeed;
                }
                else
                {
                    // Nếu hạt đang chọn đã hết thì reset
                    if (!player.getSelectedSeed().empty())
                        player.setSelectedSeed("");
                }
            }

            ImGui::Text("Current Seed:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", displaySeed.c_str());

            if (ImGui::CollapsingHeader("Seed Selection"))
            {
                const auto& inventory = player.getInventory();
                const std::string& selectedSeed = player.getSelectedSeed();

                for (auto const& [name, data] : CropDatabase)
                {
                    int count = inventory.count(name) ? inventory.at(name) : 0;
                    bool isSelected = (name == selectedSeed);

                    // Đặt màu nền đặc biệt cho loại đang chọn
                    if (isSelected)
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));

                    // Nếu hết hạt => disable button
                    if (count <= 0)
                    {
                        ImGui::BeginDisabled();
                        ImGui::Button((name + " Seeds (" + std::to_string(count) + ")").c_str(), ImVec2(180, 0));
                        ImGui::SameLine();
                        ImGui::TextDisabled("Out of seeds!");
                        ImGui::EndDisabled();
                    }
                    else
                    {
                        std::string label = data.name + " Seeds (" + std::to_string(count) + ")";;
                        if (ImGui::Button(label.c_str(), ImVec2(180, 0)))
                        {
                            player.setSelectedSeed(name);
                        }
                    }

                    if (isSelected)
                        ImGui::PopStyleColor();
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
                    if (inventory.count(data.harvestedItem) && inventory.at(data.harvestedItem) > 0)
                    {
                        int count = inventory.at(data.harvestedItem);
                        ImGui::Text("%s: %d in stock | Sell: %d",
                            data.harvestedItem.c_str(), count, data.sellPrice);
                        ImGui::SameLine();
                        std::string buttonId = "Sell All##" + data.harvestedItem;
                        if (ImGui::Button(buttonId.c_str()))
                        {
                            player.sellCrop(data.harvestedItem, count);
                        }
                    }

                }
            }

            ImGui::End();
        }
        
        // **Farm Tilemap Debug Window**
        if (showFarmDebug)
        {
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
    }

    void renderImGui(sf::RenderWindow& window)
    {
        ImGui::SFML::Render(window);
    }

    void shutdown()
    {
        ImGui::SFML::Shutdown();
    }

    bool isMouseCaptured()
    {
        return ImGui::GetIO().WantCaptureMouse;
    }

    bool isKeyboardCaptured()
    {
        return ImGui::GetIO().WantCaptureKeyboard;
    }
}

void handlePlotInteraction(sf::RenderWindow& window, const sf::Event::MouseButtonEvent& mouse,
    Player& player, std::vector<FarmPlot>& plots, const sf::View& camera)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(
        sf::Vector2i(mouse.x, mouse.y), camera
    );

    /*std::cout << "Clicked at world position: ("
        << mousePos.x << ", " << mousePos.y << ")\n";*/

        /*for (auto& plot : plots)
        {
            if (plot.getGlobalBounds().contains(mousePos))
            {
                plot.setHighlight(true);
            }
            else
            {
                plot.setHighlight(false);
            }
        }*/

    for (auto& plot : plots)
    {
        if (plot.getGlobalBounds().contains(mousePos))
        {
            /*std::cout << "Hit plot at: ("
                << plot.getGlobalBounds().left << ", "
                << plot.getGlobalBounds().top << ")\n";*/

            const std::string& selectedSeed = player.getSelectedSeed();

            if (plot.getStage() == CropStage::Harvestable)
            {
                // Thu hoạch
                std::string crop = plot.harvest();
                if (!crop.empty())
                {
                    player.addHarvestedCrop(crop);
                    plot.resetToDry();
                }
            }
            else if (plot.isEmpty() && !plot.isWatered())
            {
                // Tưới đất
                plot.water();
            }
            else if (plot.isEmpty() && plot.isWatered() && !selectedSeed.empty())
            {
                // Trồng cây
                if (player.tryPlantSelectedSeed(selectedSeed))
                {
                    plot.plant(selectedSeed);
                }
            }

            break;
        }
    }
}
