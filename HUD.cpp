#include "HUD.h"
#include <iomanip>   // Potrzebne do std::setprecision, std::setw
#include <sstream>   // Potrzebne do std::ostringstream

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY HUD
//
//  Zawiera logikę metod zadeklarowanych w HUD.h.
//
// ===================================================================================


// --- Stałe definiujące pozycje elementów w HUD ---
// Użycie stałych ułatwia zachowanie spójności i wprowadzanie zmian w układzie interfejsu.

const float MONEY_SECTION_CENTER_X = 138.f;  // Środek sekcji "Pieniądze"
const float ENERGY_SECTION_CENTER_X = 422.f; // Środek sekcji "Energia"
const float ENV_SECTION_CENTER_X = 742.f;    // Środek sekcji "Środowisko"
const float TIME_SECTION_CENTER_X = 1053.f;  // Środek sekcji "Czas"
const float LABEL_Y = 730.f; // Pozycja Y dla etykiet (np. "Stan konta")
const float VALUE_Y = 758.f; // Pozycja Y dla głównych wartości (np. "1000$")
const float TIME_Y = 742.f;  // Specjalna pozycja Y dla czasu gry

// Implementacja konstruktora.
HUD::HUD(sf::Font& font) : m_font(font) {
    // Ustawienie wspólnego koloru dla większości tekstów.
    sf::Color brown(101, 67, 33);

    // --- Inicjalizacja sekcji Pieniędzy ---
    m_labelMoney.setFont(m_font); m_labelMoney.setString("Stan konta"); m_labelMoney.setCharacterSize(24); m_labelMoney.setFillColor(brown);
    // Ustawienie punktu odniesienia (origin) na środek napisu w osi X, co ułatwia centrowanie.
    m_labelMoney.setOrigin(m_labelMoney.getLocalBounds().left + m_labelMoney.getLocalBounds().width / 2.f, 0.f);
    m_labelMoney.setPosition(MONEY_SECTION_CENTER_X, LABEL_Y);

    m_valueMoney.setFont(m_font); m_valueMoney.setCharacterSize(24); m_valueMoney.setFillColor(brown);
    m_valueMoneyRate.setFont(m_font); m_valueMoneyRate.setCharacterSize(16); m_valueMoneyRate.setFillColor(sf::Color(38, 128, 0)); // Zielony dla przychodu

    // --- Inicjalizacja sekcji Energii ---
    m_labelEnergy.setFont(m_font); m_labelEnergy.setString("Energia"); m_labelEnergy.setCharacterSize(24); m_labelEnergy.setFillColor(brown);
    m_labelEnergy.setOrigin(m_labelEnergy.getLocalBounds().left + m_labelEnergy.getLocalBounds().width / 2.f, 0.f);
    m_labelEnergy.setPosition(ENERGY_SECTION_CENTER_X, LABEL_Y);

    m_valueEnergy.setFont(m_font); m_valueEnergy.setCharacterSize(24); m_valueEnergy.setFillColor(brown);

    // --- Inicjalizacja sekcji Środowiska ---
    m_labelEnv.setFont(m_font); m_labelEnv.setString("Stan srodowiska"); m_labelEnv.setCharacterSize(24); m_labelEnv.setFillColor(brown);
    m_labelEnv.setOrigin(m_labelEnv.getLocalBounds().left + m_labelEnv.getLocalBounds().width / 2.f, 0.f);
    m_labelEnv.setPosition(ENV_SECTION_CENTER_X, LABEL_Y);

    m_valueEnv.setFont(m_font); m_valueEnv.setCharacterSize(24); m_valueEnv.setFillColor(brown);

    // --- Inicjalizacja sekcji Czasu ---
    m_labelTime.setFont(m_font); m_labelTime.setCharacterSize(30); m_labelTime.setFillColor(brown);

    // --- Inicjalizacja przycisku sprzedaży energii ---
    m_energySellButtonBg.setRadius(15.f);
    m_energySellButtonBg.setFillColor(sf::Color(90, 160, 90));
    m_energySellIcon.setFont(m_font);
    m_energySellIcon.setString("$");
    m_energySellIcon.setCharacterSize(24);
    m_energySellIcon.setFillColor(sf::Color(255, 165, 0)); // Złoty/pomarańczowy kolor
    m_energySellIcon.setStyle(sf::Text::Bold);

    // Obliczenie pozycji przycisku względem etykiety "Energia".
    float buttonCenterX = ENERGY_SECTION_CENTER_X + (m_labelEnergy.getLocalBounds().width / 2.f) + 30.f;
    float buttonCenterY = LABEL_Y + m_labelEnergy.getLocalBounds().height / 2.f + 5.f;

    m_energySellButtonBg.setOrigin(15.f, 15.f); // Origin w środku koła
    m_energySellButtonBg.setPosition(buttonCenterX, buttonCenterY);

    sf::FloatRect textBounds = m_energySellIcon.getLocalBounds();
    m_energySellIcon.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_energySellIcon.setPosition(buttonCenterX, buttonCenterY);
}

