
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
	float growthDuration; // Time for grow
	std::string harvestedItem; // Item khi thu hoạch
	int spriteBaseID; // ID cơ sở trong tileset
	int spriteHeight; // Chiều cao sprite
};

// Database plant 
const std::map<std::string, CropData> CropDatabase = 
{
<<<<<<< HEAD
	{"Carrot", {"Cà rốt",10, 10, 30, 10.0f}},
	{"Tomato", {"Cà chua",20, 20, 50, 15.0f}},
	{"Corn", {"Ngô",30, 30, 80, 20.0f}},
	{"Wheat",{"Lúa mì",6, 8, 20, 8.0f}},
	{"Strawberry",{"Dâu tây",9,25 ,60 , 18.0f}},
	{"Cabbage",{"Bắp cải",7, 12, 35, 12.0f}},
	{"Watermelon",{"Dưa hấu",18, 45, 120, 30.0f}},
	{"Onion",{"Hành tây",6, 10, 28, 9.0f}},
=======
	{"Carrot", {"Cà rốt",10, 10, 30, 5.0f, " Cà rốt", 112, 2}},
	{"Tomato", {"Cà chua",20, 20, 50, 7.5f, " Cà chua", 144, 2}},
	{"Corn", {"Ngô",30, 30, 80, 10.0f, " Ngô", 116, 2}}
>>>>>>> temp
};

std::string getCropStageName(CropStage stage);