#include "WeatherManager.h"
#include "Game.h"
#include <iostream>

// ... (reszta pliku bez zmian) ...
WeatherManager::WeatherManager()
    : m_rng(std::random_device{}()),
    m_weatherTimeDist(15, 30),
    m_damageDist(0.0f, 1.0f) {
    reset();
}

void WeatherManager::reset() {
    m_forecast.clear();
    m_weatherEffectMultiplierSolar = 1.0f;
    m_weatherEffectMultiplierWind = 1.0f;
    m_currentWeather = WeatherType::Sunny;
    for (int i = 0; i < FORECAST_LENGTH + 1; ++i) {
        changeWeather(100.f);
    }
}

WeatherType WeatherManager::generateNextWeather(float environmentHealth) {
    std::vector<WeatherType> possibleWeathers;
    std::vector<double> weights;
    if (environmentHealth > 75) {
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog};
        weights          = /*{ 35, 18, 15, 15, 8, 3, 3, 3 };*/ {0,0,0,0,1,0,0,0};
    } else if (environmentHealth > 40) {
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog };
        weights          = { 23, 18, 17, 10, 17, 5, 5, 5 };
    } else {
        possibleWeathers = { WeatherType::Sunny, WeatherType::Windy, WeatherType::Cloudy, WeatherType::Rain, WeatherType::Storm, WeatherType::AcidRain, WeatherType::Heatwave, WeatherType::Smog };
        weights          = { 7, 22, 18, 13, 10, 10, 10, 10 };
    }
    std::discrete_distribution<int> weatherDist(weights.begin(), weights.end());
    return possibleWeathers[weatherDist(m_rng)];
}

void WeatherManager::changeWeather(float environmentHealth) {
    if (m_forecast.empty()) {
        m_currentWeather = WeatherType::Sunny;
    } else {
        m_currentWeather = m_forecast.front();
        m_forecast.pop_front();
    }
    m_forecast.push_back(generateNextWeather(environmentHealth));
    m_weatherTimer = m_weatherTimeDist(m_rng);
    switch (m_currentWeather) {
    case WeatherType::Sunny:     m_weatherEffectMultiplierSolar = 1.25f; m_weatherEffectMultiplierWind = 0.9f; break;
    case WeatherType::Windy:     m_weatherEffectMultiplierSolar = 1.0f; m_weatherEffectMultiplierWind = 1.3f; break;
    case WeatherType::Cloudy:    m_weatherEffectMultiplierSolar = 0.7f; m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Rain:      m_weatherEffectMultiplierSolar = 0.5f; m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Storm:     m_weatherEffectMultiplierSolar = 0.1f; m_weatherEffectMultiplierWind = 1.6f; break;
    case WeatherType::AcidRain:  m_weatherEffectMultiplierSolar = 0.6f; m_weatherEffectMultiplierWind = 1.0f; break;
    case WeatherType::Heatwave:  m_weatherEffectMultiplierSolar = 0.5f; m_weatherEffectMultiplierWind = 0.5f; break;
    case WeatherType::Smog:      m_weatherEffectMultiplierSolar = 0.05f; m_weatherEffectMultiplierWind = 0.8f; break;
    }
    std::cout << "ZMIANA POGODY (Zdrowie: " << (int)environmentHealth << "%): " << getWeatherName(m_currentWeather) << " (na " << m_weatherTimer << "s)\n";
}

void WeatherManager::update(float dt, float environmentHealth, std::vector<PlacedObject>& placedObjects, std::function<void(const std::string&)> showNotificationCallback) {
    m_weatherTimer -= dt;
    if (m_weatherTimer <= 0.f) {
        changeWeather(environmentHealth);
    }
    switch (m_currentWeather) {
    case WeatherType::AcidRain:
        if (m_damageDist(m_rng) < 0.1f * dt && !placedObjects.empty()) {
            int objIdx = std::uniform_int_distribution<int>(0, placedObjects.size() - 1)(m_rng);
            if (!placedObjects[objIdx].isDamaged) {
                placedObjects[objIdx].isDamaged = true;
                placedObjects[objIdx].sprite.setColor(sf::Color(255, 100, 100));
                showNotificationCallback("Kwasny deszcz uszkodzil budynek!");
            }
        }
        break;
    case WeatherType::Storm:
        if (m_damageDist(m_rng) < 0.2f * dt && !placedObjects.empty()) {
            int objIdx = std::uniform_int_distribution<int>(0, placedObjects.size() - 1)(m_rng);
            if (!placedObjects[objIdx].isDamaged) {
                placedObjects[objIdx].isDamaged = true;
                placedObjects[objIdx].sprite.setColor(sf::Color(255, 100, 100));
                showNotificationCallback("Burza uszkodzila budynek!");
            }
        }
        break;
    default:
        break;
    }
}
WeatherType WeatherManager::getCurrentWeather() const { return m_currentWeather; }
const std::deque<WeatherType>& WeatherManager::getForecast() const { return m_forecast; }
float WeatherManager::getWeatherMultiplierSolar() const { return m_weatherEffectMultiplierSolar; }
float WeatherManager::getWeatherMultiplierWind() const { return m_weatherEffectMultiplierWind; }

// ##### NOWA IMPLEMENTACJA #####
float WeatherManager::getWeatherTimer() const {
    return m_weatherTimer;
}
// ##############################
