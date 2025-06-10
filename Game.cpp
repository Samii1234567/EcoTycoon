#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

// Implementacja klasy BuildingTooltip została przeniesiona do Game.h
// Implementacja klasy Grid została przeniesiona do Grid.cpp

Game::Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures)
    : m_font(font),
    m_buildingTextures(buildingTextures),
    m_hud(font)
{
    musicVolume = 0.5f;
    sfxVolume = 0.5f;

    m_gameBg.loadFromFile("images/eco_tycoon_game.png");
    m_buildMenu.initialize(m_font, buildingTextures);

    m_grassArea.setSize({1200.f, 600.f});
    m_grassArea.setPosition(0.f, 121.f);
    if (!m_grassTexture.loadFromFile("images/eco_grass.png")) { std::cerr << "Blad wczytywania trawy\n"; }
    m_grassArea.setTexture(&m_grassTexture);

    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);

    m_buildModeOverlay.setSize({1200.f, 800.f});
    m_buildModeOverlay.setFillColor(sf::Color(0, 0, 0, 80));

    if (!m_bulldozerTexture.loadFromFile("images/bulldozer.png")) { std::cerr << "Nie mozna wczytac bulldozer.png\n"; }
    m_bulldozerSprite.setTexture(m_bulldozerTexture);
    sf::Vector2u texSize = m_bulldozerTexture.getSize();
    m_bulldozerSprite.setScale(m_demolishHotspot.width / texSize.x, m_demolishHotspot.height / texSize.y);
    m_bulldozerSprite.setPosition(m_demolishHotspot.left, m_demolishHotspot.top);

    if (!m_solarPanelBuffer.loadFromFile("audio/solar_panel.wav")) { std::cerr << "Nie mozna wczytac solar_panel.wav\n"; }
    m_solarPanelSound.setBuffer(m_solarPanelBuffer);
    if (!m_windTurbineBuffer.loadFromFile("audio/wind_turbine.wav")) { std::cerr << "Nie mozna wczytac wind_turbine.wav\n"; }
    m_windTurbineSound.setBuffer(m_windTurbineBuffer);
    if (!m_energyStorageBuffer.loadFromFile("audio/energy_magazine.wav")) { std::cerr << "Nie mozna wczytac energy_magazine.wav\n"; }
    m_energyStorageSound.setBuffer(m_energyStorageBuffer);
    if (!m_upgradeBuffer.loadFromFile("audio/upgrade_sound.wav")) { std::cerr << "Nie mozna wczytac upgrade_sound.wav\n"; }
    m_upgradeSound.setBuffer(m_upgradeBuffer);

    const unsigned WIN_W = 1200, WIN_H = 800;

    m_demolishCancelText.setFont(m_font);
    m_demolishCancelText.setString("[ESC] Anuluj");
    m_demolishCancelText.setCharacterSize(24);
    m_demolishCancelText.setFillColor(sf::Color::White);
    m_demolishCancelText.setOutlineColor(sf::Color::Black);
    m_demolishCancelText.setOutlineThickness(2.f);
    sf::FloatRect textBounds = m_demolishCancelText.getLocalBounds();
    m_demolishCancelText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_demolishCancelText.setPosition(WIN_W / 2.f, 30.f);

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

    m_tooltip.initialize(m_font);

    m_optionsPanel.setSize({500.f, 400.f});
    m_optionsPanel.setFillColor({40, 40, 60, 220});
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
    m_musicSlider->setValue(musicVolume);
    m_sfxSlider->setValue(sfxVolume);
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
    currentMoney = 1000;
    currentEnergy = 0;
    maxEnergy = 100;
    environmentHealth = 100.f;
    totalGameTimeSeconds = 0.f;
    placedObjects.clear();
    m_buildMenu.setVisible(false);
    m_hammerPressed = false;
    m_demolishModeActive = false;
    m_isBuildMode = false;
    m_hoveredBuilding = nullptr;
    m_tooltip.hide();
    currentSaveName.clear();
    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);
}

