#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <string>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY HUD
//
//  Definiuje klasę `HUD` (Heads-Up Display), która odpowiada za rysowanie
//  głównego interfejsu użytkownika na dole ekranu podczas rozgrywki.
//  Wyświetla kluczowe informacje, takie jak stan konta, energia, środowisko i czas gry.
//
// ===================================================================================

class HUD {
public:
    // --- Metody publiczne ---

    // Konstruktor, który przyjmuje referencję do wczytanej czcionki.
    HUD(sf::Font& font);

    // Aktualizuje wszystkie teksty wyświetlane w HUD na podstawie bieżących danych z gry.
    void update(int money, float moneyRate, int energy, int maxEnergy, float netEnergyRate, float envHealth, float netEnvRate, float totalTime);

    // Rysuje wszystkie elementy HUD w podanym oknie.
    void draw(sf::RenderWindow& window);

    // Zwraca globalne granice (prostokąt) przycisku do sprzedaży energii,
    // co pozwala na wykrywanie kliknięć w innych częściach kodu.
    const sf::FloatRect getEnergySellButtonBounds() const;

    // Uruchamia krótką animację podświetlenia stanu konta, np. po zarobieniu pieniędzy.
    void flashMoney();

private:
    // --- Prywatne pola klasy ---

    // Referencja do czcionki używanej przez wszystkie teksty w HUD.
    sf::Font& m_font;

    // Obiekty tekstowe dla etykiet (np. "Stan konta").
    sf::Text m_labelMoney, m_labelEnergy, m_labelEnv, m_labelTime;

    // Obiekty tekstowe dla wartości (np. "1000$").
    sf::Text m_valueMoney, m_valueEnergy, m_valueEnv;

    // Obiekt tekstowy dla wskaźnika przychodu/straty pieniędzy (np. "+10$/s").
    sf::Text m_valueMoneyRate;

    // Elementy graficzne dla przycisku sprzedaży energii.
    sf::CircleShape m_energySellButtonBg; // Tło przycisku
    sf::Text m_energySellIcon;           // Ikona dolara "$" na przycisku

    // Zmienne do obsługi animacji migania stanu konta.
    bool m_isFlashingMoney = false; // Flaga, czy animacja jest aktywna.
    sf::Clock m_moneyFlashClock;    // Zegar do mierzenia czasu trwania animacji.
};

#endif // HUD_H
