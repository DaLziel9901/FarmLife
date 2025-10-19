#include "FarmPlot.h"
// Giả định rằng Crop.h chứa enum CropStage và CropDatabase
#include "Crop.h" 
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace FarmGlobals;

FarmPlot::FarmPlot(float x, float y, float size) :
    m_cropName(""),
    m_stage(CropStage::Empty),
    m_timeInStage(0.0f)
{
    m_shape.setSize(sf::Vector2f(size, size));
    m_shape.setPosition(x, y);
    // Màu đất mặc định
    m_shape.setFillColor(sf::Color(139, 69, 19));
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color::Black);
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
        m_shape.setFillColor(sf::Color(100, 250, 100)); // Màu xanh lá cây cho cây có thể thu hoạch
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
            m_shape.setFillColor(sf::Color(150, 150, 80)); // Màu xanh non cho cây đang lớn
        }
        else if (m_stage == CropStage::Growing && m_timeInStage >= data.growthDuration)
        {
            m_stage = CropStage::Harvestable;
            m_timeInStage = 0.0f;
            m_shape.setFillColor(sf::Color(100, 250, 100)); // Màu xanh lá cây đậm cho cây có thể thu hoạch
        }
    }
    else
    {
        // Xử lý lỗi nếu không tìm thấy CropData (để an toàn)
        std::cerr << "Lỗi: Không tìm thấy CropData cho " << m_cropName << std::endl;
        m_stage = CropStage::Empty;
        m_cropName = "";
        m_shape.setFillColor(sf::Color(139, 69, 19));
    }
}

// Triển khai các chức năng còn lại

void FarmPlot::plant(const std::string& cropName)
{
    m_cropName = cropName;
    m_stage = CropStage::Seed;
    m_timeInStage = 0.0f;
    m_shape.setFillColor(sf::Color(160, 82, 45)); // Màu hơi nâu đậm cho đất đã gieo hạt
}

std::string FarmPlot::harvest()
{
    if (m_stage == CropStage::Harvestable)
    {
        std::string harvestedCrop = m_cropName;
        m_cropName = "";
        m_stage = CropStage::Empty;
        m_timeInStage = 0.0f;
        m_shape.setFillColor(sf::Color(139, 69, 19)); // Trở lại màu đất trống
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
    return m_shape.getGlobalBounds();
}

void FarmPlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    // TODO: Thêm logic vẽ hình ảnh cây trồng dựa trên stage tại đây
}

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