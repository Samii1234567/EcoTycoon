#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Constants.h"

class Game;
class Building;

// Przeniesiono definicję PlacedObject tutaj, aby uniknąć cyklicznych zależności
struct PlacedObject {
    int typeId;
    sf::Sprite sprite;
    std::unique_ptr<Building> logic;
    int price;
    int level; // Aktualny poziom budynku
    sf::Vector2i gridPosition;
    sf::Vector2i sizeInCells;
};

class Building {
public:
    Building() = default;
    virtual ~Building() = default;
    // ZMIANA: Update przyjmuje teraz referencję do swojego obiektu PlacedObject
    virtual void update(float dt, Game& game, PlacedObject& self) = 0;
    virtual sf::IntRect getTextureRect() const { return {}; }
};

class EnergyStorage : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};

class SolarPanel : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
private:
    float m_productionAccumulator = 0.0f;
};

class WindTurbine : public Building {
public:
    WindTurbine();
    void update(float dt, Game& game, PlacedObject& self) override;
    sf::IntRect getTextureRect() const override;
private:
    float m_productionAccumulator = 0.0f;
    sf::Clock m_animClock;
    int m_currentFrame;
};

inline std::unique_ptr<Building> createBuildingById(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID: return std::make_unique<EnergyStorage>();
    case GameConstants::SOLAR_PANEL_ID:    return std::make_unique<SolarPanel>();
    case GameConstants::WIND_TURBINE_ID:   return std::make_unique<WindTurbine>();
    default:                               return nullptr;
    }
}

#endif
