#include "Building.h"
#include "Game.h"
#include <cmath>

void EnergyStorage::update(float dt, Game& game, PlacedObject& self) {
    game.environmentHealth += GameConstants::STORAGE_DATA.envEffect[self.level - 1] * dt;
}

void SolarPanel::update(float dt, Game& game, PlacedObject& self) {
    float productionRate = GameConstants::SOLAR_PANEL_DATA.value[self.level - 1];

    // ##### ZMIANA #####
    // Odwołujemy się do WeatherManager, aby pobrać modyfikator
    m_productionAccumulator += productionRate * game.getWeatherManager().getWeatherMultiplierSolar() * dt;
    // ##################

    if (m_productionAccumulator >= 1.f) {
        int energyGained = static_cast<int>(floor(m_productionAccumulator));
        if (game.currentEnergy < game.maxEnergy) {
            game.currentEnergy += energyGained;
        }
        m_productionAccumulator -= energyGained;
    }

    game.environmentHealth += GameConstants::SOLAR_PANEL_DATA.envEffect[self.level - 1] * dt;
}

WindTurbine::WindTurbine() : m_currentFrame(0) {}

void WindTurbine::update(float dt, Game& game, PlacedObject& self) {
    float energyRate = GameConstants::WIND_TURBINE_DATA.value[self.level - 1];

    // ##### ZMIANA #####
    // Odwołujemy się do WeatherManager, aby pobrać modyfikator
    m_productionAccumulator += energyRate * game.getWeatherManager().getWeatherMultiplierWind() * dt;
    // ##################

    if (m_productionAccumulator >= 1.f) {
        int energyGained = static_cast<int>(floor(m_productionAccumulator));
        if (game.currentEnergy < game.maxEnergy) {
            game.currentEnergy += energyGained;
        }
        m_productionAccumulator -= energyGained;
    }

    float envRate = GameConstants::WIND_TURBINE_DATA.envEffect[self.level - 1];
    if (game.environmentHealth < 100.f) {
        game.environmentHealth += envRate * dt;
    }

    if (m_animClock.getElapsedTime().asSeconds() > GameConstants::TURBINE_ANIM_SPEED_SEC) {
        m_currentFrame = (m_currentFrame + 1) % GameConstants::TURBINE_FRAME_COUNT;
        m_animClock.restart();
    }
}

sf::IntRect WindTurbine::getTextureRect() const {
    return sf::IntRect(
        m_currentFrame * GameConstants::TURBINE_FRAME_WIDTH,
        0,
        GameConstants::TURBINE_FRAME_WIDTH,
        GameConstants::TURBINE_FRAME_HEIGHT
        );
}

void AirFilter::update(float dt, Game& game, PlacedObject& self) {
    float energyNeeded = GameConstants::AIR_FILTER_DATA.value[self.level - 1] * dt;
    if (game.currentEnergy >= static_cast<int>(ceil(energyNeeded))) {
        game.currentEnergy -= static_cast<int>(ceil(energyNeeded));

        float envRegen = GameConstants::AIR_FILTER_DATA.envEffect[self.level - 1];
        game.environmentHealth += envRegen * dt;
    }
}
