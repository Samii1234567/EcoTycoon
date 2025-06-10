#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "Building.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <memory>
#include <sstream> // Potrzebne dla ostringstream w tooltipie
#include "SaveManager.h"
#include "GameBackground.h"
#include "BuildMenu.h"
#include "HUD.h"
#include "GameState.h"
#include "Slider.h"

// ZMIANA: Implementacja klasy BuildingTooltip została przeniesiona w całości do pliku nagłówkowego
class BuildingTooltip {
public:
    BuildingTooltip() : m_visible(false), m_upgradeClicked(false) {}

    void initialize(sf::Font& font) {
        m_background.setSize({220.f, 120.f});
        m_background.setFillColor(sf::Color(255, 255, 255, 240));
        m_background.setOutlineColor(sf::Color::Black);
        m_background.setOutlineThickness(2.f);

        m_titleText.setFont(font);
        m_titleText.setCharacterSize(16);
        m_titleText.setStyle(sf::Text::Bold);
        m_titleText.setFillColor(sf::Color::Black);

        m_levelText.setFont(font);
        m_levelText.setCharacterSize(14);
        m_levelText.setFillColor(sf::Color::Black);

        m_infoText.setFont(font);
        m_infoText.setCharacterSize(14);
        m_infoText.setFillColor(sf::Color::Black);

        m_upgradeButton.setSize({200.f, 30.f});
        m_upgradeButtonText.setFont(font);
        m_upgradeButtonText.setCharacterSize(14);
        m_upgradeButtonText.setFillColor(sf::Color::Black);
    }

    void show(const PlacedObject& building, int playerMoney) {
        m_visible = true;
        m_upgradeClicked = false;

        std::string name;
        std::string info;
        int cost = 0;
        bool maxLevel = (building.level >= GameConstants::MAX_LEVEL);

        switch (building.typeId) {
        case GameConstants::SOLAR_PANEL_ID:
            name = "Panel sloneczny";
            info = "Produkcja: " + std::to_string(GameConstants::SOLAR_PANEL_DATA.value[building.level - 1]) + " E/s";
            if (!maxLevel) cost = GameConstants::SOLAR_PANEL_DATA.upgradeCost[building.level];
            break;
        case GameConstants::WIND_TURBINE_ID:
            name = "Turbina wiatrowa";
            info = "Produkcja: " + std::to_string(GameConstants::WIND_TURBINE_DATA.value[building.level - 1]) + " E/s\n";
            info += "Srodowisko: +" + std::to_string(GameConstants::WIND_TURBINE_DATA.envEffect[building.level - 1]) + "%/s";
            if (!maxLevel) cost = GameConstants::WIND_TURBINE_DATA.upgradeCost[building.level];
            break;
        case GameConstants::ENERGY_STORAGE_ID:
            name = "Magazyn energii";
            info = "Pojemnosc: +" + std::to_string(GameConstants::STORAGE_DATA.value[building.level - 1]);
            if (!maxLevel) cost = GameConstants::STORAGE_DATA.upgradeCost[building.level];
            break;
        }

        m_titleText.setString(name);
        m_levelText.setString("Poziom: " + std::to_string(building.level));
        m_infoText.setString(info);

        if (maxLevel) {
            m_upgradeButtonText.setString("Maksymalny poziom");
            m_upgradeButton.setFillColor(sf::Color(100, 100, 100));
        } else {
            m_upgradeButtonText.setString("Ulepsz (" + std::to_string(cost) + "$)");
            if(playerMoney >= cost) {
                m_upgradeButton.setFillColor(sf::Color(90, 160, 90));
            } else {
                m_upgradeButton.setFillColor(sf::Color(200, 90, 90));
            }
        }

        m_background.setPosition(building.sprite.getPosition().x - m_background.getSize().x / 2, building.sprite.getPosition().y - m_background.getSize().y - 10);
        m_titleText.setPosition(m_background.getPosition() + sf::Vector2f(10, 5));
        m_levelText.setPosition(m_background.getPosition() + sf::Vector2f(10, 30));
        m_infoText.setPosition(m_background.getPosition() + sf::Vector2f(10, 50));
        m_upgradeButton.setPosition(m_background.getPosition() + sf::Vector2f(10, 85));

        sf::FloatRect textRect = m_upgradeButtonText.getLocalBounds();
        m_upgradeButtonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        m_upgradeButtonText.setPosition(m_upgradeButton.getPosition().x + m_upgradeButton.getSize().x / 2.0f, m_upgradeButton.getPosition().y + m_upgradeButton.getSize().y / 2.0f);
    }

    void hide() {
        m_visible = false;
        m_upgradeClicked = false;
    }

    bool isVisible() const { return m_visible; }

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
        if (!m_visible) return;
        m_upgradeClicked = false;

        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            if (m_upgradeButton.getGlobalBounds().contains(window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y}))) {
                m_upgradeClicked = true;
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (!m_visible) return;
        window.draw(m_background);
        window.draw(m_titleText);
        window.draw(m_levelText);
        window.draw(m_infoText);
        window.draw(m_upgradeButton);
        window.draw(m_upgradeButtonText);
    }

    bool isUpgradeClicked() const { return m_upgradeClicked; }

private:
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    sf::Text m_levelText;
    sf::Text m_infoText;
    sf::RectangleShape m_upgradeButton;
    sf::Text m_upgradeButtonText;
    bool m_visible;
    bool m_upgradeClicked;
};


class Game {
public:
    int currentMoney; int currentEnergy; int maxEnergy;
    float environmentHealth; float totalGameTimeSeconds;

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
    void upgradeBuilding(PlacedObject& buildingToUpgrade);

    sf::Font& m_font;
    std::vector<sf::Texture>& m_buildingTextures;

    GameBackground m_gameBg;
    BuildMenu m_buildMenu;
    HUD m_hud;
    Grid m_grid;
    BuildingTooltip m_tooltip;
    PlacedObject* m_hoveredBuilding = nullptr;

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

    sf::SoundBuffer m_solarPanelBuffer, m_windTurbineBuffer, m_energyStorageBuffer, m_upgradeBuffer;
    sf::Sound m_solarPanelSound, m_windTurbineSound, m_energyStorageSound, m_upgradeSound;
};

#endif // GAME_H
