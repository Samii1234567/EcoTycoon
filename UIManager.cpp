#include "UIManager.h"
#include "Game.h"
#include "WeatherManager.h"
#include "Constants.h"
#include <sstream>
#include <iomanip>
#include <iostream>

UIManager::UIManager(sf::Font& font, Game& game)
    : m_font(font), m_game(game), m_weatherBoardExpanded(false), m_showNotification(false), m_showingForecastDetails(false) {

    const unsigned WIN_W = 1200, WIN_H = 800;

    loadWeatherTextures();

    const float boardWidth = 300.f;
    const float boardX = (WIN_W - boardWidth) / 2.f;
    const float currentPanelHeight = 53.f;
    const float forecastPanelHeight = 53.f;
    const float panelSpacing = 4.f;

    m_weatherBoardBG.setSize({boardWidth, currentPanelHeight});
    m_weatherBoardBG.setFillColor(sf::Color(0,0,0,100));
    m_weatherBoardBG.setOutlineColor(sf::Color::White);
    m_weatherBoardBG.setOutlineThickness(1.f);
    m_weatherBoardBG.setPosition(boardX, 5.f);
    m_weatherIconSprite.setPosition(boardX + 30.f, m_weatherBoardBG.getPosition().y + currentPanelHeight / 2.f);
    m_weatherStatusText.setFont(m_font);
    m_weatherStatusText.setCharacterSize(18);
    m_weatherStatusText.setFillColor(sf::Color::White);
    m_infoButtonBG.setSize({50.f, 30.f});
    m_infoButtonBG.setFillColor(sf::Color(80, 120, 180, 150));
    m_infoButtonBG.setOutlineColor(sf::Color::White);
    m_infoButtonBG.setOutlineThickness(1.f);
    m_infoButtonBG.setPosition(boardX + boardWidth - 60.f, m_weatherBoardBG.getPosition().y + (currentPanelHeight - 30.f) / 2.f);
    m_infoButtonText.setFont(m_font);
    m_infoButtonText.setString("Info");
    m_infoButtonText.setCharacterSize(16);
    m_infoButtonText.setFillColor(sf::Color::White);
    sf::FloatRect infoBtnRect = m_infoButtonText.getLocalBounds();
    m_infoButtonText.setOrigin(infoBtnRect.left + infoBtnRect.width / 2.f, infoBtnRect.top + infoBtnRect.height / 2.f);
    m_infoButtonText.setPosition(m_infoButtonBG.getPosition().x + m_infoButtonBG.getSize().x / 2.f, m_infoButtonBG.getPosition().y + m_infoButtonBG.getSize().y / 2.f);
    m_forecastBoardBG.setSize({boardWidth, forecastPanelHeight});
    m_forecastBoardBG.setFillColor(sf::Color(0,0,0,80));
    m_forecastBoardBG.setOutlineColor(sf::Color(180,180,180));
    m_forecastBoardBG.setOutlineThickness(1.f);
    m_forecastBoardBG.setPosition(boardX, m_weatherBoardBG.getPosition().y + currentPanelHeight + panelSpacing);
    m_forecastIconSprite.setPosition(boardX + 30.f, m_forecastBoardBG.getPosition().y + forecastPanelHeight / 2.f);
    float forecastTextBlockY = m_forecastBoardBG.getPosition().y + (forecastPanelHeight - 30.f) / 2.f;
    m_forecastLabelText.setFont(m_font);
    m_forecastLabelText.setString("Prognoza:");
    m_forecastLabelText.setCharacterSize(14);
    m_forecastLabelText.setFillColor(sf::Color(200,200,200));
    m_forecastLabelText.setPosition(boardX + 60.f, forecastTextBlockY);
    m_forecastInfoText.setFont(m_font);
    m_forecastInfoText.setCharacterSize(16);
    m_forecastInfoText.setFillColor(sf::Color::White);
    m_forecastInfoText.setPosition(boardX + 60.f, forecastTextBlockY + 15.f);
    m_forecastInfoButtonBG.setSize({50.f, 30.f});
    m_forecastInfoButtonBG.setFillColor(sf::Color(80, 120, 180, 150));
    m_forecastInfoButtonBG.setOutlineColor(sf::Color::White);
    m_forecastInfoButtonBG.setOutlineThickness(1.f);
    m_forecastInfoButtonBG.setPosition(boardX + boardWidth - 60.f, m_forecastBoardBG.getPosition().y + (forecastPanelHeight - 30.f) / 2.f);
    m_forecastInfoButtonText.setFont(m_font);
    m_forecastInfoButtonText.setString("Info");
    m_forecastInfoButtonText.setCharacterSize(16);
    m_forecastInfoButtonText.setFillColor(sf::Color::White);
    sf::FloatRect forecastInfoBtnRect = m_forecastInfoButtonText.getLocalBounds();
    m_forecastInfoButtonText.setOrigin(forecastInfoBtnRect.left + forecastInfoBtnRect.width / 2.f, forecastInfoBtnRect.top + forecastInfoBtnRect.height / 2.f);
    m_forecastInfoButtonText.setPosition(m_forecastInfoButtonBG.getPosition().x + m_forecastInfoButtonBG.getSize().x / 2.f, m_forecastInfoButtonBG.getPosition().y + m_forecastInfoButtonBG.getSize().y / 2.f);
    m_weatherPanel.setSize({600, 250});
    m_weatherPanel.setFillColor(sf::Color(40,40,60,240));
    m_weatherPanel.setOutlineColor(sf::Color::White);
    m_weatherPanel.setOutlineThickness(2.f);
    m_weatherPanel.setPosition((WIN_W - 600.f)/2.f, (WIN_H - 250.f)/2.f);
    m_weatherPanelTitle.setFont(m_font);
    m_weatherPanelTitle.setString("Szczegoly Pogody");
    m_weatherPanelTitle.setCharacterSize(32);
    m_weatherPanelTitle.setFillColor(sf::Color::White);
    m_weatherPanelTitle.setPosition(m_weatherPanel.getPosition().x + (m_weatherPanel.getSize().x - m_weatherPanelTitle.getGlobalBounds().width)/2.f, m_weatherPanel.getPosition().y + 10.f);
    m_weatherPanelCurrentLabel.setFont(m_font);
    m_weatherPanelCurrentLabel.setCharacterSize(24);
    m_weatherPanelCurrentLabel.setFillColor(sf::Color(200,200,200));
    m_weatherPanelCurrentLabel.setPosition(m_weatherPanel.getPosition().x + 20.f, m_weatherPanel.getPosition().y + 70.f);
    m_weatherPanelCurrentDesc.setFont(m_font);
    m_weatherPanelCurrentDesc.setCharacterSize(20);
    m_weatherPanelCurrentDesc.setFillColor(sf::Color::White);
    m_weatherPanelCurrentDesc.setPosition(m_weatherPanel.getPosition().x + 20.f, m_weatherPanel.getPosition().y + 110.f);

    m_panelOverlay.setSize({(float)WIN_W, (float)WIN_H});
    m_panelOverlay.setFillColor({0,0,0,100});
    m_repairPanel.setSize({400, 200});
    m_repairPanel.setFillColor({40, 40, 60, 240});
    m_repairPanel.setOutlineColor(sf::Color::White);
    m_repairPanel.setOutlineThickness(2.f);
    m_repairPanel.setPosition((WIN_W - 400.f) / 2.f, (WIN_H - 200.f) / 2.f);
    m_repairTitle.setFont(m_font);
    m_repairTitle.setString("Naprawa budynku");
    m_repairTitle.setCharacterSize(28);
    m_repairTitle.setFillColor(sf::Color::White);
    m_repairTitle.setPosition(m_repairPanel.getPosition().x + (m_repairPanel.getSize().x - m_repairTitle.getGlobalBounds().width) / 2.f, m_repairPanel.getPosition().y + 15.f);
    m_repairInfoText.setFont(m_font);
    m_repairInfoText.setCharacterSize(20);
    m_repairInfoText.setFillColor(sf::Color::White);
    m_repairInfoText.setPosition(m_repairPanel.getPosition().x + 20.f, m_repairPanel.getPosition().y + 70.f);
    m_repairConfirmButton.setSize({150, 40});
    m_repairConfirmButton.setFillColor(sf::Color(80, 180, 80));
    m_repairConfirmButton.setPosition(m_repairPanel.getPosition().x + 40.f, m_repairPanel.getPosition().y + 130.f);
    m_repairConfirmText.setString("Napraw");
    m_repairConfirmText.setFont(m_font);
    m_repairConfirmText.setCharacterSize(20);
    sf::FloatRect rct = m_repairConfirmText.getLocalBounds();
    m_repairConfirmText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_repairConfirmText.setPosition(m_repairConfirmButton.getPosition().x + m_repairConfirmButton.getSize().x / 2.f, m_repairConfirmButton.getPosition().y + m_repairConfirmButton.getSize().y / 2.f);
    m_repairCancelButton.setSize({150, 40});
    m_repairCancelButton.setFillColor(sf::Color(180, 80, 80));
    m_repairCancelButton.setPosition(m_repairPanel.getPosition().x + m_repairPanel.getSize().x - 190.f, m_repairPanel.getPosition().y + 130.f);
    m_repairCancelText.setString("Anuluj");
    m_repairCancelText.setFont(m_font);
    m_repairCancelText.setCharacterSize(20);
    rct = m_repairCancelText.getLocalBounds();
    m_repairCancelText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_repairCancelText.setPosition(m_repairCancelButton.getPosition().x + m_repairCancelButton.getSize().x / 2.f, m_repairCancelButton.getPosition().y + m_repairCancelButton.getSize().y / 2.f);

    m_energyPanel.setSize({500.f, 300.f});
    m_energyPanel.setFillColor({40, 40, 60, 220});
    m_energyPanel.setOutlineColor({120, 120, 150, 255});
    m_energyPanel.setOutlineThickness(2.f);
    m_energyPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 300.f) / 2.f);
    m_energyMenuTitle.setString("Zarzadzanie Energia");
    m_energyMenuTitle.setFont(m_font); m_energyMenuTitle.setCharacterSize(32); m_energyMenuTitle.setFillColor(sf::Color::White);
    m_energyMenuTitle.setPosition(m_energyPanel.getPosition().x + (m_energyPanel.getSize().x - m_energyMenuTitle.getLocalBounds().width) / 2.f, m_energyPanel.getPosition().y + 20.f);
    m_energyInfoText.setFont(m_font); m_energyInfoText.setCharacterSize(22); m_energyInfoText.setFillColor(sf::Color::White);
    m_energyInfoText.setPosition(m_energyPanel.getPosition().x + 30.f, m_energyPanel.getPosition().y + 80.f);
    m_energyValueText.setFont(m_font); m_energyValueText.setCharacterSize(22); m_energyValueText.setFillColor(sf::Color(180, 255, 180));
    m_energyValueText.setPosition(m_energyPanel.getPosition().x + 30.f, m_energyPanel.getPosition().y + 110.f);
    m_sellButton.setSize({200.f, 50.f}); m_sellButton.setFillColor(sf::Color(80, 180, 80));
    m_sellButton.setPosition(m_energyPanel.getPosition().x + 50.f, m_energyPanel.getPosition().y + 190.f);
    m_sellButtonText.setString("Sprzedaj"); m_sellButtonText.setFont(m_font); m_sellButtonText.setCharacterSize(24);
    m_sellButtonText.setPosition(m_sellButton.getPosition().x + (m_sellButton.getSize().x - m_sellButtonText.getLocalBounds().width)/2.f, m_sellButton.getPosition().y + 10.f);
    m_closeButton.setSize({200.f, 50.f}); m_closeButton.setFillColor(sf::Color(180, 80, 80));
    m_closeButton.setPosition(m_energyPanel.getPosition().x + m_energyPanel.getSize().x - 250.f, m_energyPanel.getPosition().y + 190.f);
    m_closeButtonText.setString("Zamknij"); m_closeButtonText.setFont(m_font); m_closeButtonText.setCharacterSize(24);
    m_closeButtonText.setPosition(m_closeButton.getPosition().x + (m_closeButton.getSize().x - m_closeButtonText.getLocalBounds().width)/2.f, m_closeButton.getPosition().y + 10.f);

    m_optionsPanel.setSize({500.f, 400.f});
    m_optionsPanel.setFillColor({40, 40, 60, 220});
    m_optionsPanel.setOutlineColor({120, 120, 150, 255});
    m_optionsPanel.setOutlineThickness(2.f);
    m_optionsPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 400.f) / 2.f);
    m_optionsMenuTitle.setString("Opcje");
    m_optionsMenuTitle.setFont(m_font); m_optionsMenuTitle.setCharacterSize(32); m_optionsMenuTitle.setFillColor(sf::Color::White);
    m_optionsMenuTitle.setPosition(m_optionsPanel.getPosition().x + (m_optionsPanel.getSize().x - m_optionsMenuTitle.getLocalBounds().width) / 2.f, m_optionsPanel.getPosition().y + 20.f);
    m_musicLabel.setString("Muzyka"); m_musicLabel.setFont(m_font); m_musicLabel.setCharacterSize(24); m_musicLabel.setFillColor(sf::Color::White);
    m_musicLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 100.f);
    m_musicSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 140.f, 400.f, 6.f, 15.f);
    m_sfxLabel.setString("Efekty dzwiekowe"); m_sfxLabel.setFont(m_font); m_sfxLabel.setCharacterSize(24); m_sfxLabel.setFillColor(sf::Color::White);
    m_sfxLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 200.f);
    m_sfxSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 240.f, 400.f, 6.f, 15.f);
    m_applyButton.setSize({200.f, 50.f}); m_applyButton.setFillColor(sf::Color(80, 120, 180));
    m_applyButton.setPosition(m_optionsPanel.getPosition().x + 40.f, m_optionsPanel.getPosition().y + 320.f);
    m_applyButtonText.setString("Zastosuj"); m_applyButtonText.setFont(m_font); m_applyButtonText.setCharacterSize(24);
    m_applyButtonText.setPosition(m_applyButton.getPosition().x + (m_applyButton.getSize().x - m_applyButtonText.getLocalBounds().width)/2.f, m_applyButton.getPosition().y + 10.f);
    m_cancelButton.setSize({200.f, 50.f}); m_cancelButton.setFillColor(sf::Color(180, 80, 80));
    m_cancelButton.setPosition(m_optionsPanel.getPosition().x + m_optionsPanel.getSize().x - 240.f, m_optionsPanel.getPosition().y + 320.f);
    m_cancelButtonText.setString("Anuluj"); m_cancelButtonText.setFont(m_font); m_cancelButtonText.setCharacterSize(24);
    m_cancelButtonText.setPosition(m_cancelButton.getPosition().x + (m_cancelButton.getSize().x - m_cancelButtonText.getLocalBounds().width)/2.f, m_cancelButton.getPosition().y + 10.f);

    m_notificationText.setFont(m_font);
    m_notificationText.setCharacterSize(32);
    m_notificationText.setFillColor(sf::Color::Red);
    m_notificationText.setOutlineColor(sf::Color::Black);
    m_notificationText.setOutlineThickness(2.f);
    m_notificationText.setPosition(WIN_W / 2.f, 125.f);
}

