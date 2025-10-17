// include/Crop.hpp

#pragma once

#include <string>
#include <map>
#include <iostream>

enum class CropStage
{
	Empty, 
	Seed,
	Growing,
	Harvestable,
	Dead
};

struct CropData
{
	std::string name;
	int seedPrice;      // Giá mua
	int sellPrice;      // Giá bán 
	float growthDurationSeconds; // Time for grow
};

// Database plant 
const std::map<std::string, CropData> CropDatabase = {
	{"Carrot", {"Cà r?t", 10, 30, 10.0f}},
	{"Tomato", {"Cà chua", 20, 50, 15.0f}},
	{"Corn", {"Ngô", 30, 80, 20.0f}}
};

inline std::string getCropStageName(CropStage stage)
{
	switch (stage)
	{
	case CropStage::Empty:
		return "Empty";
	case CropStage::Seed:
		return "Seed";
	case CropStage::Growing:
		return "Growing";
	case CropStage::Harvestable:
		return "Harvestable (READY!)";
	case CropStage::Dead:
		return "Dead";
	default:
		return "Unknown";
	}
}