
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
};

// Database plant 
const std::map<std::string, CropData> CropDatabase = 
{
	{"Carrot", {"Cà rốt",10, 10, 30, 10.0f}},
	{"Tomato", {"Cà chua",20, 20, 50, 15.0f}},
	{"Corn", {"Ngô",30, 30, 80, 20.0f}}
};

std::string getCropStageName(CropStage stage);