void UIManager::loadWeatherTextures() {
    for (int i = 0; i <= static_cast<int>(WeatherType::Smog); ++i) {
        WeatherType type = static_cast<WeatherType>(i);
        std::string filename = getWeatherIconFilename(type);
        if (!filename.empty()) {
            if (!m_weatherTextures[type].loadFromFile(filename)) {
                std::cerr << "Nie udalo sie wczytac tekstury pogody: " << filename << std::endl;
            }
        }
    }
}

void UIManager::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState) {
    if (m_game.getRepairTarget() != nullptr) {
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_repairConfirmButton.getGlobalBounds().contains(pos)) {
                m_game.confirmRepair();
            } else if (m_repairCancelButton.getGlobalBounds().contains(pos)) {
                m_game.setRepairTarget(nullptr);
            }
        }
        return;
    }

    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});

        if (m_infoButtonBG.getGlobalBounds().contains(pos) && currentState == GameState::Playing) {
            m_showingForecastDetails = false;
            m_weatherBoardExpanded = true;
            return;
        }
        if (m_forecastInfoButtonBG.getGlobalBounds().contains(pos) && currentState == GameState::Playing) {
            if (!m_game.getWeatherManager().getForecast().empty()) {
                m_showingForecastDetails = true;
                m_weatherBoardExpanded = true;
            }
            return;
        }
        if (m_weatherBoardExpanded && !m_weatherPanel.getGlobalBounds().contains(pos)) {
            m_weatherBoardExpanded = false;
            return;
        }
    }

    switch (currentState) {
    case GameState::EnergyMenu:
        if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) {
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_sellButton.getGlobalBounds().contains(pos)) {
                if (m_game.currentEnergy > 0) {
                    m_game.getAudioManager().play(SoundEffect::CashRegister);
                    m_game.currentMoney += m_game.currentEnergy * GameConstants::ENERGY_SELL_PRICE;
                    m_game.currentEnergy = 0;
                    m_game.getHUD().flashMoney();
                }
                currentState = GameState::Playing;
            } else if (m_closeButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                currentState = GameState::Playing;
            }
        }
        break;
    case GameState::InGameOptionsMenu:
        m_musicSlider->handleEvent(ev, window);
        m_sfxSlider->handleEvent(ev, window);
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
            syncSliderValues();
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_applyButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                m_game.musicVolume = m_musicSlider->getValue();
                m_game.sfxVolume = m_sfxSlider->getValue();
                currentState = GameState::Playing;
            } else if (m_cancelButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                syncSliderValues();
                currentState = GameState::Playing;
            }
        }
        break;
    default:
        break;
    }
}

