#pragma once
#include <string>
#include <vector>
#include "player.h"
#include "FarmPlot.h"

class SaveManager {
public:
    static bool saveGame(const Player& player, const std::vector<FarmPlot>& plots, const std::string& filePath);
    static bool loadGame(Player& player, std::vector<FarmPlot>& plots, const std::string& filePath);
};
