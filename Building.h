#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Constants.h"

// Deklaracje wyprzedzające
class Game;
class Building;

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA SYSTEMU BUDYNKÓW
//
//  Definiuje podstawowe struktury i klasy związane z budynkami w grze.
//  `PlacedObject` reprezentuje fizyczną instancję budynku na mapie,
//  a klasa `Building` i jej pochodne definiują logikę dla różnych typów budynków.
//
// ===================================================================================

// Struktura reprezentująca pojedynczy obiekt (budynek) umieszczony na mapie.
struct PlacedObject {
    int typeId;                     // ID typu budynku (z GameConstants)
    sf::Sprite sprite;              // Obiekt graficzny do rysowania
    std::unique_ptr<Building> logic; // Wskaźnik na obiekt z logiką specyficzną dla danego typu
    int price;                      // Cena, za jaką budynek został kupiony
    int level;                      // Aktualny poziom ulepszenia
    sf::Vector2i gridPosition;      // Pozycja na siatce (indeksy)
    sf::Vector2i sizeInCells;       // Rozmiar w komórkach siatki
    bool isDamaged = false;         // Flaga, czy budynek jest uszkodzony
};

// Abstrakcyjna klasa bazowa dla logiki wszystkich budynków.
// Definiuje wspólny interfejs, który muszą implementować wszystkie typy budynków.
class Building {
public:
    Building() = default;
    virtual ~Building() = default; // Wirtualny destruktor jest ważny dla klas bazowych

    // Czysto wirtualna metoda `update`, którą każda klasa pochodna musi zaimplementować.
    // Odpowiada za logikę budynku w każdej klatce gry (np. animacje).
    virtual void update(float dt, Game& game, PlacedObject& self) = 0;

    // Wirtualna metoda zwracająca prostokąt tekstury dla animacji.
    // Domyślnie zwraca pusty prostokąt, co oznacza użycie całej tekstury.
    virtual sf::IntRect getTextureRect() const { return {}; }
};


// --- Konkretne klasy logiki dla każdego typu budynku ---

class EnergyStorage : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};

class SolarPanel : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};

class WindTurbine : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};

class AirFilter : public Building {
public:
    void update(float dt, Game& game, PlacedObject& self) override;
};


// --- Funkcja fabrykująca ---

// Tworzy i zwraca inteligentny wskaźnik na obiekt z logiką budynku
// na podstawie podanego ID typu.
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
