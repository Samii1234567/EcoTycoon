#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/System/Vector2.hpp>

namespace GameConstants {
// --- Siatka ---
constexpr float GRID_CELL_SIZE = 100.f;
constexpr int GRID_COLS = 12;
constexpr int GRID_ROWS = 6;

// --- Identyfikatory typów budynków ---
constexpr int ENERGY_STORAGE_ID = 0;
constexpr int SOLAR_PANEL_ID = 1;
constexpr int WIND_TURBINE_ID = 2;

// --- Rozmiary budynków (w komórkach siatki) ---
const sf::Vector2i ENERGY_STORAGE_SIZE = {2, 2};
const sf::Vector2i SOLAR_PANEL_SIZE = {1, 1};
const sf::Vector2i WIND_TURBINE_SIZE = {1, 1};

// --- Właściwości budynków ---
constexpr int ENERGY_STORAGE_CAPACITY_INCREASE = 100;
constexpr int SOLAR_PANEL_ENERGY_PER_SEC = 2;
constexpr int WIND_TURBINE_ENERGY_PER_SEC = 5;
constexpr float WIND_TURBINE_ENV_REGEN_PER_SEC = 0.1f;

// --- Animacja ---
constexpr int TURBINE_FRAME_WIDTH = 128;
constexpr int TURBINE_FRAME_HEIGHT = 128;
constexpr int TURBINE_FRAME_COUNT = 4;
constexpr float TURBINE_ANIM_SPEED_SEC = 0.15f;

// --- Ekonomia ---
constexpr int ENERGY_SELL_PRICE = 2;
}

#endif // CONSTANTS_H
