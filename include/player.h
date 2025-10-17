// include/Player.hpp

#pragma once

#include <string>
#include <map>
#include "Crop.h" 

class Player
{
public:
    Player(long long initialMoney, const std::string& startingSeed);

    // Getters
    long long getMoney() const;
    const std::map<std::string, int>& getInventory() const;
    const std::string& getSelectedSeed() const;

    // Actions
    void addMoney(long long amount);
    bool buySeed(const std::string& cropName, int count = 1);
    bool sellCrop(const std::string& cropName, int count = 1);

    // Logic tương tác với đất
    bool tryPlantSelectedSeed(const std::string& seedName);
    void addHarvestedCrop(const std::string& cropName);
    void setSelectedSeed(const std::string& cropName);

private:
    long long m_money;
    std::map<std::string, int> m_inventory;
    std::string m_selectedSeed;
};