// src/Player.cpp

#include "player.h"
#include "Crop.h"
#include <iostream>

Player::Player(long long initialMoney, const std::string& startingSeed)
    : m_money(initialMoney)
{
    // Cấp cho người chơi 5 hạt giống loại ban đầu
    if (CropDatabase.count(startingSeed)) {
        m_inventory[startingSeed] = 5;
        m_selectedSeed = startingSeed;
    }
    else {
        m_selectedSeed = "";
    }
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

        // Tự động chọn hạt giống vừa mua nếu chưa có hạt nào được chọn
        if (m_selectedSeed.empty() || m_inventory.count(m_selectedSeed) == 0 || m_inventory.at(m_selectedSeed) == 0) {
            m_selectedSeed = cropName;
        }

        std::cout << "[Player] Bought " << count << "x " << cropName << ". Money left: " << m_money << std::endl;
        return true;
    }

    std::cout << "[Player] Not enough money to buy " << cropName << "." << std::endl;
    return false;
}

bool Player::sellCrop(const std::string& cropName, int count)
{
    if (CropDatabase.count(cropName) == 0 || m_inventory.count(cropName) == 0 || m_inventory.at(cropName) < count)
    {
        return false;
    }

    const auto& data = CropDatabase.at(cropName);
    long long earned = (long long)data.sellPrice * count;

    m_money += earned;
    m_inventory[cropName] -= count;

    // Nếu bán hết hạt giống đang chọn, đặt lại selectedSeed
    if (m_inventory[cropName] == 0 && m_selectedSeed == cropName) {
        m_selectedSeed = "";
    }

    std::cout << "[Player] Sold " << count << "x " << cropName << ". Earned: " << earned << ". New money: " << m_money << std::endl;
    return true;
}

bool Player::tryPlantSelectedSeed(const std::string& seedName)
{
    if (m_inventory.count(seedName) && m_inventory.at(seedName) > 0)
    {
        m_inventory[seedName]--;

        // Nếu dùng hết hạt giống đang chọn, tìm hạt giống khác có sẵn để chọn
        if (m_inventory[seedName] == 0)
        {
            std::string nextSeed = "";
            for (auto const& [name, count] : m_inventory) {
                if (count > 0 && CropDatabase.count(name)) {
                    nextSeed = name;
                    break;
                }
            }
            m_selectedSeed = nextSeed;
        }
        return true;
    }

    std::cout << "[Player] No " << seedName << " seeds left in inventory." << std::endl;
    return false;
}

void Player::addHarvestedCrop(const std::string& cropName)
{
    m_inventory[cropName]++;
    std::cout << "[Plot] Harvested " << cropName << std::endl;
}

void Player::setSelectedSeed(const std::string& cropName)
{
    if (m_inventory.count(cropName) && m_inventory.at(cropName) > 0)
    {
        m_selectedSeed = cropName;
        std::cout << "[Player] Selected seed: " << cropName << std::endl;
    }
    else
    {
        std::cout << "[Player] Cannot select " << cropName << ". Not in inventory." << std::endl;
    }
}