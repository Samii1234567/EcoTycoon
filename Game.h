#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <memory>
#include "SaveManager.h"
#include "GameBackground.h"
#include "BuildMenu.h"
#include "HUD.h"
#include "GameState.h"
#include "Slider.h"
#include "Building.h"
#include "Grid.h" // ZMIANA: Dołączenie nowego pliku nagłówkowego

struct PlacedObject {
    int typeId;
    sf::Sprite sprite;
    std::unique_ptr<Building> logic;
    int price;
    sf::Vector2i gridPosition;
    sf::Vector2i sizeInCells;
};

class Game {
public:
    int currentMoney;
    int currentEnergy;
    int maxEnergy;
    float environmentHealth;
    float totalGameTimeSeconds;

    std::vector<PlacedObject> placedObjects;
    std::string currentSaveName;
    float musicVolume;
    float sfxVolume;

    Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures);
    void reset();
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState, sf::Sound& clickSound);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawForPause(sf::RenderWindow& window);
    void updateEnergyMenu();
    void drawEnergyMenu(sf::RenderWindow& window);
    void drawOptionsMenu(sf::RenderWindow& window);

    Grid& getGrid() { return m_grid; }
    void placeBuilding(int typeId, int price, sf::Vector2f position, bool fromPlayerAction = true);

private:
    sf::Font& m_font;
    std::vector<sf::Texture>& m_buildingTextures;

    GameBackground m_gameBg;
    BuildMenu m_buildMenu;
    HUD m_hud;
    Grid m_grid; // ZMIANA: Usunięto stąd definicję klasy, zostaje tylko pole

    sf::Texture m_grassTexture;
    sf::RectangleShape m_grassArea;
    sf::RectangleShape m_buildModeOverlay;

    bool m_hammerPressed = false;
    bool m_isBuildMode = false;
    bool m_demolishModeActive = false;

    sf::FloatRect m_hammerHotspot{14.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_demolishHotspot{114.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_optionsIconHotspot{1000.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_pauseIconHotspot{1095.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_playableArea{0.f, 121.f, 1200.f, 600.f};

    sf::Texture m_bulldozerTexture;
    sf::Sprite m_bulldozerSprite;
    sf::Text m_demolishCancelText;

    sf::RectangleShape m_energyPanel;
    sf::Text m_energyMenuTitle, m_energyInfoText, m_energyValueText;
    sf::RectangleShape m_sellButton, m_closeButton;
    sf::Text m_sellButtonText, m_closeButtonText;

    sf::RectangleShape m_optionsPanel;
    sf::Text m_optionsMenuTitle, m_musicLabel, m_sfxLabel;
    std::unique_ptr<Slider> m_musicSlider;
    std::unique_ptr<Slider> m_sfxSlider;
    sf::RectangleShape m_applyButton, m_cancelButton;
    sf::Text m_applyButtonText, m_cancelButtonText;

    sf::SoundBuffer m_solarPanelBuffer, m_windTurbineBuffer, m_energyStorageBuffer;
    sf::Sound m_solarPanelSound, m_windTurbineSound, m_energyStorageSound;
};

#endif // GAME_H
