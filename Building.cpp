#include "Building.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <memory>
#include <cmath>

void EnergyStorage::update(float dt, Game& game) {
    (void)dt;
    (void)game;
}

void SolarPanel::update(float dt, Game& game) {
    m_productionAccumulator += GameConstants::SOLAR_PANEL_ENERGY_PER_SEC * dt;
    if (m_productionAccumulator >= 1.f) {
        int energyGained = static_cast<int>(floor(m_productionAccumulator));
        if (game.currentEnergy < game.maxEnergy) {
            game.currentEnergy += energyGained;
        }
        m_productionAccumulator -= energyGained;
    }
}

WindTurbine::WindTurbine() : m_currentFrame(0) {}

void WindTurbine::update(float dt, Game& game) {
    m_productionAccumulator += GameConstants::WIND_TURBINE_ENERGY_PER_SEC * dt;
    if (m_productionAccumulator >= 1.f) {
        int energyGained = static_cast<int>(floor(m_productionAccumulator));
        if (game.currentEnergy < game.maxEnergy) {
            game.currentEnergy += energyGained;
        }
        m_productionAccumulator -= energyGained;
    }

    if (game.environmentHealth < 100.f) {
        game.environmentHealth += GameConstants::WIND_TURBINE_ENV_REGEN_PER_SEC * dt;
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
