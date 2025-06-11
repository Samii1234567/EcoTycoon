#ifndef WEATHERSYSTEM_H
#define WEATHERSYSTEM_H

#include <string>
#include <SFML/Graphics.hpp>

// Enum przechowujący wszystkie możliwe stany pogody
enum class WeatherType {
    Sunny,
    Windy,
    Cloudy,
    Rain,
    Storm,
    AcidRain,
    Heatwave,
    Smog
};

// Funkcja pomocnicza do uzyskania nazwy pogody jako string
inline std::string getWeatherName(WeatherType type) {
    switch (type) {
    case WeatherType::Sunny:     return "Slonecznie";
    case WeatherType::Windy:     return "Wietrznie";
    case WeatherType::Cloudy:    return "Pochmurno";
    case WeatherType::Rain:      return "Deszcz";
    case WeatherType::Storm:     return "Burza";
    case WeatherType::AcidRain:  return "Kwasny Deszcz";
    case WeatherType::Heatwave:  return "Fala Upalow";
    case WeatherType::Smog:      return "Smog";
    default:                     return "Brak danych";
    }
}

// Funkcja zwracająca kolor dla ikony-placeholdera
inline sf::Color getWeatherColor(WeatherType type) {
    switch (type) {
    case WeatherType::Sunny:     return sf::Color::Yellow;
    case WeatherType::Windy:     return sf::Color(200, 200, 200); // Jasnoszary
    case WeatherType::Cloudy:    return sf::Color(128, 128, 128); // Ciemnoszary
    case WeatherType::Rain:      return sf::Color::Blue;
    case WeatherType::Storm:     return sf::Color(70, 70, 130);   // Ciemnoniebieski
    case WeatherType::AcidRain:  return sf::Color(100, 200, 100);   // Toksyczny zielony
    case WeatherType::Heatwave:  return sf::Color::Red;
    case WeatherType::Smog:      return sf::Color(100, 80, 50);   // Brązowy
    default:                     return sf::Color::Black;
    }
}

// Funkcja zwracająca opis efektów pogodowych
inline std::string getWeatherDescription(WeatherType type) {
    switch (type) {
    case WeatherType::Sunny:     return "Efekty:\n- Panele sloneczne: +25%\n- Turbiny wiatrowe: -10%";
    case WeatherType::Windy:     return "Efekty:\n- Turbiny wiatrowe: +30%";
    case WeatherType::Cloudy:    return "Efekty:\n- Panele sloneczne: -30%";
    case WeatherType::Rain:      return "Efekty:\n- Panele sloneczne: -50%\n- Regeneracja srodowiska";
    case WeatherType::Storm:     return "Efekty:\n- Turbiny wiatrowe: +60%\n- Panele sloneczne: -90%\n- Ryzyko uszkodzenia budynkow";
    case WeatherType::AcidRain:  return "KRYZYS:\n- Degradacja srodowiska\n- Ryzyko uszkodzenia budynkow";
    case WeatherType::Heatwave:  return "KRYZYS:\n- Wszystkie zrodla energii: -50%";
    case WeatherType::Smog:      return "KRYZYS:\n- Panele sloneczne: -95%\n- Szybka degradacja srodowiska";
    default:                     return "Brak danych.";
    }
}


#endif // WEATHERSYSTEM_H
