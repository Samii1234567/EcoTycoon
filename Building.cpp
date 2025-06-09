#include "Building.h"
#include "Game.h"
#include "Constants.h"

void EnergyStorage::update(float dt, Game& game) {
}

void SolarPanel::update(float dt, Game& game) {
    if (game.currentEnergy < game.maxEnergy) {
        game.currentEnergy += GameConstants::SOLAR_PANEL_ENERGY_PER_SEC * dt;
    }
}

void WindTurbine::update(float dt, Game& game) {
    if (game.currentEnergy < game.maxEnergy) {
        game.currentEnergy += GameConstants::WIND_TURBINE_ENERGY_PER_SEC * dt;
    }
    if (game.environmentHealth < 100.f) {
        game.environmentHealth += GameConstants::WIND_TURBINE_ENV_REGEN_PER_SEC * dt;
    }
}
