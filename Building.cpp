#include "Building.h"
#include "Game.h"
#include "Constants.h"
#include <cmath>

void EnergyStorage::update(float dt, Game& game, PlacedObject& self) {
    (void)dt; (void)game; (void)self;
}

void SolarPanel::update(float dt, Game& game, PlacedObject& self) {
    float productionRate = GameConstants::SOLAR_PANEL_DATA.value[self.level - 1];
    m_productionAccumulator += productionRate * dt;

    if (m_productionAccumulator >= 1.f) {
        int energyGained = static_cast<int>(floor(m_productionAccumulator));
        if (game.currentEnergy < game.maxEnergy) {
            game.currentEnergy += energyGained;
        }
        m_productionAccumulator -= energyGained;
    }
}

WindTurbine::WindTurbine() : m_currentFrame(0) {}

void WindTurbine::update(float dt, Game& game, PlacedObject& self) {
    float energyRate = GameConstants::WIND_TURBINE_DATA.value[self.level - 1];
    m_productionAccumulator += energyRate * dt;
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
