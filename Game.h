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
#include "Slider.h" // ZMIANA: Dodano include dla Slidera

class Game {
public:
    float currentMoney; float currentEnergy; float maxEnergy; float environmentHealth; float totalGameTimeSeconds;
    std::vector<PlacedObject> placedObjects;
    std::string currentSaveName;

    Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures);
    void reset();
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawForPause(sf::RenderWindow& window);
    void updateEnergyMenu();
    void drawEnergyMenu(sf::RenderWindow& window);
    void drawOptionsMenu(sf::RenderWindow& window);

private:
    sf::Font& m_font;
    std::vector<sf::Texture>& m_buildingTextures;

    GameBackground m_gameBg;
    BuildMenu m_buildMenu;
    HUD m_hud;

    sf::Texture m_grassTexture;
    sf::RectangleShape m_grassArea;

    bool m_hammerPressed = false;
    sf::FloatRect m_hammerHotspot{14.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_optionsIconHotspot{1000.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_pauseIconHotspot{1095.f, 18.f, 90.f, 90.f};
    sf::FloatRect m_playableArea{0.f, 121.f, 1200.f, 579.f};

    sf::RectangleShape m_energyPanel;
    sf::Text m_energyMenuTitle;
    sf::Text m_energyInfoText;
    sf::Text m_energyValueText;
    sf::RectangleShape m_sellButton;
    sf::Text m_sellButtonText;
    sf::RectangleShape m_closeButton;
    sf::Text m_closeButtonText;

    // ZMIANA: UI menu opcji w grze zawiera teraz suwaki
    sf::RectangleShape m_optionsPanel;
    sf::Text m_optionsMenuTitle;
    sf::Text m_musicLabel;
    sf::Text m_sfxLabel;
    std::unique_ptr<Slider> m_musicSlider;
    std::unique_ptr<Slider> m_sfxSlider;
    float m_savedMusicValue;
    float m_savedSfxValue;
    sf::RectangleShape m_applyButton;
    sf::Text m_applyButtonText;
    sf::RectangleShape m_cancelButton;
    sf::Text m_cancelButtonText;

    void placeBuilding(int typeId, float price, sf::Vector2f position);
};

#endif // GAME_H
