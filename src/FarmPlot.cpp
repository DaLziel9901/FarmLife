#include "FarmPlot.h"
#include "Crop.h" 
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

    // Tạo FarmPlot ở những ô có ID khác -1
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

FarmPlot::FarmPlot(float x, float y, float size)
    : m_stage(CropStage::Empty)
    , m_timeInStage(0.0f)
    , m_highlighted(false)
    , m_soilState(SoilState::Dry)
{
    if (s_texture.getSize().x == 0)
        s_texture.loadFromFile(RESOURCES_PATH "Tilesets/Decor.png");

    m_sprite.setTexture(s_texture);

    const int tilesPerRow = 16; // vì texture 512x512 và mỗi tile 32x32
    int id = 32; // đất khô
    int tu = id % tilesPerRow;
    int tv = id / tilesPerRow;

    m_sprite.setTextureRect(sf::IntRect(tu * 32, tv * 32, 32, 32));
    m_sprite.setPosition(x, y);
}

//Phương thức update
void FarmPlot::update(float deltaTime)
{
    if (m_stage == CropStage::Empty || m_stage == CropStage::Dead || m_cropName.empty())
    {
        // Không có gì để update
        return;
    }

    if (m_stage == CropStage::Harvestable)
    {
        // Cây đã sẵn sàng thu hoạch, không cần tăng thời gian nữa.
        return;
    }

    // Lấy dữ liệu cây trồng
    if (CropDatabase.count(m_cropName))
    {
        const CropData& data = CropDatabase.at(m_cropName);
        m_timeInStage += deltaTime;

        // Xử lý chuyển đổi giai đoạn
        if (m_stage == CropStage::Seed && m_timeInStage >= data.seedDuration)
        {
            m_stage = CropStage::Growing;
            m_timeInStage = 0.0f;
        }
        else if (m_stage == CropStage::Growing && m_timeInStage >= data.growthDuration)
        {
            m_stage = CropStage::Harvestable;
            m_timeInStage = 0.0f;
        }
    }
    else
    {
        // Xử lý lỗi nếu không tìm thấy CropData (để an toàn)
        std::cerr << "Lỗi: Không tìm thấy CropData cho " << m_cropName << std::endl;
        m_stage = CropStage::Empty;
        m_cropName = "";
    }
}

// Triển khai các chức năng còn lại

void FarmPlot::plant(const std::string& cropName)
{
    m_cropName = cropName;
    m_stage = CropStage::Seed;
    m_timeInStage = 0.0f;
}

void FarmPlot::setHighlight(bool value)
{
    m_highlighted = value;
}

void FarmPlot::updateTexture()
{
    // Mỗi tile có kích thước 32x32
    sf::IntRect rect;

    if (m_soilState == SoilState::Dry)
        rect = sf::IntRect(0, 0, 32, 32);     // Ô đất khô
    else if (m_soilState == SoilState::Wet)
        rect = sf::IntRect(32, 0, 32, 32);    // Ô đất ướt (ID = 32)

    m_sprite.setTextureRect(rect);
}

void FarmPlot::water()
{
    m_soilState = SoilState::Wet;

    const int tilesPerRow = 16; // 512 / 32
    int id = 48;                // tile ID cho đất ướt
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

    const int tilesPerRow = 16; // 512 / 32
    int id = 32;                // tile ID cho đất khô
    int tu = id % tilesPerRow;
    int tv = id / tilesPerRow;

    m_sprite.setTextureRect(sf::IntRect(tu * 32, tv * 32, 32, 32));
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

    if (m_highlighted) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
        overlay.setPosition(m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top);
        overlay.setFillColor(sf::Color(255, 255, 0, 60)); // vàng trong suốt
        target.draw(overlay, states);
    }
}