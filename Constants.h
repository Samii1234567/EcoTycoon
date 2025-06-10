#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/System/Vector2.hpp>

namespace GameConstants {
constexpr float GRID_CELL_SIZE = 100.f;
constexpr int GRID_COLS = 12;
constexpr int GRID_ROWS = 6;

constexpr int ENERGY_STORAGE_ID = 0;
constexpr int SOLAR_PANEL_ID = 1;
constexpr int WIND_TURBINE_ID = 2;

const sf::Vector2i ENERGY_STORAGE_SIZE = {2, 2};
const sf::Vector2i SOLAR_PANEL_SIZE = {1, 1};
const sf::Vector2i WIND_TURBINE_SIZE = {1, 1};

constexpr int ENERGY_STORAGE_CAPACITY_INCREASE = 100;
constexpr int SOLAR_PANEL_ENERGY_PER_SEC = 2;
constexpr int WIND_TURBINE_ENERGY_PER_SEC = 5;
constexpr float WIND_TURBINE_ENV_REGEN_PER_SEC = 0.1f;

constexpr int TURBINE_FRAME_WIDTH = 128;
constexpr int TURBINE_FRAME_HEIGHT = 128;
constexpr int TURBINE_FRAME_COUNT = 4;
constexpr float TURBINE_ANIM_SPEED_SEC = 0.15f;

constexpr float ENERGY_SELL_PRICE = 0.5f;
}

// ZMIANA: Przeniesienie funkcji pomocniczej jako inline do nagłówka
inline sf::Vector2i getBuildingSize(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID: return GameConstants::ENERGY_STORAGE_SIZE;
    case GameConstants::SOLAR_PANEL_ID:    return GameConstants::SOLAR_PANEL_SIZE;
    case GameConstants::WIND_TURBINE_ID:   return GameConstants::WIND_TURBINE_SIZE;
    default:                               return {1, 1};
    }
}

#endif // CONSTANTS_H
