#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Crop.h" 

namespace FarmGlobals
{
    const float PLOT_SIZE = 70.f;
    const int NUM_PLOTS_X = 5;
    const int NUM_PLOTS_Y = 5;
}

enum class CropStage;
enum class SoilState { Dry, Wet };

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

    // Tương tác với plot
    void setHighlight(bool value);
    void water(); 
    bool isWatered() const;
    void resetToDry();
    void updateSoilTexture();
    void updateCropTexture();

    static sf::Texture s_cropTexture;

private:
    std::string m_cropName;
    CropStage m_stage;
    float m_timeInStage;
    sf::Sprite m_sprite;
    sf::Sprite m_cropSprite;
    static sf::Texture s_texture;

    bool m_highlighted;
    SoilState m_soilState;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

void loadFarmPlotsFromCSV(const std::string& csvFile,
    std::vector<FarmPlot>& plots,
    float tileSize);
