#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
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
#include "BuildingTooltip.h" // Zastąpiono definicję klasy tym includem

class Game {
public:
    // --- Główne zasoby gry ---
    int currentMoney;
    int currentEnergy;
    int maxEnergy;
    float environmentHealth;
    float totalGameTimeSeconds;
    float musicVolume;
    float sfxVolume;
    std::string currentSaveName;

    // --- Obiekty w grze ---
    std::vector<PlacedObject> placedObjects;

    Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures, AudioManager& audioManager);
    void reset();

    // --- Główne pętle ---
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState);
    void update(float dt);
    void draw(sf::RenderWindow& window, GameState currentState);

    // --- Akcje w grze ---
    void placeBuilding(int typeId, int price, sf::Vector2f position, bool fromPlayerAction = true);

    // --- Metody do obsługi naprawy ---
    void setRepairTarget(PlacedObject* target);
    PlacedObject* getRepairTarget() const;
    void confirmRepair();

    // --- Gettery dla innych klas ---
    Grid& getGrid();
    HUD& getHUD();
    AudioManager& getAudioManager();
    WeatherManager& getWeatherManager();

private:
    void upgradeBuilding(PlacedObject& buildingToUpgrade);
    void drawDamagedIcons(sf::RenderWindow& window);

    // --- Główne komponenty i menedżery ---
    sf::Font& m_font;
    std::vector<sf::Texture>& m_buildingTextures;
    AudioManager& m_audioManager;
    WeatherManager m_weatherManager;
    UIManager m_uiManager;

    // --- Komponenty specyficzne dla rozgrywki ---
    GameBackground m_gameBg;
    BuildMenu m_buildMenu;
    HUD m_hud;
    Grid m_grid;
    BuildingTooltip m_tooltip;

    // --- Stan rozgrywki ---
    PlacedObject* m_hoveredBuilding = nullptr;
    bool m_hammerPressed = false;
    bool m_isBuildMode = false;
    bool m_demolishModeActive = false;
    PlacedObject* m_repairTarget = nullptr;

    // --- Hotspoty ikon na głównym UI ---
    sf::FloatRect m_hammerHotspot{14.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_demolishHotspot{114.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_optionsIconHotspot{1000.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_pauseIconHotspot{1095.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_playableArea{0.f, 121.f, 1200.f, 600.f};

    // --- Elementy trybu wyburzania ---
    sf::Texture m_bulldozerTexture;
    sf::Sprite m_bulldozerSprite;
    sf::Text m_demolishCancelText;

    // --- Elementy tła ---
    sf::Texture m_grassTexture;
    sf::RectangleShape m_grassArea;
    sf::RectangleShape m_buildModeOverlay;
    sf::Texture m_repairIconTexture;
    sf::Sprite m_repairIconSprite;
};
#endif // GAME_H
