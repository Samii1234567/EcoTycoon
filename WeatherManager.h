#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <deque>
#include <random>
#include <functional>
#include "WeatherSystem.h"
#include "Building.h"

class WeatherManager {
public:
    WeatherManager();

    void reset();
    void update(float dt, float environmentHealth, std::vector<PlacedObject>& placedObjects, std::function<void(const std::string&)> showNotificationCallback);

    WeatherType getCurrentWeather() const;
    const std::deque<WeatherType>& getForecast() const;
    float getWeatherMultiplierSolar() const;
    float getWeatherMultiplierWind() const;

    // ##### NOWA FUNKCJA #####
    float getWeatherTimer() const;
    // ########################

private:
    void changeWeather(float environmentHealth);
    WeatherType generateNextWeather(float environmentHealth);

    WeatherType m_currentWeather;
    float m_weatherTimer;
    std::deque<WeatherType> m_forecast;
    static constexpr int FORECAST_LENGTH = 5;

    float m_weatherEffectMultiplierSolar;
    float m_weatherEffectMultiplierWind;

    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_weatherTimeDist;
    std::uniform_real_distribution<float> m_damageDist;
};

#endif // WEATHERMANAGER_H
