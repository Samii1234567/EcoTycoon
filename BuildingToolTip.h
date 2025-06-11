#ifndef BUILDINGTOOLTIP_H
#define BUILDINGTOOLTIP_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Building.h" // Potrzebne dla definicji PlacedObject

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY BUILDINGTOOLTIP
//
//  Definiuje klasę `BuildingTooltip`, która odpowiada za wyświetlanie
//  okienka z informacjami (tzw. "dymku" lub "podpowiedzi") po najechaniu
//  myszą na budynek w grze. Dymek zawiera szczegóły takie jak nazwa, poziom,
//  statystyki oraz przycisk do ulepszania.
//
// ===================================================================================

class BuildingTooltip {
public:
    // --- Metody publiczne ---
    BuildingTooltip();

    // Inicjalizuje wygląd dymka, ustawiając domyślne właściwości jego elementów.
    void initialize(sf::Font& font);

    // Wyświetla dymek dla konkretnego budynku, pobierając jego dane.
    void show(const PlacedObject& building, int playerMoney);

    // Ukrywa dymek.
    void hide();

    // Sprawdza, czy dymek jest aktualnie widoczny.
    bool isVisible() const;

    // Przetwarza zdarzenia (głównie kliknięcie myszą w przycisk ulepszenia).
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window);

    // Rysuje wszystkie elementy dymka w podanym oknie.
    void draw(sf::RenderWindow& window);

    // Sprawdza, czy przycisk ulepszenia został kliknięty w ostatniej obsłużonej ramce.
    bool isUpgradeClicked() const;

private:
    // --- Prywatne pola klasy ---

    // Elementy graficzne składające się na dymek.
    sf::RectangleShape m_background;        // Tło dymka.
    sf::Text m_titleText, m_levelText,      // Teksty z nazwą, poziomem,
        m_infoText, m_upgradeButtonText; // informacjami i etykietą przycisku.
    sf::RectangleShape m_upgradeButton;     // Kształt przycisku "Ulepsz".

    // Flagi kontrolujące stan dymka.
    bool m_visible;         // Czy dymek jest aktualnie widoczny.
    bool m_upgradeClicked;  // Czy przycisk ulepszenia został właśnie kliknięty.
};

#endif // BUILDINGTOOLTIP_H
