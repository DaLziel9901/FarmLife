
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
	{"Corn", {"Ngô",30, 30, 80, 20.0f}},
	{"Wheat",{"Lúa mì",6, 8, 20, 8.0f}},
	{"Strawberry",{"Dâu tây",9,25 ,60 , 18.0f}},
	{"Cabbage",{"Bắp cải",7, 12, 35, 12.0f}},
	{"Watermelon",{"Dưa hấu",18, 45, 120, 30.0f}},
	{"Onion",{"Hành tây",6, 10, 28, 9.0f}},
};

std::string getCropStageName(CropStage stage);