#include "Building.h"
#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <memory>

void EnergyStorage::update(float dt, Game& game) {
    (void)dt;
    (void)game;
}

void SolarPanel::update(float dt, Game& game) {
    if (game.maxEnergy > 0 && game.currentEnergy < game.maxEnergy) {
        game.currentEnergy += GameConstants::SOLAR_PANEL_ENERGY_PER_SEC * dt;
    }
}

WindTurbine::WindTurbine() : m_currentFrame(0) {}

void WindTurbine::update(float dt, Game& game) {
    if (game.maxEnergy > 0 && game.currentEnergy < game.maxEnergy) {
        game.currentEnergy += GameConstants::WIND_TURBINE_ENERGY_PER_SEC * dt;
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