// Implementacja aktualizacji danych w HUD.
void HUD::update(int money, float moneyRate, int energy, int maxEnergy, float netEnergyRate, float envHealth, float netEnvRate, float totalTime) {
    // Używamy `std::ostringstream` do łatwego formatowania tekstu z liczbami.
    std::ostringstream oss;

    // Aktualizacja tekstu pieniędzy.
    m_valueMoney.setString(std::to_string(money) + "$");
    m_valueMoney.setOrigin(m_valueMoney.getLocalBounds().left + m_valueMoney.getLocalBounds().width / 2.f, 0.f);
    m_valueMoney.setPosition(MONEY_SECTION_CENTER_X, VALUE_Y);

    // Aktualizacja wskaźnika przychodu (wyświetlany tylko, gdy jest dodatni).
    oss.str("");
    oss.clear();
    if (moneyRate > 0) {
        oss << std::fixed << std::setprecision(0) << "(+" << moneyRate << "$/s)";
    } else {
        oss << ""; // Pusty string, jeśli nie ma przychodu
    }
    m_valueMoneyRate.setString(oss.str());
    m_valueMoneyRate.setOrigin(m_valueMoneyRate.getLocalBounds().left + m_valueMoneyRate.getLocalBounds().width / 2.f, 0.f);
    m_valueMoneyRate.setPosition(MONEY_SECTION_CENTER_X, VALUE_Y + 25.f);

    // Aktualizacja tekstu energii.
    oss.str("");
    oss.clear();
    std::string energySign = (netEnergyRate >= 0) ? "+" : ""; // Dodajemy "+" dla dodatnich wartości.
    oss << std::fixed << std::setprecision(1) << netEnergyRate; // Precyzja do jednego miejsca po przecinku.
    m_valueEnergy.setString(std::to_string(energy) + "/" + std::to_string(maxEnergy) + " (" + energySign + oss.str() + "/s)");
    m_valueEnergy.setOrigin(m_valueEnergy.getLocalBounds().left + m_valueEnergy.getLocalBounds().width / 2.f, 0.f);
    m_valueEnergy.setPosition(ENERGY_SECTION_CENTER_X, VALUE_Y);

    // Aktualizacja tekstu środowiska.
    oss.str("");
    oss.clear();
    std::string envSign = (netEnvRate >= 0) ? "+" : "";
    oss << std::fixed << std::setprecision(2) << netEnvRate; // Precyzja do dwóch miejsc po przecinku.
    m_valueEnv.setString(std::to_string(static_cast<int>(envHealth)) + "% (" + envSign + oss.str() + "%/s)");
    m_valueEnv.setOrigin(m_valueEnv.getLocalBounds().left + m_valueEnv.getLocalBounds().width / 2.f, 0.f);
    m_valueEnv.setPosition(ENV_SECTION_CENTER_X, VALUE_Y);

    // Aktualizacja czasu gry (format MM:SS).
    int m = static_cast<int>(totalTime) / 60; // Minuty
    int s = static_cast<int>(totalTime) % 60; // Sekundy
    oss.str("");
    oss.clear();
    oss << "Czas gry: " << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
    m_labelTime.setString(oss.str());
    m_labelTime.setOrigin(m_labelTime.getLocalBounds().left + m_labelTime.getLocalBounds().width / 2.f, 0.f);
    m_labelTime.setPosition(TIME_SECTION_CENTER_X, TIME_Y);
}

// Implementacja rysowania HUD.
void HUD::draw(sf::RenderWindow& window) {
    // Sprawdzamy, czy zakończyć animację migania pieniędzy.
    if (m_isFlashingMoney && m_moneyFlashClock.getElapsedTime().asSeconds() > 0.5f) {
        m_isFlashingMoney = false;
    }

    // Rysowanie wszystkich elementów.
    window.draw(m_labelMoney);

    // Jeśli flaga migania jest aktywna, tymczasowo zmieniamy kolor tekstu pieniędzy na zielony.
    sf::Color originalColor = m_valueMoney.getFillColor();
    if (m_isFlashingMoney) {
        m_valueMoney.setFillColor(sf::Color::Green);
    }
    window.draw(m_valueMoney);
    m_valueMoney.setFillColor(originalColor); // Przywracamy oryginalny kolor.

    window.draw(m_valueMoneyRate);
    window.draw(m_labelEnergy); window.draw(m_valueEnergy);
    window.draw(m_labelEnv); window.draw(m_valueEnv);
    window.draw(m_labelTime);
    window.draw(m_energySellButtonBg);
    window.draw(m_energySellIcon);
}

// Implementacja gettera dla granic przycisku.
const sf::FloatRect HUD::getEnergySellButtonBounds() const {
    return m_energySellButtonBg.getGlobalBounds();
}

// Implementacja uruchamiania animacji migania.
void HUD::flashMoney() {
    m_isFlashingMoney = true;
    m_moneyFlashClock.restart(); // Resetujemy zegar animacji.
}
