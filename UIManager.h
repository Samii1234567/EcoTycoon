#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "GameState.h"
#include "Slider.h"
#include "WeatherSystem.h"
#include "Contract.h"

class Game;
class WeatherManager;

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY UIMANAGER
//
//  Definiuje klasę `UIManager`, która jest centralnym menedżerem dla
//  interfejsu użytkownika wyświetlanego "na wierzchu" głównej rozgrywki.
//  Zarządza wszystkimi panelami, menu i oknami dialogowymi, które mogą
//  pojawić się podczas gry, takimi jak menu opcji, kontrakty, sprzedaż energii itp.
//
// ===================================================================================


// Struktura pomocnicza dla panelu informacyjnego o pogodzie.
struct WeatherInfoEntry {
    sf::Text name;
    sf::Text description;
};

class UIManager {
public:
    // --- Metody publiczne ---

    // Konstruktor, który przyjmuje referencje do czcionki i głównego obiektu gry.
    UIManager(sf::Font& font, Game& game);

    // Przetwarza zdarzenia dla wszystkich paneli UI, w zależności od aktualnego stanu gry.
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState);

    // Aktualizuje elementy UI, które muszą odświeżać swoje dane w każdej klatce (np. zegary, liczniki).
    void update(const WeatherManager& weatherManager);

    // Rysuje odpowiednie panele UI w zależności od aktualnego stanu gry.
    void draw(sf::RenderWindow& window, GameState currentState);

    // Wyświetla na ekranie krótkie powiadomienie tekstowe (np. o braku środków).
    void showNotification(const std::string& message);

    // Synchronizuje wartości suwaków w menu opcji z aktualnymi ustawieniami głośności w grze.
    void syncSliderValues();

private:
    // --- Prywatne metody pomocnicze ---
    // Każda z tych metod odpowiada za rysowanie konkretnego panelu/menu.

    void loadWeatherTextures();
    void drawWeatherUI(sf::RenderWindow& window);
    void drawEnergyMenu(sf::RenderWindow& window);
    void drawOptionsMenu(sf::RenderWindow& window);
    void drawRepairPanel(sf::RenderWindow& window);
    void drawContractsMenu(sf::RenderWindow& window);
    void drawWeatherInfoPanel(sf::RenderWindow& window);
    void drawContractList(sf::RenderWindow& window);
    void drawActiveContractList(sf::RenderWindow& window);
    void drawContractDetail(sf::RenderWindow& window);

    // --- Prywatne pola klasy ---

    // Główne referencje
    Game& m_game;
    sf::Font& m_font;

    // --- Elementy UI pogody ---
    bool m_weatherBoardExpanded;
    sf::RectangleShape m_weatherBoardBG, m_forecastBoardBG, m_weatherPanel, m_infoButtonBG;
    sf::Text m_weatherStatusText, m_forecastLabelText, m_forecastInfoText, m_weatherPanelTitle;
    sf::Text m_weatherPanelCurrentLabel, m_weatherPanelCurrentDesc, m_infoButtonText;
    sf::Sprite m_weatherIconSprite, m_forecastIconSprite;
    std::map<WeatherType, sf::Texture> m_weatherTextures;
    sf::RectangleShape m_forecastInfoButtonBG;
    sf::Text m_forecastInfoButtonText;
    bool m_showingForecastDetails;

    // --- Elementy UI menu kontraktów ---
    bool m_showingActiveContracts;
    int m_viewedContractId;
    sf::RectangleShape m_contractListBackground;
    sf::Text m_contractListTitle;
    sf::Text m_noContractsText;
    sf::RectangleShape m_contractListCloseButton;
    sf::Text m_contractListCloseText;
    sf::RectangleShape m_pendingTabButton, m_activeTabButton;
    sf::Text m_pendingTabText, m_activeTabText;
    sf::Texture m_scrollTexture;
    sf::Sprite m_scrollSprite;
    sf::Text m_contractCityText, m_contractDemandText, m_contractPaymentText;
    sf::RectangleShape m_acceptButton, m_rejectButton;
    sf::Text m_acceptText, m_rejectText;
    sf::Text m_scrollCloseButton;

    // --- Wspólne panele i elementy ---
    sf::RectangleShape m_energyPanel, m_sellButton, m_closeButton;
    sf::Text m_energyMenuTitle, m_energyInfoText, m_energyValueText, m_sellButtonText, m_closeButtonText;
    sf::RectangleShape m_optionsPanel, m_applyButton, m_cancelButton;
    sf::Text m_optionsMenuTitle, m_musicLabel, m_sfxLabel, m_applyButtonText, m_cancelButtonText;
    std::unique_ptr<Slider> m_musicSlider;
    std::unique_ptr<Slider> m_sfxSlider;
    sf::Text m_notificationText;
    sf::Clock m_notificationClock;
    bool m_showNotification;
    sf::RectangleShape m_repairPanel;
    sf::Text m_repairTitle, m_repairInfoText;
    sf::RectangleShape m_repairConfirmButton, m_repairCancelButton;
    sf::Text m_repairConfirmText, m_repairCancelText;
    sf::RectangleShape m_panelOverlay; // Półprzezroczyste tło przyciemniające grę

    // --- Panel informacyjny o pogodzie ---
    sf::RectangleShape m_weatherInfoPanel;
    sf::Text m_weatherInfoTitle;
    std::vector<WeatherInfoEntry> m_weatherInfoEntries;
    sf::RectangleShape m_weatherInfoCloseButton;
    sf::Text m_weatherInfoCloseButtonText;
    float m_weatherInfoScrollOffset; // Przesunięcie w pikselach dla przewijanej treści
    float m_weatherInfoContentHeight; // Całkowita wysokość przewijanej treści
    sf::RectangleShape m_scrollbarBg;
    sf::RectangleShape m_scrollbarHandle;
};

#endif // UIMANAGER_H
