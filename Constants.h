#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace GameConstants {
// Identyfikatory typów budynków
constexpr int ENERGY_STORAGE_ID = 0;
constexpr int SOLAR_PANEL_ID = 1;
constexpr int WIND_TURBINE_ID = 2;

// Właściwości budynków
// ZMIANA: Magazyn daje teraz 100 jednostek pojemności
constexpr int ENERGY_STORAGE_CAPACITY_INCREASE = 100;
// ZMIANA: Panele słoneczne produkują 2 jednostki energii na sekundę
constexpr int SOLAR_PANEL_ENERGY_PER_SEC = 2;
constexpr int WIND_TURBINE_ENERGY_PER_SEC = 5;
constexpr float WIND_TURBINE_ENV_REGEN_PER_SEC = 0.1f;

// ZMIANA: Cena sprzedaży energii
constexpr int ENERGY_SELL_PRICE = 1;
}

#endif // CONSTANTS_H
