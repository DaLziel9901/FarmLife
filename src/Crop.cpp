#include "FarmPlot.h"
#include "Crop.h"
#include <iostream>

std::string getCropStageName(CropStage stage)
{
    switch (stage)
    {
    case CropStage::Empty: return "Empty";
    case CropStage::Seed: return "Seed";
    case CropStage::GrowingStage1: return "GrowingStage1";
    case CropStage::GrowingStage2: return "GrowingStage2";
    case CropStage::Harvestable: return "Harvestable";
    case CropStage::Dead: return "Dead";
    default: return "Unknown";
    }
}