void Game::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState, sf::Sound& clickSound) {
    if (m_tooltip.isVisible() && !m_demolishModeActive) {
        m_tooltip.handleEvent(ev, window);
        if (m_tooltip.isUpgradeClicked() && m_hoveredBuilding) {
            upgradeBuilding(*m_hoveredBuilding);
        }
    }

    if (currentState == GameState::Playing) {
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::P) { currentState = GameState::PauseMenu; return; }
            if (ev.key.code == sf::Keyboard::Escape) {
                if (m_demolishModeActive) { m_demolishModeActive = false; }
                if (m_isBuildMode) { m_buildMenu.cancelDragging(); m_isBuildMode = false; }
            }
        }

        if (ev.type == sf::Event::MouseMoved) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseMove.x, ev.mouseMove.y});

            if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
                drag->sprite.setPosition(pos);
            }
            else if (!m_demolishModeActive) {
                m_hoveredBuilding = nullptr;
                for (auto& building : placedObjects) {
                    if (building.sprite.getGlobalBounds().contains(pos)) {
                        m_hoveredBuilding = &building;
                        break;
                    }
                }
                if (m_hoveredBuilding) {
                    m_tooltip.show(*m_hoveredBuilding, currentMoney);
                } else {
                    m_tooltip.hide();
                }
            }
        }

        if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
            m_isBuildMode = true;
            if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    if (m_playableArea.contains(pos)) {
                        placeBuilding(drag->itemId, drag->price, pos);
                    }
                    m_buildMenu.cancelDragging();
                    m_isBuildMode = false;
                } else if (ev.mouseButton.button == sf::Mouse::Right) {
                    m_buildMenu.cancelDragging();
                    m_isBuildMode = false;
                }
            }
        } else {
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});

                // ZMIANA: Przebudowa logiki kliknięć
                // 1. Kliknięcie na przycisk ulepszenia w tooltipie jest już obsłużone na górze funkcji
                if (m_tooltip.isVisible() && m_tooltip.isUpgradeClicked()) {
                    // Akcja już wykonana, nie rób nic więcej
                }
                // 2. Obsługa trybu burzenia
                else if (m_demolishModeActive) {
                    for (auto it = placedObjects.begin(); it != placedObjects.end(); ++it) {
                        if (it->sprite.getGlobalBounds().contains(pos)) {
                            currentMoney += it->price / 2;
                            if (it->typeId == GameConstants::ENERGY_STORAGE_ID) {
                                maxEnergy -= GameConstants::STORAGE_DATA.value[it->level - 1];
                                if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;
                            }
                            m_grid.freeArea(it->gridPosition, it->sizeInCells);
                            placedObjects.erase(it);
                            clickSound.play();
                            m_demolishModeActive = false;
                            break;
                        }
                    }
                }
                // 3. Obsługa przycisków UI i kliknięć na budynki
                else {
                    bool uiClicked = false;
                    if (m_hammerHotspot.contains(pos)) {
                        clickSound.play();
                        m_hammerPressed = !m_hammerPressed;
                        m_buildMenu.setVisible(m_hammerPressed);
                        uiClicked = true;
                    } else if (m_demolishHotspot.contains(pos)) {
                        clickSound.play();
                        m_demolishModeActive = !m_demolishModeActive;
                        if (m_hammerPressed) { m_hammerPressed = false; m_buildMenu.setVisible(false); }
                        uiClicked = true;
                    } else if (m_optionsIconHotspot.contains(pos)) {
                        clickSound.play();
                        m_musicSlider->setValue(musicVolume);
                        m_sfxSlider->setValue(sfxVolume);
                        currentState = GameState::InGameOptionsMenu;
                        uiClicked = true;
                    } else if (m_pauseIconHotspot.contains(pos)) {
                        clickSound.play();
                        currentState = GameState::PauseMenu;
                        uiClicked = true;
                    } else if (m_hud.getEnergySellButtonBounds().contains(pos)) {
                        clickSound.play();
                        currentState = GameState::EnergyMenu;
                        uiClicked = true;
                    }

                    // Jeśli nie kliknięto żadnego przycisku UI, sprawdź kliknięcie na budynek
                    if (!uiClicked) {
                        for (auto it = placedObjects.rbegin(); it != placedObjects.rend(); ++it) {
                            if (it->sprite.getGlobalBounds().contains(pos)) {
                                // Odtwarzaj dźwięk tylko jeśli tooltip nie jest widoczny LUB jeśli kliknięcie nie było na jego przycisku
                                if (!m_tooltip.isVisible() || !m_tooltip.isUpgradeClicked()) {
                                    switch (it->typeId) {
                                    case GameConstants::SOLAR_PANEL_ID: m_solarPanelSound.play(); break;
                                    case GameConstants::WIND_TURBINE_ID: m_windTurbineSound.play(); break;
                                    case GameConstants::ENERGY_STORAGE_ID: m_energyStorageSound.play(); break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
            if (m_buildMenu.isVisible()) {
                m_buildMenu.handleEvent(ev, window, currentMoney);
            }
        }
    }
    else if (currentState == GameState::EnergyMenu) {
        if (m_demolishModeActive) { m_demolishModeActive = false; }
        if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) {
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_sellButton.getGlobalBounds().contains(pos)) {
                clickSound.play();
                currentMoney += currentEnergy * GameConstants::ENERGY_SELL_PRICE;
                currentEnergy = 0;
                currentState = GameState::Playing;
            } else if (m_closeButton.getGlobalBounds().contains(pos)) {
                clickSound.play();
                currentState = GameState::Playing;
            }
        }
    }
    else if (currentState == GameState::InGameOptionsMenu) {
        if (m_demolishModeActive) { m_demolishModeActive = false; }
        m_musicSlider->handleEvent(ev, window);
        m_sfxSlider->handleEvent(ev, window);

        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
            m_musicSlider->setValue(musicVolume);
            m_sfxSlider->setValue(sfxVolume);
            currentState = GameState::Playing;
        }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_applyButton.getGlobalBounds().contains(pos)) {
                clickSound.play();
                musicVolume = m_musicSlider->getValue();
                sfxVolume = m_sfxSlider->getValue();
                std::cout << "Zastosowano nowe ustawienia dzwieku w grze!\n";
                currentState = GameState::Playing;
            }
            else if (m_cancelButton.getGlobalBounds().contains(pos)) {
                clickSound.play();
                m_musicSlider->setValue(musicVolume);
                m_sfxSlider->setValue(sfxVolume);
                currentState = GameState::Playing;
            }
        }
    }
}

