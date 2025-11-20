
#pragma once

#include <string>
#include <map>
#include <iostream>

enum class CropStage
{
	Empty, 
	Seed,
	GrowingStage1,
	GrowingStage2,
	Harvestable,
	Dead
};

struct CropData
{
	std::string name;
	int seedDuration; // Thời gian hạt giống
	int seedPrice;      // Giá mua
	int sellPrice;      // Giá bán 
	float growthDuration; // Thời gian phát triển
	std::string harvestedItem; // Item thu hoạch
	int spriteBaseID; // ID trong tileset
	int spriteHeight; // Chiều cao sprite
};

// Database plant 
const std::map<std::string, CropData> CropDatabase = 
{
	{"Cà rốt", {"Cà rốt",10, 10, 30, 5.0f, "Cà rốt", 112, 2}},
	{"Cà chua", {"Cà chua",20, 20, 50, 7.5f, "Cà chua", 144, 2}},
	{"Ngô", {"Ngô",30, 30, 80, 10.0f, "Ngô", 116, 2}},
	{"Lúa mì",{"Lúa mì",6, 8, 20, 4.0f, "Lúa mì", 24, 2}},
	{"Dâu tây",{"Dâu tây",9,25 ,60 , 9.0f, "Dâu tây", 80, 1}},
	{"Bắp cải",{"Bắp cải",7, 12, 35, 6.0f, "Bắp cải", 52, 2}},
	{"Dưa hấu",{"Dưa hấu",18, 45, 120, 15.0f, "Dưa hấu", 176, 2}},
	{"Ớt",{"Ớt",6, 10, 28, 4.5f,"Ớt", 152, 2 }},
};

std::string getCropStageName(CropStage stage);