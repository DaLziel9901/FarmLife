#include "Animation.h"
#include "player.h"
#include "Crop.h"
#include "AudioManager.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <SFML/Window/Keyboard.hpp>

Player::Player(long long initialMoney, const std::string& startingSeed)
    : m_money(initialMoney) ,m_direction(Direction::Down)
{
	// Texture và Sprite người chơi
    if (!m_texture.loadFromFile(RESOURCES_PATH "Sprite/farmer.png"))
    {
		std::cerr << "[Player] Error loading farmer.png" << std::endl;
		// Set sprite mặc định
		m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 64));
    }
    else
    {
		// Set sprite với texture đã load
		std::cout << "[Player] Successfully loaded farmer.png" << std::endl;
		m_sprite.setTexture(m_texture);
        m_sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_FRAME_WIDTH, PLAYER_FRAME_HEIGHT));

        //Scale
		m_sprite.setScale(1.0f, 1.0f);

        m_sprite.setOrigin(PLAYER_FRAME_WIDTH / 2.f, PLAYER_FRAME_HEIGHT / 2.f); 
    }

    //Set vị trí khởi đầu player
    m_sprite.setPosition(466.0f, 545.0f);
    setPosition(m_sprite.getPosition());

    //Inventory khởi đầu
    if (CropDatabase.count(startingSeed)) {
        m_inventory[startingSeed] = 5;
        m_selectedSeed = startingSeed;
    }
    else {
        m_selectedSeed = "";
    }

    m_anim = new Animation(m_sprite, 32, 64, 0.15f);
}

void Player::handleInput()
{
	// Reset vận tốc every frame
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    bool moving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_velocity.y -= 1.0f;
        m_direction = Direction::Up; 
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_velocity.y += 1.0f;
        m_direction = Direction::Down; 
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_velocity.x -= 1.0f;
        m_direction = Direction::Left;
        moving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_velocity.x += 1.0f;
        m_direction = Direction::Right; 
        moving = true;
    }

    if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
    {
        float length = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
        m_velocity.x /= length;
        m_velocity.y /= length;
    }

    if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
    {
        m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 64)); // Walking frame
    }
    else
    {
        m_sprite.setTextureRect(sf::IntRect(0, 0, 32, 64)); // Idle frame
    }
	m_isMoving = moving;
}

void Player::update(float deltaTime)
{
    m_sprite.move(m_velocity * PLAYER_SPEED * deltaTime);
    if (m_anim)
        m_anim->update(deltaTime, static_cast<int>(m_direction), m_isMoving);
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

long long Player::getMoney() const
{
    return m_money;
}

const std::map<std::string, int>& Player::getInventory() const
{
    return m_inventory;
}

const std::string& Player::getSelectedSeed() const
{
    return m_selectedSeed;
}

void Player::addMoney(long long amount)
{
    m_money += amount;
}

bool Player::buySeed(const std::string& cropName, int count)
{
    if (CropDatabase.count(cropName) == 0) return false;

    const auto& data = CropDatabase.at(cropName);
    long long cost = (long long)data.seedPrice * count;

    if (m_money >= cost)
    {
        m_money -= cost;
        m_inventory[cropName] += count;

        if (m_selectedSeed.empty() || m_inventory.count(m_selectedSeed) == 0 || m_inventory.at(m_selectedSeed) == 0) {
            m_selectedSeed = cropName;
        }
        return true;
    }
    return false;
}

bool Player::sellCrop(const std::string& cropName, int count)
{
    auto it = m_inventory.find(cropName);
    if (it == m_inventory.end() || it->second < count)
        return false;

    std::string baseCrop = "";
    for (auto const& [key, data] : CropDatabase)
    {
        if (data.harvestedItem == cropName)
        {
            baseCrop = key;
            break;
        }
    }

    if (baseCrop.empty()) return false;

    int totalMoney = CropDatabase.at(baseCrop).sellPrice * count;
    m_inventory[cropName] -= count;
    if (m_inventory[cropName] <= 0) m_inventory.erase(cropName);

    m_money += totalMoney;
    return true;
}

bool Player::tryPlantSelectedSeed(const std::string& seedName)
{
    if (m_inventory.count(seedName) && m_inventory.at(seedName) > 0)
    {
        m_inventory[seedName]--;
        return true;
    }
    return false;
}


void Player::addHarvestedCrop(const std::string& cropName)
{
    m_inventory[cropName]++;
}

void Player::setSelectedSeed(const std::string& cropName)
{
    if (m_inventory.count(cropName) && m_inventory.at(cropName) > 0)
    {
        m_selectedSeed = cropName;
    }
}


Player::~Player()
{
    if (m_anim)
    {
        delete m_anim;
        m_anim = nullptr;
    }
}