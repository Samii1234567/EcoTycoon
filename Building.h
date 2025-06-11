#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Constants.h"

class Game;
class Building;

// Definicja PlacedObject jest tutaj, aby uniknąć cyklicznych zależności
struct PlacedObject {
    int typeId;
    sf::Sprite sprite;
    std::unique_ptr<Building> logic;
    int price;
    int level;
    sf::Vector2i gridPosition;
    sf::Vector2i sizeInCells;
    bool isDamaged = false;
};

class Building {
public:
    Building() = default;
    virtual ~Building() = default;
    // Ujednolicona sygnatura metody update, obsługująca ulepszenia
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

class AirFilter : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};

// Zaktualizowana fabryka o nowy budynek
inline std::unique_ptr<Building> createBuildingById(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID:
        return std::make_unique<EnergyStorage>();
    case GameConstants::SOLAR_PANEL_ID:
        return std::make_unique<SolarPanel>();
    case GameConstants::WIND_TURBINE_ID:
        return std::make_unique<WindTurbine>();
    case GameConstants::AIR_FILTER_ID:
        return std::make_unique<AirFilter>();
    default:
        std::cerr << "Nieznane ID budynku podczas tworzenia: " << typeId << std::endl;
        return nullptr;
    }
}

#endif // BUILDING_H
