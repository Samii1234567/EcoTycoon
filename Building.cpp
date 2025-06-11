#include "Building.h"
#include "Game.h"
#include <cmath>

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLAS BUDYNKÓW
//
//  Zawiera implementacje metod `update` dla poszczególnych typów budynków.
//
//  UWAGA: Większość kluczowej logiki (produkcja energii, wpływ na środowisko)
//  została przeniesiona do klasy `Game` w celu centralizacji obliczeń.
//  Metody `update` w budynkach odpowiadają głównie za aspekty wizualne,
//  takie jak animacje, które są specyficzne dla danego typu.
//
// ===================================================================================

// Metoda `update` dla magazynu energii. Obecnie pusta.
void EnergyStorage::update(float dt, Game& game, PlacedObject& self) {
    // Logika wpływu na środowisko została przeniesiona do Game::update
}

// Metoda `update` dla paneli słonecznych. Obecnie pusta.
void SolarPanel::update(float dt, Game& game, PlacedObject& self) {
    // Ta logika nie jest już potrzebna, ponieważ produkcja jest liczona globalnie w Game::update
}

// Metoda `update` dla turbiny wiatrowej. Obecnie pusta.
void WindTurbine::update(float dt, Game& game, PlacedObject& self) {
    // Logika produkcji przeniesiona do Game::update
}

// Metoda `update` dla stacji filtrowania. Obecnie pusta.
void AirFilter::update(float dt, Game& game, PlacedObject& self) {
    // Logika zużycia energii przeniesiona do Game::update
}
