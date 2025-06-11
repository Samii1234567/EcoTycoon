#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <deque>
#include <random>
#include <functional>
#include "WeatherSystem.h"
#include "Building.h"

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY WEATHERMANAGER
//
//  Definiuje klasę `WeatherManager`, która jest odpowiedzialna za symulowanie
//  dynamicznych warunków pogodowych w grze. Zarządza zmianami pogody,
//  przechowuje prognozę i aplikuje efekty pogodowe (np. bonusy do produkcji,
//  ryzyko uszkodzeń) do świata gry.
//
// ===================================================================================

class WeatherManager {
public:
    // --- Metody publiczne ---
    WeatherManager();

    // Resetuje stan menedżera do wartości początkowych.
    void reset();

    // Aktualizuje stan pogody. Odpowiada za odliczanie czasu do zmiany pogody
    // oraz za efekty specjalne, takie jak uszkodzenia budynków.
    void update(float dt, float environmentHealth, std::vector<PlacedObject>& placedObjects, std::function<void(const std::string&)> showNotificationCallback);

    // --- Gettery ---
    // Zwracają informacje o aktualnym stanie pogody.
    WeatherType getCurrentWeather() const;
    const std::deque<WeatherType>& getForecast() const;
    float getWeatherMultiplierSolar() const;
    float getWeatherMultiplierWind() const;
    float getWeatherTimer() const;

private:
    // --- Metody i pola prywatne ---

    // Prywatna metoda odpowiedzialna za logikę zmiany pogody na następną z prognozy.
    void changeWeather(float environmentHealth);
    // Prywatna metoda generująca nową, losową pogodę do prognozy na podstawie stanu środowiska.
    WeatherType generateNextWeather(float environmentHealth);

    WeatherType m_currentWeather;  // Aktualnie panująca pogoda.
    float m_weatherTimer;          // Czas pozostały do następnej zmiany pogody.
    std::deque<WeatherType> m_forecast; // Kolejka z prognozowanymi zjawiskami pogodowymi.
    static constexpr int FORECAST_LENGTH = 5; // Długość prognozy.

    // Mnożniki wpływające na produkcję energii.
    float m_weatherEffectMultiplierSolar;
    float m_weatherEffectMultiplierWind;

    // Narzędzia do generowania liczb losowych.
    std::mt19937 m_rng; // Generator liczb pseudolosowych (Mersenne Twister).
    std::uniform_int_distribution<int> m_weatherTimeDist; // Rozkład do losowania czasu trwania pogody.
    std::uniform_real_distribution<float> m_damageDist;   // Rozkład do losowania szansy na uszkodzenie.
};

#endif // WEATHERMANAGER_H