void UIManager::update(const WeatherManager& weatherManager) {
    if (m_showNotification && m_notificationClock.getElapsedTime().asSeconds() > 2.0f) {
        m_showNotification = false;
    }

    WeatherType currentWeather = weatherManager.getCurrentWeather();
    if (m_weatherTextures.count(currentWeather)) {
        const sf::Texture& tex = m_weatherTextures.at(currentWeather);
        m_weatherIconSprite.setTexture(tex, true);
        m_weatherIconSprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
        m_weatherIconSprite.setScale(36.f / tex.getSize().x, 36.f / tex.getSize().y);
    }
    float timerValue = weatherManager.getWeatherTimer();
    int minutes = static_cast<int>(timerValue) / 60;
    int seconds = static_cast<int>(timerValue) % 60;
    std::ostringstream oss_timer;
    oss_timer << " (" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds << ")";
    m_weatherStatusText.setString(getWeatherName(currentWeather) + oss_timer.str());
    sf::FloatRect textBounds = m_weatherStatusText.getLocalBounds();
    m_weatherStatusText.setOrigin(textBounds.left, textBounds.top + textBounds.height / 2.f);
    m_weatherStatusText.setPosition(m_weatherBoardBG.getPosition().x + 60.f, m_weatherBoardBG.getPosition().y + m_weatherBoardBG.getSize().y / 2.f);

    const auto& forecast = weatherManager.getForecast();
    if (!forecast.empty()) {
        WeatherType nextWeather = forecast.front();
        m_forecastInfoText.setString(getWeatherName(nextWeather));
        if (m_weatherTextures.count(nextWeather)) {
            const sf::Texture& tex = m_weatherTextures.at(nextWeather);
            m_forecastIconSprite.setTexture(tex, true);
            m_forecastIconSprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
            m_forecastIconSprite.setScale(30.f / tex.getSize().x, 30.f / tex.getSize().y);
        }
    }

    if (m_weatherBoardExpanded) {
        if (m_showingForecastDetails) {
            if (!forecast.empty()) {
                WeatherType forecastWeather = forecast.front();
                m_weatherPanelCurrentLabel.setString("Prognoza: " + getWeatherName(forecastWeather));
                m_weatherPanelCurrentDesc.setString(getWeatherDescription(forecastWeather));
            }
        } else {
            m_weatherPanelCurrentLabel.setString("Aktualna pogoda: " + getWeatherName(currentWeather));
            m_weatherPanelCurrentDesc.setString(getWeatherDescription(currentWeather));
        }
    }

    if (PlacedObject* target = m_game.getRepairTarget()) {
        int repairCost = target->price / 4;
        m_repairInfoText.setString("Koszt naprawy: " + std::to_string(repairCost) + "$");
    }

    std::ostringstream oss;
    oss << "Zgromadzona energia: " << m_game.currentEnergy << " / " << m_game.maxEnergy;
    m_energyInfoText.setString(oss.str());
    oss.str("");
    oss.clear();
    oss << "Wartosc sprzedazy: " << (m_game.currentEnergy * GameConstants::ENERGY_SELL_PRICE) << "$";
    m_energyValueText.setString(oss.str());
}