void Game::upgradeBuilding(PlacedObject& building) {
    if (building.level >= GameConstants::MAX_LEVEL) return;

    int cost = 0;
    const GameConstants::BuildingLevelData* data = nullptr;

    switch (building.typeId) {
    case GameConstants::SOLAR_PANEL_ID: data = &GameConstants::SOLAR_PANEL_DATA; break;
    case GameConstants::WIND_TURBINE_ID: data = &GameConstants::WIND_TURBINE_DATA; break;
    case GameConstants::ENERGY_STORAGE_ID: data = &GameConstants::STORAGE_DATA; break;
    }

    if(data) {
        cost = data->upgradeCost[building.level];
    }

    if (currentMoney >= cost) {
        currentMoney -= cost;

        if (building.typeId == GameConstants::ENERGY_STORAGE_ID) {
            maxEnergy -= GameConstants::STORAGE_DATA.value[building.level - 1];
            maxEnergy += GameConstants::STORAGE_DATA.value[building.level];
        }

        building.level++;
        m_upgradeSound.play();
        m_tooltip.show(building, currentMoney);
    } else {
        std::cout << "Za malo pieniedzy na ulepszenie!\n";
    }
}

void Game::update(float dt) {
    totalGameTimeSeconds += dt;
    for (auto& obj : placedObjects) {
        if(obj.logic) {
            obj.logic->update(dt, *this, obj);
            sf::IntRect textureRect = obj.logic->getTextureRect();
            if (textureRect.width != 0) {
                obj.sprite.setTextureRect(textureRect);
            }
        }
    }

    if (m_demolishModeActive) m_bulldozerSprite.setColor(sf::Color(180, 255, 180));
    else m_bulldozerSprite.setColor(sf::Color::White);

    // ZMIANA: Ściszenie dźwięków budynków
    float buildingSoundVolume = (this->sfxVolume * 100.f) / 2.f;
    m_solarPanelSound.setVolume(buildingSoundVolume);
    m_windTurbineSound.setVolume(buildingSoundVolume);
    m_energyStorageSound.setVolume(buildingSoundVolume);
    m_upgradeSound.setVolume(this->sfxVolume * 100.f);

    if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;
    if (environmentHealth > 100.f) environmentHealth = 100.f;
    m_hud.update(currentMoney, currentEnergy, maxEnergy, environmentHealth, totalGameTimeSeconds);
}

