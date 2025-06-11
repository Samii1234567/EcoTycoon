#include "WeatherManager.h"
#include "Game.h"
#include <iostream>

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY WEATHERMANAGER
//
//  Zawiera logikę metod zadeklarowanych w WeatherManager.h.
//
// ===================================================================================

// Implementacja konstruktora.
WeatherManager::WeatherManager()
    : m_rng(std::random_device{}()), // Inicjalizacja generatora losowego ziarnem z systemu.
    m_weatherTimeDist(15, 30),       // Pogoda będzie trwać od 15 do 30 sekund.
    m_damageDist(0.0f, 1.0f)       // Rozkład do losowania szansy na uszkodzenie (od 0% do 100%).
{
    reset();
}

// Implementacja resetowania stanu.
void WeatherManager::reset() {
    m_forecast.clear();
    m_weatherEffectMultiplierSolar = 1.0f;
    m_weatherEffectMultiplierWind = 1.0f;
    m_currentWeather = WeatherType::Sunny;
    // Wypełniamy prognozę początkowymi, losowymi zjawiskami.
    for (int i = 0; i < FORECAST_LENGTH + 1; ++i) {
        changeWeather(100.f); // Zakładamy początkowy, idealny stan środowiska.
    }
}

// Implementacja generowania nowej pogody.
WeatherType WeatherManager::generateNextWeather(float environmentHealth) {
    std::vector<WeatherType> possibleWeathers;
    std::vector<double> weights;

    // Wagi prawdopodobieństwa dla różnych zjawisk pogodowych w zależności od stanu środowiska.
    // Im gorszy stan środowiska, tym większa szansa na negatywne zjawiska.
    if (environmentHealth > 75) { // Stan bardzo dobry
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog};
        weights          = { 35, 18, 15, 15, 8, 3, 3, 3 }; // Duża szansa na dobrą pogodę
    } else if (environmentHealth > 40) { // Stan średni
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog };
        weights          = { 23, 18, 17, 10, 17, 5, 5, 5 };
    } else { // Stan zły
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog };
        weights          = { 7, 22, 18, 13, 10, 10, 10, 10 }; // Mała szansa na dobrą pogodę
    }
    // Użycie `discrete_distribution` do przeprowadzenia losowania z wagami.
    std::discrete_distribution<int> weatherDist(weights.begin(), weights.end());
    return possibleWeathers[weatherDist(m_rng)];
}

// Implementacja zmiany pogody.
void WeatherManager::changeWeather(float environmentHealth) {
    // Jeśli prognoza jest pusta, ustawiamy domyślną pogodę.
    if (m_forecast.empty()) {
        m_currentWeather = WeatherType::Sunny;
    } else {
        // Bierzemy następną pogodę z początku kolejki.
        m_currentWeather = m_forecast.front();
        m_forecast.pop_front();
    }
    // Dodajemy nową, wylosowaną pogodę na koniec kolejki.
    m_forecast.push_back(generateNextWeather(environmentHealth));
    // Losujemy czas trwania nowej pogody.
    m_weatherTimer = m_weatherTimeDist(m_rng);
    // Ustawiamy odpowiednie mnożniki produkcji energii w zależności od nowej pogody.
    switch (m_currentWeather) {
    case WeatherType::Sunny:     m_weatherEffectMultiplierSolar = 1.25f; m_weatherEffectMultiplierWind = 0.9f; break;
    case WeatherType::Windy:     m_weatherEffectMultiplierSolar = 1.0f;  m_weatherEffectMultiplierWind = 1.3f; break;
    case WeatherType::Cloudy:    m_weatherEffectMultiplierSolar = 0.7f;  m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Rain:      m_weatherEffectMultiplierSolar = 0.5f;  m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Storm:     m_weatherEffectMultiplierSolar = 0.1f;  m_weatherEffectMultiplierWind = 1.6f; break;
    case WeatherType::AcidRain:  m_weatherEffectMultiplierSolar = 0.6f;  m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Heatwave:  m_weatherEffectMultiplierSolar = 0.5f;  m_weatherEffectMultiplierWind = 0.5f; break;
    case WeatherType::Smog:      m_weatherEffectMultiplierSolar = 0.05f; m_weatherEffectMultiplierWind = 0.8f; break;
    }
}

// Implementacja aktualizacji stanu pogody.
void WeatherManager::update(float dt, float environmentHealth, std::vector<PlacedObject>& placedObjects, std::function<void(const std::string&)> showNotificationCallback) {
    // Odliczanie czasu do zmiany pogody.
    m_weatherTimer -= dt;
    if (m_weatherTimer <= 0.f) {
        changeWeather(environmentHealth);
    }

    // Obsługa efektów specjalnych, które mogą wystąpić w każdej klatce.
    switch (m_currentWeather) {
    case WeatherType::AcidRain:
        // Szansa 10% na sekundę na uszkodzenie losowego budynku.
        if (m_damageDist(m_rng) < 0.1f * dt && !placedObjects.empty()) {
            int objIdx = std::uniform_int_distribution<int>(0, (int)placedObjects.size() - 1)(m_rng);
            if (!placedObjects[objIdx].isDamaged) {
                placedObjects[objIdx].isDamaged = true;
                placedObjects[objIdx].sprite.setColor(sf::Color(255, 100, 100));
                showNotificationCallback("Kwasny deszcz uszkodzil budynek!"); // Wywołanie powiadomienia
            }
        }
        break;
    case WeatherType::Storm:
        // Szansa 20% na sekundę na uszkodzenie losowego budynku.
        if (m_damageDist(m_rng) < 0.2f * dt && !placedObjects.empty()) {
            int objIdx = std::uniform_int_distribution<int>(0, (int)placedObjects.size() - 1)(m_rng);
            if (!placedObjects[objIdx].isDamaged) {
                placedObjects[objIdx].isDamaged = true;
                placedObjects[objIdx].sprite.setColor(sf::Color(255, 100, 100));
                showNotificationCallback("Burza uszkodzila budynek!");
            }
        }
        break;
    default:
        // Inne typy pogody nie mają efektów per-klatka.
        break;
    }
}

// Implementacje prostych getterów.
WeatherType WeatherManager::getCurrentWeather() const { return m_currentWeather; }
const std::deque<WeatherType>& WeatherManager::getForecast() const { return m_forecast; }
float WeatherManager::getWeatherMultiplierSolar() const { return m_weatherEffectMultiplierSolar; }
float WeatherManager::getWeatherMultiplierWind() const { return m_weatherEffectMultiplierWind; }
float WeatherManager::getWeatherTimer() const { return m_weatherTimer; }
