#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Constants.h"

class Game;

class Building {
public:
    Building() = default;
    virtual ~Building() = default;
    virtual void update(float dt, Game& game) = 0;
    virtual sf::IntRect getTextureRect() const { return {}; }
};

class EnergyStorage : public Building {
public:
    void update(float dt, Game& game) override;
};

class SolarPanel : public Building {
public:
    void update(float dt, Game& game) override;
private:
    float m_productionAccumulator = 0.0f;
};

class WindTurbine : public Building {
public:
    WindTurbine();
    void update(float dt, Game& game) override;
    sf::IntRect getTextureRect() const override;
private:
    float m_productionAccumulator = 0.0f;
    sf::Clock m_animClock;
    int m_currentFrame;
};

// Funkcja fabrykująca, przeniesiona do nagłówka jako inline, aby uniknąć błędów linkera
inline std::unique_ptr<Building> createBuildingById(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID:
        return std::make_unique<EnergyStorage>();
    case GameConstants::SOLAR_PANEL_ID:
        return std::make_unique<SolarPanel>();
    case GameConstants::WIND_TURBINE_ID:
        return std::make_unique<WindTurbine>();
    default:
        std::cerr << "Nieznane ID budynku podczas tworzenia: " << typeId << std::endl;
        return nullptr;
    }
}

#endif // BUILDING_H