void Game::draw(sf::RenderWindow& window) {
    m_gameBg.draw(window);
    window.draw(m_bulldozerSprite);
    window.draw(m_grassArea);
    for (const auto& obj : placedObjects) {
        window.draw(obj.sprite);
    }

    if (m_isBuildMode) {
        window.draw(m_buildModeOverlay);
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        auto& drag = m_buildMenu.getDragState();
        if (drag.has_value()) {
            m_grid.draw(window, mousePos, getBuildingSize(drag->itemId));
        }
    }

    m_hud.draw(window);
    if (m_hammerPressed) {
        m_buildMenu.draw(window);
    }
    if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
        window.draw(drag->sprite);
    }
    if (m_demolishModeActive) {
        window.draw(m_demolishCancelText);
    }
    m_tooltip.draw(window);
}

void Game::drawForPause(sf::RenderWindow& window) {
    draw(window);
}

void Game::placeBuilding(int typeId, int price, sf::Vector2f position, bool fromPlayerAction) {
    sf::Vector2i gridPos = m_grid.worldToGridCoords(position);
    sf::Vector2i buildingSize = getBuildingSize(typeId);

    if (fromPlayerAction && !m_grid.isAreaFree(gridPos, buildingSize)) {
        std::cout << "Miejsce zajete!\n";
        return;
    }

    PlacedObject newObj;
    newObj.typeId = typeId;
    newObj.logic = createBuildingById(typeId);
    newObj.price = price;
    newObj.level = 1;
    newObj.gridPosition = gridPos;
    newObj.sizeInCells = buildingSize;

    if (!newObj.logic) return;

    const auto& texture = m_buildingTextures[typeId];
    newObj.sprite.setTexture(texture);

    sf::IntRect textureRect = newObj.logic->getTextureRect();
    float sourceWidth = (textureRect.width != 0) ? static_cast<float>(textureRect.width) : static_cast<float>(texture.getSize().x);
    float sourceHeight = (textureRect.height != 0) ? static_cast<float>(textureRect.height) : static_cast<float>(texture.getSize().y);

    sf::Vector2f targetSize(buildingSize.x * GameConstants::GRID_CELL_SIZE, buildingSize.y * GameConstants::GRID_CELL_SIZE);
    newObj.sprite.setScale(targetSize.x / sourceWidth, targetSize.y / sourceHeight);
    newObj.sprite.setOrigin(sourceWidth / 2.f, sourceHeight / 2.f);

    sf::Vector2f finalPosition = m_grid.gridToWorldCoords(gridPos);
    finalPosition.x += targetSize.x / 2.0f;
    finalPosition.y += targetSize.y / 2.0f;
    newObj.sprite.setPosition(finalPosition);

    if (fromPlayerAction) {
        currentMoney -= price;
        if (typeId == GameConstants::ENERGY_STORAGE_ID) {
            maxEnergy += GameConstants::STORAGE_DATA.value[0];
        }
    }

    m_grid.occupyArea(gridPos, buildingSize);
    placedObjects.push_back(std::move(newObj));
}

void Game::updateEnergyMenu() {
    std::ostringstream oss;
    oss << "Zgromadzona energia: " << currentEnergy << " / " << maxEnergy;
    m_energyInfoText.setString(oss.str());
    oss.str("");
    oss.clear();
    oss << "Wartosc sprzedazy: " << (currentEnergy * GameConstants::ENERGY_SELL_PRICE) << "$";
    m_energyValueText.setString(oss.str());
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
