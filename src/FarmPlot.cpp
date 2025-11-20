#include "FarmPlot.h"
#include "Crop.h" 
#include "AudioManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>



using namespace FarmGlobals;

void loadFarmPlotsFromCSV(const std::string& csvFile,
    std::vector<FarmPlot>& plots,
    float tileSize)
{
    plots.clear();

    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Cannot open farmplot CSV: " << csvFile << std::endl;
        return;
    }

    std::vector<int> ids;
    std::string line;
    unsigned int width = 0, height = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        unsigned int count = 0;
        while (std::getline(ss, val, ',')) {
            if (!val.empty()) {
                ids.push_back(std::stoi(val));
                ++count;
            }
        }
        if (width == 0) width = count;
        ++height;
    }

    file.close();

    // Tạo FarmPlot thoe id từ file CSV
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int id = ids[x + y * width];
            if (id >= 0) {
                float worldX = x * tileSize;
                float worldY = y * tileSize;
                plots.emplace_back(worldX, worldY, tileSize);
            }
        }
    }

    std::cout << "Loaded " << plots.size() << " farm plots from " << csvFile << std::endl;
}

sf::Texture FarmPlot::s_texture;
sf::Texture FarmPlot::s_cropTexture;

FarmPlot::FarmPlot(float x, float y, float size)
    : m_stage(CropStage::Empty)
    , m_timeInStage(0.0f)
    , m_highlighted(false)
    , m_soilState(SoilState::Dry)
{
    if (s_texture.getSize().x == 0)
        s_texture.loadFromFile(RESOURCES_PATH "Tilesets/Decor.png");

    m_sprite.setTexture(s_texture);

    const int tilesPerRow = 16; 
    int id = 32; // id đất khô
    int tu = id % tilesPerRow;
    int tv = id / tilesPerRow;

    m_sprite.setTextureRect(sf::IntRect(tu * 32, tv * 32, 32, 32));
    m_sprite.setPosition(x, y);
}

// Update
void FarmPlot::update(float deltaTime)
{
    if (m_stage == CropStage::Empty || m_stage == CropStage::Dead || m_cropName.empty())
    {
        return;
    }

    if (m_stage == CropStage::Harvestable)
    {
        return;
    }

    // Lấy dữ liệu cây trồng
    if (CropDatabase.count(m_cropName))
    {
        const CropData& data = CropDatabase.at(m_cropName);
        m_timeInStage += deltaTime;

        // Seed -> Stage 1
        if (m_stage == CropStage::Seed && m_timeInStage >= data.seedDuration)
        {
            m_stage = CropStage::GrowingStage1;
            m_timeInStage = 0.0f;
			updateCropTexture();
        }
		// Stage 1 -> Stage 2
        else if (m_stage == CropStage::GrowingStage1 && m_timeInStage >= data.growthDuration)
        {
            m_stage = CropStage::GrowingStage2;
            m_timeInStage = 0.0f;
			updateCropTexture();
        }
		// Stage 2 -> Harvestable
        else if (m_stage == CropStage::GrowingStage2 && m_timeInStage >= data.growthDuration)
        {
            m_stage = CropStage::Harvestable;
            m_timeInStage = 0.0f;
			updateCropTexture();
        }
    }
    else
    {
		// Báo lỗi nếu không tìm thấy dữ liệu cây trồng
        std::cerr << "Lỗi: Không tìm thấy CropData cho " << m_cropName << std::endl;
        m_stage = CropStage::Empty;
        m_cropName = "";
    }
}

void FarmPlot::plant(const std::string& cropName)
{
    m_cropName = cropName;
    m_stage = CropStage::Seed;
    m_timeInStage = 0.0f;

	updateCropTexture();
}

void FarmPlot::setHighlight(bool value)
{
    m_highlighted = value;
}

void FarmPlot::updateSoilTexture()
{
    sf::IntRect rect;

    if (m_soilState == SoilState::Dry)
        rect = sf::IntRect(0, 0, 32, 32);     // Ô đất khô
    else if (m_soilState == SoilState::Wet)
    {
        rect = sf::IntRect(32, 0, 32, 32);    // Ô đất có nước 
      
    }

    m_sprite.setTextureRect(rect);
}

void FarmPlot::updateCropTexture()
{
    if (m_cropName.empty()|| !CropDatabase.count(m_cropName))
        return;

    const CropData& data = CropDatabase.at(m_cropName);
	int stageOffset = 0;

    switch (m_stage)
    {
    case CropStage::Seed:          stageOffset = 0; break;
    case CropStage::GrowingStage1: stageOffset = 1; break;
    case CropStage::GrowingStage2: stageOffset = 2; break;
    case CropStage::Harvestable:   stageOffset = 3; break;
    default: return;
    }

    const int tile_size = 32;
    const int tilesPerRow = 16;

	int baseID = data.spriteBaseID + stageOffset;
    int topID = baseID - ( tilesPerRow * (data.spriteHeight - 1));

	int tu = baseID % tilesPerRow;
	int tv = baseID / tilesPerRow;

	m_cropSprite.setTexture(FarmPlot::s_cropTexture);
	m_cropSprite.setTextureRect(sf::IntRect(
        tu * tile_size,
        (tv - (data.spriteHeight -1)) * tile_size,
        tile_size,
		tile_size * data.spriteHeight
    ));

	sf::Vector2f pos = m_sprite.getPosition();
	pos.y -= (data.spriteHeight - 1) * tile_size; // Dịch chuyển lên trên
	m_cropSprite.setPosition(pos);
}

void FarmPlot::water()
{
    m_soilState = SoilState::Wet;

    const int tilesPerRow = 16; 
    int id = 48;                // tile ID cho đất có nước
    int tu = id % tilesPerRow;
    int tv = id / tilesPerRow;

    m_sprite.setTextureRect(sf::IntRect(tu * 32, tv * 32, 32, 32));
}

bool FarmPlot::isWatered() const
{
    return m_soilState == SoilState::Wet;
}

void FarmPlot::resetToDry()
{
    m_soilState = SoilState::Dry;

    const int tilesPerRow = 16;
    int id = 32;                // tile ID cho đất khô
    int tu = id % tilesPerRow;
    int tv = id / tilesPerRow;

    m_sprite.setTextureRect(sf::IntRect(tu * 32, tv * 32, 32, 32));
    updateCropTexture();
}

std::string FarmPlot::harvest()
{
    if (m_stage == CropStage::Harvestable)
    {
        std::string harvestedCrop = CropDatabase.at(m_cropName).harvestedItem;
        m_cropName = "";
        m_stage = CropStage::Empty;
        m_timeInStage = 0.0f;
        return harvestedCrop;
    }
    return ""; // Trả về chuỗi rỗng nếu không thu hoạch được
}

bool FarmPlot::isEmpty() const
{
    return m_stage == CropStage::Empty;
}

CropStage FarmPlot::getStage() const
{
    return m_stage;
}

const std::string& FarmPlot::getCropName() const
{
    return m_cropName;
}

sf::FloatRect FarmPlot::getGlobalBounds() const
{
    return m_sprite.getGlobalBounds();
}

void FarmPlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
    if (m_stage != CropStage::Empty && !m_cropName.empty())
    {
        target.draw(m_cropSprite, states);
	}

    if (m_highlighted) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
        overlay.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top);
        overlay.setFillColor(sf::Color(255, 255, 0, 60)); // vàng trong suốt
        target.draw(overlay, states);
    }
}