void UIManager::draw(sf::RenderWindow& window, GameState currentState) {
    drawWeatherUI(window);

    if (currentState == GameState::EnergyMenu) drawEnergyMenu(window);
    else if (currentState == GameState::InGameOptionsMenu) drawOptionsMenu(window);

    drawRepairPanel(window);

    if (m_showNotification) {
        window.draw(m_notificationText);
    }
}

void UIManager::drawWeatherUI(sf::RenderWindow& window) {
    window.draw(m_weatherBoardBG);
    window.draw(m_weatherIconSprite);
    window.draw(m_weatherStatusText);
    window.draw(m_infoButtonBG);
    window.draw(m_infoButtonText);

    if (!m_game.getWeatherManager().getForecast().empty()) {
        window.draw(m_forecastBoardBG);
        window.draw(m_forecastIconSprite);
        window.draw(m_forecastLabelText);
        window.draw(m_forecastInfoText);
        window.draw(m_forecastInfoButtonBG);
        window.draw(m_forecastInfoButtonText);
    }

    if (m_weatherBoardExpanded) {
        window.draw(m_weatherPanel);
        window.draw(m_weatherPanelTitle);
        window.draw(m_weatherPanelCurrentLabel);
        window.draw(m_weatherPanelCurrentDesc);
    }
}

