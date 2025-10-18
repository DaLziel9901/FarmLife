#include "GameUI.h"
#include "player.h"  // Cần thiết để truy cập các phương thức của Player
#include "FarmPlot.h" // Cần thiết để truy cập các phương thức của FarmPlot
#include "Crop.h" // Cần thiết cho CropStage và CropDatabase

// === HÀM HELPER getCropStageName (DI CHUYỂN TỪ main.cpp) ===
// Giả định rằng CropStage được định nghĩa trong Crop.h


// ===========================================================


namespace GameUI
{
    void init(sf::RenderWindow& window)
    {
        // Khởi tạo và thiết lập cấu hình ImGui từ #pragma region imgui cũ
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
        // === NỘI DUNG TỪ HÀM renderGameUI CŨ ===

        // *LƯU Ý*: Hàm này dựa vào biến toàn cục/extern 'CropDatabase'
        // được giả định là có thể truy cập được thông qua Crop.h

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
            for (auto const& [name, data] : CropDatabase)
            {
                if (inventory.count(name) && inventory.at(name) > 0)
                {
                    int count = inventory.at(name);
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