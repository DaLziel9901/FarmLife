
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "Crop.h" 
#include "Animation.h"

enum class Direction { Down = 0, Left, Right, Up };

//Tốc độ di chuyển người chơi
const float PLAYER_SPEED = 150.0f;
const int PLAYER_FRAME_WIDTH = 32;
const int PLAYER_FRAME_HEIGHT = 64;

class Player : public sf::Drawable, public sf::Transformable
{
public:
    Player(long long initialMoney, const std::string& startingSeed);
    ~Player();

	void update(float deltaTime);
    void handleInput();


    // Getters
    long long getMoney() const;
    const std::map<std::string, int>& getInventory() const;
    const std::string& getSelectedSeed() const;
	const sf::Sprite& getSprite() const { return m_sprite; }

	// Actions
    void addMoney(long long amount);
    bool buySeed(const std::string& cropName, int count = 1);
    bool sellCrop(const std::string& cropName, int count = 1);

    bool tryPlantSelectedSeed(const std::string& seedName);
    void addHarvestedCrop(const std::string& cropName);
    void setSelectedSeed(const std::string& cropName);

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;

    long long m_money;
    std::map<std::string, int> m_inventory;
    std::string m_selectedSeed;

	sf::Vector2f m_velocity; // Vận tốc hiện tại
	bool m_isMoving; // Trạng thái di chuyển

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Direction m_direction;
    Animation* m_anim;
};