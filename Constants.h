#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace GameConstants {
// --- Siatka ---
constexpr float GRID_CELL_SIZE = 100.f;
constexpr int GRID_COLS = 12;
constexpr int GRID_ROWS = 6;

// --- Identyfikatory typów budynków ---
constexpr int ENERGY_STORAGE_ID = 0;
constexpr int SOLAR_PANEL_ID = 1;
constexpr int WIND_TURBINE_ID = 2;
constexpr int AIR_FILTER_ID = 3;

// --- Rozmiary budynków (w komórkach siatki) ---
const sf::Vector2i ENERGY_STORAGE_SIZE = {2, 2};
const sf::Vector2i SOLAR_PANEL_SIZE = {1, 1};
const sf::Vector2i WIND_TURBINE_SIZE = {1, 1};
const sf::Vector2i AIR_FILTER_SIZE = {2, 1};

// --- Właściwości i ulepszenia budynków ---
constexpr int MAX_LEVEL = 3;

struct BuildingLevelData {
    std::vector<int> upgradeCost;
    std::vector<int> value;
    std::vector<float> envEffect;
};

const BuildingLevelData STORAGE_DATA = {
    {0, 750, 1500}, // Koszt ulepszenia
    {100, 250, 500}, // Wartość (pojemność)
    {-0.05f, -0.05f, -0.05f} // Wpływ na środowisko (negatywny)
};

const BuildingLevelData SOLAR_PANEL_DATA = {
    {0, 300, 600}, // Koszt ulepszenia
    {2, 5, 10}, // Wartość (produkcja E/s)
    {-0.02f, -0.02f, -0.02f} // Wpływ na środowisko (negatywny)
};

const BuildingLevelData WIND_TURBINE_DATA = {
    {0, 750, 1500}, // Koszt ulepszenia
    {5, 12, 25}, // Wartość (produkcja E/s)
    {0.1f, 0.2f, 0.3f} // Wpływ na środowisko (pozytywny)
};

const BuildingLevelData AIR_FILTER_DATA = {
    {0, 2000, 4000}, // Koszt ulepszenia
    {10, 15, 20}, // Wartość (koszt E/s)
    {0.4f, 0.6f, 0.8f} // Wpływ na środowisko (pozytywny)
};


// --- Animacja ---
constexpr int TURBINE_FRAME_WIDTH = 128;
constexpr int TURBINE_FRAME_HEIGHT = 128;
constexpr int TURBINE_FRAME_COUNT = 4;
constexpr float TURBINE_ANIM_SPEED_SEC = 0.15f;

// --- Ekonomia ---
constexpr int ENERGY_SELL_PRICE = 2;
}

inline sf::Vector2i getBuildingSize(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID: return GameConstants::ENERGY_STORAGE_SIZE;
    case GameConstants::SOLAR_PANEL_ID:    return GameConstants::SOLAR_PANEL_SIZE;
    case GameConstants::WIND_TURBINE_ID:   return GameConstants::WIND_TURBINE_SIZE;
    case GameConstants::AIR_FILTER_ID:     return GameConstants::AIR_FILTER_SIZE;
    default:                               return {1, 1};
    }
}

#endif // CONSTANTS_H
