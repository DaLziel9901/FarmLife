#pragma once
// include/FarmPlot.hpp

#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Crop.h"

class FarmPlot : public sf::Drawable, public sf::Transformable
{
public:
	// Khởi tạo ô đất tại vị trí (x, y) với kích thước size
	FarmPlot(float x, float y, float size) : m_cropName(""), m_currentStage(CropStage::Empty), m_growthTimer(0.0f), m_size(size)
	{
		m_shape.setSize(sf::Vector2f(size, size));
		m_shape.setPosition(x, y);
		m_shape.setOutlineThickness(1.0f);
		m_shape.setOutlineColor(sf::Color::Black);
		updateAppearance();
	}

	void plant(const std::string& cropName)
	{
		if (m_currentStage == CropStage::Empty && CropDatabase.count(cropName))
		{
			m_cropName = cropName;
			m_currentStage = CropStage::Seed;
			m_growthTimer = 0.0f;
			updateAppearance();
		}
	}

	void update(float deltaTime)
	{
		if (m_currentStage == CropStage::Seed || m_currentStage == CropStage::Growing)
		{
			m_growthTimer += deltaTime;
			const auto& data = CropDatabase.at(m_cropName);
			float halfDuration = data.growthDurationSeconds / 2.0f;

			// Kiểm tra chuyển sang trạng thái Thu hoạch
			if (m_growthTimer >= data.growthDurationSeconds && m_currentStage != CropStage::Harvestable)
			{
				m_currentStage = CropStage::Harvestable;
				updateAppearance();
			}
			// Kiểm tra chuyển sang trạng thái Đang lớn
			else if (m_growthTimer > halfDuration && m_currentStage == CropStage::Seed)
			{
				m_currentStage = CropStage::Growing;
				updateAppearance();
			}
		}
	}

	std::string harvest()
	{
		if (m_currentStage == CropStage::Harvestable)
		{
			std::string harvestedCrop = m_cropName;
			m_cropName = "";
			m_currentStage = CropStage::Empty;
			m_growthTimer = 0.0f;
			updateAppearance();
			return harvestedCrop;
		}
		return ""; // Không có gì để thu hoạch
	}

	// Getters
	bool isEmpty() const { return m_currentStage == CropStage::Empty; }
	CropStage getStage() const { return m_currentStage; }
	const std::string& getCropName() const { return m_cropName; }
	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }

private:
	sf::RectangleShape m_shape;
	std::string m_cropName;
	CropStage m_currentStage;
	float m_growthTimer;
	float m_size;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_shape, states);

		if (m_currentStage != CropStage::Empty)
		{
			// Vẽ biểu tượng cây trồng (thay thế bằng Sprite/Texture sau)
			sf::CircleShape cropVisual(m_size / 4.f);
			cropVisual.setOrigin(m_size / 4.f, m_size / 4.f);
			cropVisual.setPosition(m_shape.getPosition() + sf::Vector2f(m_size / 2.f, m_size / 2.f));

			if (m_currentStage == CropStage::Seed)
				cropVisual.setFillColor(sf::Color(100, 100, 100)); // Hạt giống: Xám
			else if (m_currentStage == CropStage::Growing)
				cropVisual.setFillColor(sf::Color(0, 150, 0)); // Đang lớn: Xanh lá
			else if (m_currentStage == CropStage::Harvestable)
				cropVisual.setFillColor(sf::Color::Yellow); // Thu hoạch: Vàng

			target.draw(cropVisual, states);
		}
	}

	void updateAppearance()
	{
		// Màu đất cơ bản (Nâu)
		m_shape.setFillColor(sf::Color(139, 69, 19));

		// Màu đất trống hơi sáng hơn
		if (m_currentStage == CropStage::Empty)
			m_shape.setFillColor(sf::Color(160, 120, 80));

		// Highlight nếu có thể thu hoạch
		if (m_currentStage == CropStage::Harvestable)
			m_shape.setFillColor(sf::Color(160, 82, 45));
	}
};