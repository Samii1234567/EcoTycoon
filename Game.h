#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include "SaveManager.h"
#include "GameBackground.h"
#include "BuildMenu.h"
#include "HUD.h"
#include "GameState.h"
#include "Building.h"
#include "Grid.h"
#include "AudioManager.h"
#include "WeatherManager.h"
#include "UIManager.h"
#include "BuildingTooltip.h"
#include "ContractManager.h"

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA GŁÓWNEJ KLASY GRY
//
//  Definiuje klasę `Game`, która jest sercem całej aplikacji.
//  Orkiestruje ona wszystkimi elementami rozgrywki, łącząc w sobie logikę,
//  zarządzanie zasobami, obsługę zdarzeń i rysowanie.
//  Przechowuje stan gry oraz instancje wszystkich menedżerów i komponentów.
//
// ===================================================================================


// Struktury dla cząsteczkowych efektów pogodowych.
struct WindParticle {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float rotationSpeed;
};

struct TumbleweedParticle {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float rotationSpeed;
};

class Game {
public:
    // --- Główne zasoby i stan gry (publiczne dla łatwego dostępu z menedżerów) ---
    int currentMoney;           // Aktualna ilość pieniędzy gracza.
    int currentEnergy;          // Aktualna ilość zgromadzonej energii.
    int maxEnergy;              // Maksymalna pojemność magazynów energii.
    float environmentHealth;    // Stan środowiska w procentach (0-100).
    float totalGameTimeSeconds; // Całkowity czas spędzony w bieżącej sesji gry.
    float musicVolume;          // Głośność muzyki (0.0 - 1.0).
    float sfxVolume;            // Głośność efektów dźwiękowych (0.0 - 1.0).
    std::string currentSaveName;  // Nazwa bieżącego zapisu gry.

    // Wektor przechowujący wszystkie obiekty postawione na mapie.
    std::vector<PlacedObject> placedObjects;

    // Konstruktor, inicjalizujący całą grę.
    Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures, AudioManager& audioManager);

    // Resetuje grę do stanu początkowego.
    void reset();

    // --- Główne metody pętli gry ---

    // Przetwarza zdarzenia (wejście od użytkownika).
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState);
    // Aktualizuje logikę gry o czas, który upłynął od ostatniej klatki.
    void update(float dt);
    // Rysuje całą scenę gry.
    void draw(sf::RenderWindow& window, GameState currentState);

    // --- Główne akcje w grze ---

    // Umieszcza nowy budynek na mapie.
    void placeBuilding(int typeId, int price, sf::Vector2f position, bool fromPlayerAction = true);

    // --- Metody do obsługi naprawy budynków ---

    void setRepairTarget(PlacedObject* target); // Wyznacza budynek do naprawy.
    PlacedObject* getRepairTarget() const;      // Zwraca wskaźnik na budynek do naprawy.
    void confirmRepair();                       // Potwierdza i wykonuje naprawę.

    // --- Gettery dla innych klas (umożliwiają komunikację między systemami) ---

    Grid& getGrid();
    HUD& getHUD();
    AudioManager& getAudioManager();
    WeatherManager& getWeatherManager();

    // POPRAWKA: Dodano przeciążoną metodę const, aby umożliwić dostęp z funkcji saveGame
    ContractManager& getContractManager();
    const ContractManager& getContractManager() const;

    float getNetEnergyPerSecond() const;

private:
    // --- Prywatne metody logiki gry ---

    // Ulepsza budynek na następny poziom.
    void upgradeBuilding(PlacedObject& buildingToUpgrade);

    // --- Prywatne metody do rysowania ---

    void drawDamagedIcons(sf::RenderWindow& window);
    void drawWeatherOverlay(sf::RenderWindow& window);
    void drawRainEffect(sf::RenderWindow& window);
    void drawDynamicWeatherEffects(sf::RenderWindow& window);

    // --- Prywatne metody do obsługi efektów pogody ---

    void initializeWeatherEffects();
    void updateWeatherOverlay();
    void updateRainEffect(float dt);
    void updateCloudEffect(float dt);
    void updateWindEffect(float dt);
    void updateSmogEffect(float dt);
    void updateHeatwaveEffect(float dt);

    // --- Główne komponenty i menedżery ---
    // Przechowywane jako referencje lub pełne obiekty.

    sf::Font& m_font;
    std::vector<sf::Texture>& m_buildingTextures;
    AudioManager& m_audioManager;
    WeatherManager m_weatherManager;
    UIManager m_uiManager;
    ContractManager m_contractManager;

    // --- Komponenty specyficzne dla rozgrywki ---

    GameBackground m_gameBg;
    BuildMenu m_buildMenu;
    HUD m_hud;
    Grid m_grid;
    BuildingTooltip m_tooltip;
    sf::View m_gameplayView; // "Kamera" na obszar gry, oddziela go od HUD.

    // --- Zmienne przechowujące stan rozgrywki ---

    PlacedObject* m_hoveredBuilding = nullptr; // Wskaźnik na budynek pod kursorem.
    bool m_hammerPressed = false;            // Czy kliknięto ikonę budowania.
    bool m_isBuildMode = false;              // Czy trwa przeciąganie budynku.
    bool m_demolishModeActive = false;       // Czy aktywny jest tryb burzenia.
    PlacedObject* m_repairTarget = nullptr;   // Wskaźnik na budynek w trakcie naprawy.

    // --- Zmienne do obliczeń w pętli `update` ---

    float m_netEnergyPerSecond;     // Bilans energetyczny na sekundę.
    float m_netEnvChangePerSecond;  // Zmiana środowiska na sekundę.
    float m_moneyFromContractsPerSecond; // Przychód z kontraktów na sekundę.
    float m_secondAccumulator;      // Akumulator do wyzwalania logiki co sekundę.
    float m_energyAccumulator;      // Akumulator do płynnego dodawania/odejmowania energii.

    // --- "Gorące strefy" - klikalne obszary na głównym UI ---

    sf::FloatRect m_hammerHotspot{14.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_demolishHotspot{114.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_optionsIconHotspot{1000.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_pauseIconHotspot{1095.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_contractsHotspot{890.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_playableArea{0.f, 121.f, 1200.f, 600.f}; // Obszar, na którym można budować.

    // --- Elementy graficzne UI i gry ---

    sf::Texture m_bulldozerTexture;
    sf::Sprite m_bulldozerSprite;
    sf::Text m_demolishCancelText;
    sf::Texture m_grassTexture;
    sf::RectangleShape m_grassArea;
    sf::RectangleShape m_buildModeOverlay;
    sf::Texture m_repairIconTexture;
    sf::Sprite m_repairIconSprite;
    sf::Texture m_bellTexture;
    sf::Sprite m_bellSprite;
    sf::CircleShape m_notificationCircle;
    sf::Text m_notificationCountText;

    // --- Elementy graficzne do efektów pogodowych ---

    sf::RectangleShape m_weatherOverlay;
    sf::Texture m_rainTexture, m_cloudTexture, m_leafTexture, m_smogTexture, m_tumbleweedTexture;
    sf::Sprite m_rainSprite1, m_rainSprite2;
    std::vector<sf::Sprite> m_cloudSprites;
    std::vector<sf::Sprite> m_smogSprites;
    std::vector<WindParticle> m_windParticles;
    std::vector<TumbleweedParticle> m_tumbleweedParticles;
    sf::Clock m_effectsClock;
};
#endif // GAME_H
