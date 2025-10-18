#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>

// ImGui includes
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

// Forward declarations cho các lớp game
class Player;
class FarmPlot;

// Khai báo hàm helper getCropStageName (Cần được định nghĩa trong GameUI.cpp hoặc file khác)
// Lưu ý: Nếu hàm này được sử dụng trong các file khác, bạn nên chuyển khai báo này sang Crop.h
enum class CropStage; // Phụ thuộc vào định nghĩa trong Crop.h
std::string getCropStageName(CropStage stage);

// Chức năng quản lý UI/ImGui của Game
namespace GameUI
{
    // Khởi tạo ImGui (chạy 1 lần lúc bắt đầu game)
    void init(sf::RenderWindow& window);

    // Xử lý sự kiện ImGui (gọi trong vòng lặp sự kiện)
    void processEvent(const sf::Event& event);

    // Cập nhật ImGui (gọi trước khi render)
    void update(sf::RenderWindow& window, sf::Time deltaTime);

    // Render toàn bộ UI của game (Shop, Inventory, Debug Window)
    void render(Player& player, const std::vector<FarmPlot>& plots);

    // Render các đối tượng ImGui lên cửa sổ SFML
    void renderImGui(sf::RenderWindow& window);

    // Dọn dẹp ImGui (chạy 1 lần lúc kết thúc game)
    void shutdown();

    // Kiểm tra xem chuột có đang tương tác với ImGui không
    bool isMouseCaptured();

    // Kiểm tra xem bàn phím có đang tương tác với ImGui không
    bool isKeyboardCaptured();
}

void handlePlotInteraction(sf::RenderWindow& window, const sf::Event::MouseButtonEvent& mouse, Player& player, std::vector<FarmPlot>& plots);