void UIManager::drawRepairPanel(sf::RenderWindow& window) {
    if (m_game.getRepairTarget() != nullptr) {
        window.draw(m_panelOverlay);
        window.draw(m_repairPanel);
        window.draw(m_repairTitle);
        window.draw(m_repairInfoText);
        window.draw(m_repairConfirmButton);
        window.draw(m_repairConfirmText);
        window.draw(m_repairCancelButton);
        window.draw(m_repairCancelText);
    }
}

void UIManager::drawEnergyMenu(sf::RenderWindow& window) {
    window.draw(m_energyPanel);
    window.draw(m_energyMenuTitle);
    window.draw(m_energyInfoText);
    window.draw(m_energyValueText);
    window.draw(m_sellButton);
    window.draw(m_sellButtonText);
    window.draw(m_closeButton);
    window.draw(m_closeButtonText);
}

void UIManager::drawOptionsMenu(sf::RenderWindow& window) {
    window.draw(m_optionsPanel);
    window.draw(m_optionsMenuTitle);
    window.draw(m_musicLabel);
    m_musicSlider->draw(window);
    window.draw(m_sfxLabel);
    m_sfxSlider->draw(window);
    window.draw(m_applyButton);
    window.draw(m_applyButtonText);
    window.draw(m_cancelButton);
    window.draw(m_cancelButtonText);
}

void UIManager::showNotification(const std::string& message) {
    m_notificationText.setString(message);
    sf::FloatRect textRect = m_notificationText.getLocalBounds();
    m_notificationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_showNotification = true;
    m_notificationClock.restart();
}

void UIManager::syncSliderValues() {
    m_musicSlider->setValue(m_game.musicVolume);
    m_sfxSlider->setValue(m_game.sfxVolume);
}
