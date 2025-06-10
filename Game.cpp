#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>

std::unique_ptr<Building> createBuildingById(int typeId);

Game::Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures)
    : m_font(font),
    m_buildingTextures(buildingTextures),
    m_hud(font)
{
    m_gameBg.loadFromFile("images/eco_tycoon_game.png");
    m_buildMenu.initialize(m_font, m_buildingTextures);

    if (!m_grassTexture.loadFromFile("images/eco_grass.png")) {
        std::cerr << "Blad: nie mozna wczytac eco_grass.png\n";
    }
    m_grassArea.setSize({1200.f, 595.f});
    m_grassArea.setPosition(0.f, 127.f);
    m_grassArea.setTexture(&m_grassTexture);

    const unsigned WIN_W = 1200, WIN_H = 800;

    // Inicjalizacja UI menu energii
    m_energyPanel.setSize({500.f, 300.f});
    m_energyPanel.setFillColor({40, 40, 60, 220});
    m_energyPanel.setOutlineColor({120, 120, 150, 255});
    m_energyPanel.setOutlineThickness(2.f);
    m_energyPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 300.f) / 2.f);
    m_energyMenuTitle.setString("Zarzadzanie Energia");
    m_energyMenuTitle.setFont(m_font);
    m_energyMenuTitle.setCharacterSize(32);
    m_energyMenuTitle.setFillColor(sf::Color::White);
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

    // ZMIANA: Przebudowa UI menu opcji w grze
    m_optionsPanel.setSize({500.f, 400.f});
    m_optionsPanel.setFillColor({40, 40, 60, 220});
    m_optionsPanel.setOutlineColor({120, 120, 150, 255});
    m_optionsPanel.setOutlineThickness(2.f);
    m_optionsPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 400.f) / 2.f);
    m_optionsMenuTitle.setString("Opcje");
    m_optionsMenuTitle.setFont(m_font);
    m_optionsMenuTitle.setCharacterSize(32);
    m_optionsMenuTitle.setFillColor(sf::Color::White);
    m_optionsMenuTitle.setPosition(m_optionsPanel.getPosition().x + (m_optionsPanel.getSize().x - m_optionsMenuTitle.getLocalBounds().width) / 2.f, m_optionsPanel.getPosition().y + 20.f);

    // Etykiety i suwaki
    m_musicLabel.setString("Muzyka"); m_musicLabel.setFont(m_font); m_musicLabel.setCharacterSize(24); m_musicLabel.setFillColor(sf::Color::White);
    m_musicLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 100.f);
    m_musicSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 140.f, 400.f, 6.f, 15.f);
    m_sfxLabel.setString("Efekty dzwiekowe"); m_sfxLabel.setFont(m_font); m_sfxLabel.setCharacterSize(24); m_sfxLabel.setFillColor(sf::Color::White);
    m_sfxLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 200.f);
    m_sfxSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 240.f, 400.f, 6.f, 15.f);

    m_savedMusicValue = 0.5f;
    m_savedSfxValue = 0.5f;
    m_musicSlider->setValue(m_savedMusicValue);
    m_sfxSlider->setValue(m_savedSfxValue);

    // Przyciski Zastosuj / Anuluj
    m_applyButton.setSize({200.f, 50.f}); m_applyButton.setFillColor(sf::Color(80, 120, 180));
    m_applyButton.setPosition(m_optionsPanel.getPosition().x + 40.f, m_optionsPanel.getPosition().y + 320.f);
    m_applyButtonText.setString("Zastosuj"); m_applyButtonText.setFont(m_font); m_applyButtonText.setCharacterSize(24);
    m_applyButtonText.setPosition(m_applyButton.getPosition().x + (m_applyButton.getSize().x - m_applyButtonText.getLocalBounds().width)/2.f, m_applyButton.getPosition().y + 10.f);
    m_cancelButton.setSize({200.f, 50.f}); m_cancelButton.setFillColor(sf::Color(180, 80, 80));
    m_cancelButton.setPosition(m_optionsPanel.getPosition().x + m_optionsPanel.getSize().x - 240.f, m_optionsPanel.getPosition().y + 320.f);
    m_cancelButtonText.setString("Anuluj"); m_cancelButtonText.setFont(m_font); m_cancelButtonText.setCharacterSize(24);
    m_cancelButtonText.setPosition(m_cancelButton.getPosition().x + (m_cancelButton.getSize().x - m_cancelButtonText.getLocalBounds().width)/2.f, m_cancelButton.getPosition().y + 10.f);

    reset();
}

void Game::reset() {
    currentMoney = 1000.f;
    currentEnergy = 0.f;
    maxEnergy = 100.f;
    environmentHealth = 100.f;
    totalGameTimeSeconds = 0.f;
    placedObjects.clear();
    m_buildMenu.setVisible(false);
    m_hammerPressed = false;
    currentSaveName.clear();
}

