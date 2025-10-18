#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Crop.h" // Cần thiết cho CropStage

// GIÁ TRỊ TOÀN CỤC CHO CÁC LỚP SỬ DỤNG
namespace FarmGlobals
{
    // Kích thước khu đất và số lượng ô (Được chuyển từ main.cpp)
    const float PLOT_SIZE = 70.f;
    const int NUM_PLOTS_X = 5;
    const int NUM_PLOTS_Y = 5;
}

enum class CropStage;

class FarmPlot : public sf::Drawable
{
public:
    FarmPlot(float x, float y, float size);

    // Core game logic
    void update(float deltaTime);
    void plant(const std::string& cropName);
    std::string harvest();

    // Getters
    bool isEmpty() const;
    CropStage getStage() const;
    const std::string& getCropName() const;
    sf::FloatRect getGlobalBounds() const;

private:
    sf::RectangleShape m_shape;
    std::string m_cropName;
    CropStage m_stage;
    float m_timeInStage;

    // Override draw function from sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

void initializeFarmPlots(std::vector<FarmPlot>& plots, int startX, int startY, float plotSize);