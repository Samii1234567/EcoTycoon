#ifndef BUILDING_H
#define BUILDING_H

#include <SFML/Graphics.hpp>

// Forward declaration, aby uniknąć cyklicznej zależności z Game.h.
// Oznacza to "obiecuję, że klasa Game istnieje", pełna definicja
// zostanie dołączona w pliku .cpp.
class Game;

// Klasa bazowa dla wszystkich budynków
class Building {
public:
    virtual ~Building() = default;

    // Metoda aktualizująca logikę budynku w każdej klatce.
    // Jest "czysto wirtualna" (= 0), co oznacza, że każda klasa
    // pochodna MUSI ją zaimplementować.
    virtual void update(float dt, Game& game) = 0;
};

// Magazyn energii - jego efekt jest natychmiastowy przy budowie, więc update() jest puste
class EnergyStorage : public Building {
public:
    void update(float dt, Game& game) override;
};

// Panele słoneczne - na razie bez logiki
class SolarPanel : public Building {
public:
    void update(float dt, Game& game) override;
};

// Turbina wiatrowa - generuje energię i poprawia środowisko
class WindTurbine : public Building {
public:
    void update(float dt, Game& game) override;
};

#endif // BUILDING_H
