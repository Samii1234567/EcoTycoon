#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <map>
#include "GameState.h"
#include "Slider.h"
#include "WeatherSystem.h"

class Game;
class WeatherManager;

class UIManager {
public:
    UIManager(sf::Font& font, Game& game);

    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState);
    void update(const WeatherManager& weatherManager);
    void draw(sf::RenderWindow& window, GameState currentState);

    void showNotification(const std::string& message);
    void syncSliderValues();

private:
    void loadWeatherTextures();
    void drawWeatherUI(sf::RenderWindow& window);
    void drawEnergyMenu(sf::RenderWindow& window);
    void drawOptionsMenu(sf::RenderWindow& window);
    void drawRepairPanel(sf::RenderWindow& window);

    Game& m_game;
    sf::Font& m_font;

    bool m_weatherBoardExpanded;
    sf::RectangleShape m_weatherBoardBG, m_forecastBoardBG, m_weatherPanel, m_infoButtonBG;
    sf::Text m_weatherStatusText, m_forecastLabelText, m_forecastInfoText, m_weatherPanelTitle;
    sf::Text m_weatherPanelCurrentLabel, m_weatherPanelCurrentDesc, m_infoButtonText;
    sf::Sprite m_weatherIconSprite;
    sf::Sprite m_forecastIconSprite;
    std::map<WeatherType, sf::Texture> m_weatherTextures;
    sf::RectangleShape m_forecastInfoButtonBG;
    sf::Text           m_forecastInfoButtonText;
    bool m_showingForecastDetails;

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
    sf::RectangleShape m_panelOverlay;
};

#endif // UIMANAGER_H
