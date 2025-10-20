
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
	int seedDuration; // Thời gian hạt giống
	int seedPrice;      // Giá mua
	int sellPrice;      // Giá bán 
	float growthDuration; // Time for grow
	std::string harvestedItem; // Item khi thu hoạch
};

// Database plant 
const std::map<std::string, CropData> CropDatabase = 
{
	{"Carrot", {"Cà rốt",10, 10, 30, 10.0f, " Cà rốt"}},
	{"Tomato", {"Cà chua",20, 20, 50, 15.0f, " Cà chua"}},
	{"Corn", {"Ngô",30, 30, 80, 20.0f, " Ngô"}}
};

std::string getCropStageName(CropStage stage);