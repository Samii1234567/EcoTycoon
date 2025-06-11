#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/System/Vector2.hpp>
#include <vector>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY ZE STAŁYMI GRY
//
//  Ten plik jest centralnym miejscem do zarządzania "magicznymi liczbami" i parametrami
//  konfiguracyjnymi gry. Umieszczenie ich tutaj ułatwia balansowanie rozgrywki
//  i utrzymanie spójności, bez potrzeby modyfikowania wielu plików z kodem.
//
// ===================================================================================

namespace GameConstants {

// --- Konfiguracja siatki ---
constexpr float GRID_CELL_SIZE = 100.f; // Rozmiar boku jednej komórki siatki w pikselach
constexpr int GRID_COLS = 12;           // Liczba kolumn na mapie
constexpr int GRID_ROWS = 6;            // Liczba wierszy na mapie

// --- Identyfikatory typów budynków ---
// Używane w całym kodzie do identyfikacji, jaki budynek jest przetwarzany.
constexpr int ENERGY_STORAGE_ID = 0; // Magazyn energii
constexpr int SOLAR_PANEL_ID = 1;    // Panel słoneczny
constexpr int WIND_TURBINE_ID = 2;   // Turbina wiatrowa
constexpr int AIR_FILTER_ID = 3;     // Stacja filtrowania

// --- Rozmiary budynków (w komórkach siatki) ---
const sf::Vector2i ENERGY_STORAGE_SIZE = {2, 2}; // {szerokość, wysokość}
const sf::Vector2i SOLAR_PANEL_SIZE = {1, 1};
const sf::Vector2i WIND_TURBINE_SIZE = {1, 2}; // Turbina zajmuje 1x2 kratki
const sf::Vector2i AIR_FILTER_SIZE = {2, 1};

// --- Właściwości i ulepszenia budynków ---
constexpr int MAX_LEVEL = 3; // Maksymalny poziom, jaki może osiągnąć budynek

// Struktura przechowująca dane dla kolejnych poziomów budynku.
struct BuildingLevelData {
    std::vector<int> upgradeCost; // Koszty ulepszenia na poziom 2 i 3
    std::vector<int> value;       // Wartość (produkcja/pojemność) dla poziomów 1, 2, 3
    std::vector<float> envEffect; // Wpływ na środowisko dla poziomów 1, 2, 3
};

// Dane dla Magazynu Energii
const BuildingLevelData STORAGE_DATA = {
    {750, 1500},               // Koszt ulepszenia
    {100, 250, 500},           // Pojemność na kolejnych poziomach
    {-0.05f, -0.05f, -0.05f}   // Negatywny wpływ na środowisko (stały)
};

// Dane dla Paneli Słonecznych
const BuildingLevelData SOLAR_PANEL_DATA = {
    {300, 600},                // Koszt ulepszenia
    {2, 5, 10},                // Produkcja energii na kolejnych poziomach
    {-0.02f, -0.02f, -0.02f}   // Lekko negatywny wpływ na środowisko
};

// Dane dla Turbin Wiatrowych
const BuildingLevelData WIND_TURBINE_DATA = {
    {750, 1500},               // Koszt ulepszenia
    {5, 12, 25},               // Produkcja energii na kolejnych poziomach
    {0.1f, 0.2f, 0.3f}         // Pozytywny wpływ na środowisko (coraz lepszy)
};

// Dane dla Stacji Filtrowania
const BuildingLevelData AIR_FILTER_DATA = {
    {2000, 4000},              // Koszt ulepszenia
    {10, 15, 20},              // Zużycie energii na kolejnych poziomach
    {0.4f, 0.6f, 0.8f}         // Bardzo pozytywny wpływ na środowisko
};

// --- Ekonomia ---
constexpr int ENERGY_SELL_PRICE = 2; // Cena sprzedaży za 1 jednostkę energii

} // koniec namespace GameConstants

// --- Funkcje pomocnicze ---

// Zwraca rozmiar budynku (w komórkach) na podstawie jego ID.
inline sf::Vector2i getBuildingSize(int typeId) {
    switch (typeId) {
    case GameConstants::ENERGY_STORAGE_ID: return GameConstants::ENERGY_STORAGE_SIZE;
    case GameConstants::SOLAR_PANEL_ID:    return GameConstants::SOLAR_PANEL_SIZE;
    case GameConstants::WIND_TURBINE_ID:   return GameConstants::WIND_TURBINE_SIZE;
    case GameConstants::AIR_FILTER_ID:     return GameConstants::AIR_FILTER_SIZE;
    default:                               return {1, 1};
    }
}

// Zwraca wskaźnik do struktury z danymi dla budynku o podanym ID.
inline const GameConstants::BuildingLevelData* getBuildingData(int typeId) {
    switch (typeId) {
    case GameConstants::SOLAR_PANEL_ID:    return &GameConstants::SOLAR_PANEL_DATA;
    case GameConstants::WIND_TURBINE_ID:   return &GameConstants::WIND_TURBINE_DATA;
    case GameConstants::ENERGY_STORAGE_ID: return &GameConstants::STORAGE_DATA;
    case GameConstants::AIR_FILTER_ID:     return &GameConstants::AIR_FILTER_DATA;
    default:                               return nullptr;
    }
}

#endif // CONSTANTS_H