void Game::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState) {
    if (currentState == GameState::Playing) {
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::P) {
            currentState = GameState::PauseMenu;
            return;
        }

        if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
            if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    if (m_playableArea.contains(pos)) {
                        placeBuilding(drag->itemId, drag->price, pos);
                        m_buildMenu.cancelDragging();
                    }
                } else if (ev.mouseButton.button == sf::Mouse::Right) {
                    m_buildMenu.cancelDragging();
                }
            }
        } else {
            if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                if (m_hammerHotspot.contains(pos)) {
                    m_hammerPressed = !m_hammerPressed;
                    m_buildMenu.setVisible(m_hammerPressed);
                } else if (m_optionsIconHotspot.contains(pos)) {
                    currentState = GameState::InGameOptionsMenu;
                } else if (m_pauseIconHotspot.contains(pos)) {
                    currentState = GameState::PauseMenu;
                } else if (m_hud.getEnergySellButtonBounds().contains(pos)) {
                    currentState = GameState::EnergyMenu;
                }
            }
            if (m_buildMenu.isVisible()) {
                m_buildMenu.handleEvent(ev, window, currentMoney);
            }
        }

        if (ev.type == sf::Event::MouseMoved) {
            if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
                drag->sprite.setPosition(window.mapPixelToCoords({ev.mouseMove.x, ev.mouseMove.y}));
            }
        }
    }
    else if (currentState == GameState::EnergyMenu) {
        if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) {
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_sellButton.getGlobalBounds().contains(pos)) {
                currentMoney += currentEnergy * GameConstants::ENERGY_SELL_PRICE;
                currentEnergy = 0;
                currentState = GameState::Playing;
            } else if (m_closeButton.getGlobalBounds().contains(pos)) {
                currentState = GameState::Playing;
            }
        }
    }
    // ZMIANA: Logika obsługi menu opcji w grze
    else if (currentState == GameState::InGameOptionsMenu) {
        m_musicSlider->handleEvent(ev, window);
        m_sfxSlider->handleEvent(ev, window);

        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
            m_musicSlider->setValue(m_savedMusicValue);
            m_sfxSlider->setValue(m_savedSfxValue);
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});

            if (m_applyButton.getGlobalBounds().contains(pos)) {
                m_savedMusicValue = m_musicSlider->getValue();
                m_savedSfxValue = m_sfxSlider->getValue();
                std::cout << "Zastosowano nowe ustawienia dzwieku!\n";
                currentState = GameState::Playing;
            }
            else if (m_cancelButton.getGlobalBounds().contains(pos)) {
                m_musicSlider->setValue(m_savedMusicValue);
                m_sfxSlider->setValue(m_savedSfxValue);
                currentState = GameState::Playing;
            }
        }
    }
}

void Game::update(float dt) {
    totalGameTimeSeconds += dt;
    if (maxEnergy > 0) {
        for (auto& obj : placedObjects) {
            if(obj.logic) {
                obj.logic->update(dt, *this);
            }
        }
    }
    currentEnergy = std::min(currentEnergy, maxEnergy);
    environmentHealth = std::min(environmentHealth, 100.f);
    m_hud.update(currentMoney, currentEnergy, maxEnergy, environmentHealth, totalGameTimeSeconds);
}

void Game::draw(sf::RenderWindow& window) {
    m_gameBg.draw(window);
    window.draw(m_grassArea);
    for (const auto& obj : placedObjects) {
        window.draw(obj.sprite);
    }
    m_hud.draw(window);
    if (m_hammerPressed) {
        m_buildMenu.draw(window);
    }
    if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
        window.draw(drag->sprite);
    }
}

void Game::drawForPause(sf::RenderWindow& window) {
    draw(window);
}

void Game::placeBuilding(int typeId, float price, sf::Vector2f position) {
    PlacedObject newObj;
    newObj.typeId = typeId;
    newObj.logic = createBuildingById(typeId);
    if (!newObj.logic) return;
    newObj.sprite.setTexture(m_buildingTextures[typeId]);
    auto texSize = m_buildingTextures[typeId].getSize();
    newObj.sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
    newObj.sprite.setPosition(position);
    currentMoney -= price;
    if (typeId == GameConstants::ENERGY_STORAGE_ID) {
        maxEnergy += GameConstants::ENERGY_STORAGE_CAPACITY_INCREASE;
    }
    placedObjects.push_back(std::move(newObj));
}

void Game::updateEnergyMenu() {
    std::ostringstream energyInfo, energyGain;
    energyInfo << "Zgromadzona energia: " << static_cast<int>(currentEnergy) << " / " << static_cast<int>(maxEnergy);
    m_energyInfoText.setString(energyInfo.str());
    energyGain << "Wartosc sprzedazy: " << std::fixed << std::setprecision(2) << (currentEnergy * GameConstants::ENERGY_SELL_PRICE) << "$";
    m_energyValueText.setString(energyGain.str());
}

void Game::drawEnergyMenu(sf::RenderWindow& window) {
    draw(window);
    window.draw(m_energyPanel);
    window.draw(m_energyMenuTitle);
    window.draw(m_energyInfoText);
    window.draw(m_energyValueText);
    window.draw(m_sellButton);
    window.draw(m_sellButtonText);
    window.draw(m_closeButton);
    window.draw(m_closeButtonText);
}

void Game::drawOptionsMenu(sf::RenderWindow& window) {
    draw(window);

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
