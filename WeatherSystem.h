#ifndef WEATHERSYSTEM_H
#define WEATHERSYSTEM_H

#include <string>
#include <SFML/Graphics.hpp>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA SYSTEMU POGODY
//
//  Ten plik nie zawiera logiki, a jedynie dane i proste funkcje pomocnicze
//  związane z pogodą. Definiuje typy pogody oraz dostarcza funkcje do
//  pobierania ich nazw, opisów i ścieżek do ikon.
//  Dzięki temu wszystkie teksty i ścieżki są w jednym miejscu.
//
// ===================================================================================

// Typ wyliczeniowy reprezentujący wszystkie możliwe zjawiska pogodowe w grze.
enum class WeatherType {
    Sunny,      // Słonecznie
    Windy,      // Wietrznie
    Cloudy,     // Pochmurno
    Rain,       // Deszcz
    Storm,      // Burza
    AcidRain,   // Kwaśny Deszcz
    Heatwave,   // Fala Upałów
    Smog        // Smog
};

// Zwraca polską nazwę dla danego typu pogody.
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

// Zwraca ścieżkę do pliku z ikoną dla danego typu pogody.
inline std::string getWeatherIconFilename(WeatherType type) {
    switch (type) {
    case WeatherType::Sunny:     return "images/sunny_weather.png";
    case WeatherType::Windy:     return "images/windy_weather.png";
    case WeatherType::Cloudy:    return "images/cloudy_weather.png";
    case WeatherType::Rain:      return "images/rain_weather.png";
    case WeatherType::Storm:     return "images/storm_weather.png";
    case WeatherType::AcidRain:  return "images/acid_rain_weather.png";
    case WeatherType::Heatwave:  return "images/heatwave_weather.png";
    case WeatherType::Smog:      return "images/smog_weather.png";
    default:                     return "";
    }
}

// Zwraca opis efektów w grze dla danego typu pogody.
inline std::string getWeatherDescription(WeatherType type) {
    switch (type) {
    case WeatherType::Sunny:     return "Efekty:\n- Panele sloneczne: +25%\n- Turbiny wiatrowe: -10%";
    case WeatherType::Windy:     return "Efekty:\n- Turbiny wiatrowe: +30%";
    case WeatherType::Cloudy:    return "Efekty:\n- Panele sloneczne: -30%";
    case WeatherType::Rain:      return "Efekty:\n- Panele sloneczne: -50%\n- Regeneracja srodowiska";
    case WeatherType::Storm:     return "Efekty:\n- Turbiny wiatrowe: +60%\n- Panele sloneczne: -90%\n- Ryzyko uszkodzenia budynkow";
    case WeatherType::AcidRain:  return "KRYZYS:\n- Degradacja srodowiska\n- Ryzyko uszkodzenia budynkow";
    case WeatherType::Heatwave:  return "KRYZYS:\n- Panele sloneczne: -50%\n- Turbiny wiatrowe: -50%\n- Wysokie ryzyko pozaru";
    case WeatherType::Smog:      return "KRYZYS:\n- Panele sloneczne: -95%\n- Szybka degradacja srodowiska";
    default:                     return "Brak opisu.";
    }
}

#endif // WEATHERSYSTEM_H
