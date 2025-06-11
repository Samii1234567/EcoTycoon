#include "Game.h"
#include "Constants.h"
#include <iostream>

Game::Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures, AudioManager& audioManager)
    : m_font(font),
    m_buildingTextures(buildingTextures),
    m_audioManager(audioManager),
    m_uiManager(font, *this),
    m_hud(font)
{
    musicVolume = 0.5f;
    sfxVolume = 0.5f;
    m_gameBg.loadFromFile("images/eco_tycoon_game.png");
    m_buildMenu.initialize(m_font, buildingTextures);
    m_tooltip.initialize(m_font);

    m_grassArea.setSize({1200.f, 600.f});
    m_grassArea.setPosition(0.f, 121.f);
    if (!m_grassTexture.loadFromFile("images/eco_grass.png")) { std::cerr << "Blad wczytywania trawy\n"; }
    m_grassArea.setTexture(&m_grassTexture);
    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);

    m_buildModeOverlay.setSize({1200.f, 800.f});
    m_buildModeOverlay.setFillColor(sf::Color(0, 0, 0, 80));
    if (!m_repairIconTexture.loadFromFile("images/repair_icon.png")) { std::cerr << "Nie mozna wczytac repair_icon.png!\n"; }
    m_repairIconSprite.setTexture(m_repairIconTexture);

    // ##### ZMIANA: Usunięto statyczne skalowanie ikony naprawy #####
    // m_repairIconSprite.setScale(0.5f, 0.5f); // Usunięto
    m_repairIconSprite.setOrigin(m_repairIconTexture.getSize().x / 2.f, m_repairIconTexture.getSize().y / 2.f);
    // ###############################################################

    if (!m_bulldozerTexture.loadFromFile("images/bulldozer.png")) { std::cerr << "Nie mozna wczytac bulldozer.png\n"; }
    m_bulldozerSprite.setTexture(m_bulldozerTexture);
    sf::Vector2u texSize = m_bulldozerTexture.getSize();
    m_bulldozerSprite.setScale(m_demolishHotspot.width / texSize.x, m_demolishHotspot.height / texSize.y);
    m_bulldozerSprite.setPosition(m_demolishHotspot.left, m_demolishHotspot.top);
    m_demolishCancelText.setFont(m_font);
    m_demolishCancelText.setString("[ESC] Anuluj");
    m_demolishCancelText.setCharacterSize(24);
    m_demolishCancelText.setFillColor(sf::Color::White);
    m_demolishCancelText.setOutlineColor(sf::Color::Black);
    m_demolishCancelText.setOutlineThickness(2.f);
    sf::FloatRect textBounds = m_demolishCancelText.getLocalBounds();
    m_demolishCancelText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_demolishCancelText.setPosition(1200 / 2.f, 30.f);

    reset();
}

// ... (reset, handleEvent, update, draw - bez zmian, aż do drawDamagedIcons) ...
void Game::reset() {
    currentMoney = 1000;
    currentEnergy = 0;
    maxEnergy = 100;
    environmentHealth = 100.f;
    totalGameTimeSeconds = 0.f;
    placedObjects.clear();
    currentSaveName.clear();

    m_buildMenu.setVisible(false);
    m_hammerPressed = false;
    m_demolishModeActive = false;
    m_isBuildMode = false;
    m_hoveredBuilding = nullptr;
    m_tooltip.hide();

    m_repairTarget = nullptr;

    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);
    m_weatherManager.reset();
}

void Game::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState) {
    if (m_repairTarget) {
        m_uiManager.handleEvent(ev, window, currentState);
        return;
    }

    m_uiManager.handleEvent(ev, window, currentState);
    if (currentState != GameState::Playing) return;

    if (m_tooltip.isVisible() && !m_demolishModeActive) {
        m_tooltip.handleEvent(ev, window);
        if (m_tooltip.isUpgradeClicked() && m_hoveredBuilding) {
            upgradeBuilding(*m_hoveredBuilding);
        }
    }

    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::P) { currentState = GameState::PauseMenu; return; }
        if (ev.key.code == sf::Keyboard::Escape) {
            if (m_demolishModeActive) { m_demolishModeActive = false; }
            if (m_isBuildMode) {
                m_buildMenu.cancelDragging();
                m_isBuildMode = false;
                m_hammerPressed = false;
            }
        }
    }

    if (ev.type == sf::Event::MouseMoved) {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseMove.x, ev.mouseMove.y});

        if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
            drag->sprite.setPosition(pos);
        }
        else if (!m_demolishModeActive) {
            if (m_buildMenu.isVisible() && m_buildMenu.getBackgroundBounds().contains(pos)) {
                m_tooltip.hide();
                m_hoveredBuilding = nullptr;
            } else {
                m_hoveredBuilding = nullptr;
                for (auto& building : placedObjects) {
                    if (building.sprite.getGlobalBounds().contains(pos)) {
                        m_hoveredBuilding = &building;
                        break;
                    }
                }
                if (m_hoveredBuilding && !m_hoveredBuilding->isDamaged) {
                    m_tooltip.show(*m_hoveredBuilding, currentMoney);
                } else {
                    m_tooltip.hide();
                }
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
                m_hammerPressed = false;
            } else if (ev.mouseButton.button == sf::Mouse::Right) {
                m_buildMenu.cancelDragging();
                m_isBuildMode = false;
                m_hammerPressed = false;
            }
        }
    } else {
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});

            bool uiClicked = false;
            if (m_hammerHotspot.contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); m_hammerPressed = !m_hammerPressed; m_buildMenu.setVisible(m_hammerPressed); }
            else if (m_demolishHotspot.contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); m_demolishModeActive = !m_demolishModeActive; if (m_hammerPressed) { m_hammerPressed = false; m_buildMenu.setVisible(false); } }
            else if (m_optionsIconHotspot.contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); m_uiManager.syncSliderValues(); currentState = GameState::InGameOptionsMenu; }
            else if (m_pauseIconHotspot.contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); currentState = GameState::PauseMenu; }
            else if (m_hud.getEnergySellButtonBounds().contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); currentState = GameState::EnergyMenu; }

            if (uiClicked) return;

            if (m_demolishModeActive) {
                for (auto it = placedObjects.begin(); it != placedObjects.end(); ++it) {
                    if (it->sprite.getGlobalBounds().contains(pos)) {
                        currentMoney += it->price / 2;
                        if (it->typeId == GameConstants::ENERGY_STORAGE_ID) {
                            maxEnergy -= GameConstants::STORAGE_DATA.value[it->level - 1];
                            if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;
                        }
                        m_grid.freeArea(it->gridPosition, it->sizeInCells);
                        placedObjects.erase(it);
                        m_audioManager.play(SoundEffect::BuildingSell);
                        m_demolishModeActive = false;
                        return;
                    }
                }
            } else {
                for (auto& obj : placedObjects) {
                    if (obj.isDamaged && obj.sprite.getGlobalBounds().contains(pos)) {
                        setRepairTarget(&obj);
                        return;
                    }
                }
            }

            for (auto it = placedObjects.rbegin(); it != placedObjects.rend(); ++it) {
                if (it->sprite.getGlobalBounds().contains(pos) && (!m_tooltip.isVisible() || !m_tooltip.isUpgradeClicked())) {
                    switch (it->typeId) {
                    case GameConstants::SOLAR_PANEL_ID: m_audioManager.play(SoundEffect::SolarPanel); break;
                    case GameConstants::WIND_TURBINE_ID: m_audioManager.play(SoundEffect::WindTurbine); break;
                    case GameConstants::ENERGY_STORAGE_ID: m_audioManager.play(SoundEffect::EnergyStorage); break;
                    }
                    break;
                }
            }
        }

        if (m_buildMenu.isVisible()) {
            if (m_buildMenu.handleEvent(ev, window, currentMoney) == BuildMenu::ClickResult::NotEnoughMoney) {
                m_uiManager.showNotification("Brak wystarczajacych srodkow");
            }
        }
    }
}

void Game::update(float dt) {
    totalGameTimeSeconds += dt;

    m_weatherManager.update(dt, environmentHealth, placedObjects,
                            [this](const std::string& msg){ this->m_uiManager.showNotification(msg); });

    switch(m_weatherManager.getCurrentWeather()) {
    case WeatherType::Rain: environmentHealth += 0.05f * dt; break;
    case WeatherType::AcidRain: environmentHealth -= 0.2f * dt; break;
    case WeatherType::Smog: environmentHealth -= 0.5f * dt; break;
    default: break;
    }
    if (environmentHealth < 0) environmentHealth = 0;
    if (environmentHealth > 100) environmentHealth = 100;

    for (auto& obj : placedObjects) {
        if(obj.logic && !obj.isDamaged) {
            obj.logic->update(dt, *this, obj);
            sf::IntRect textureRect = obj.logic->getTextureRect();
            if (textureRect.width != 0) {
                obj.sprite.setTextureRect(textureRect);
            }
        }
    }

    if (m_demolishModeActive) m_bulldozerSprite.setColor(sf::Color(180, 255, 180));
    else m_bulldozerSprite.setColor(sf::Color::White);

    if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;

    m_hud.update(currentMoney, currentEnergy, maxEnergy, environmentHealth, totalGameTimeSeconds);
    m_uiManager.update(m_weatherManager);
}

void Game::draw(sf::RenderWindow& window, GameState currentState) {
    m_gameBg.draw(window);
    window.draw(m_bulldozerSprite);
    window.draw(m_grassArea);

    for (const auto& obj : placedObjects) {
        window.draw(obj.sprite);
    }
    drawDamagedIcons(window);

    if (m_isBuildMode) {
        window.draw(m_buildModeOverlay);
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (auto& drag = m_buildMenu.getDragState(); drag.has_value()) {
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

    m_uiManager.draw(window, currentState);
}

void Game::drawDamagedIcons(sf::RenderWindow& window) {
    for (const auto& obj : placedObjects) {
        if (obj.isDamaged) {
            // ##### ZMIANA: Dynamiczne skalowanie ikony naprawy #####
            const sf::FloatRect buildingBounds = obj.sprite.getGlobalBounds();
            const sf::Vector2u textureSize = m_repairIconTexture.getSize();

            if (textureSize.x > 0 && textureSize.y > 0) {
                float scaleX = buildingBounds.width / textureSize.x;
                float scaleY = buildingBounds.height / textureSize.y;
                m_repairIconSprite.setScale(scaleX, scaleY);
            }

            m_repairIconSprite.setPosition(obj.sprite.getPosition());
            window.draw(m_repairIconSprite);
            // #######################################################
        }
    }
}

// ... (reszta pliku bez zmian) ...

void Game::setRepairTarget(PlacedObject* target) {
    m_repairTarget = target;
}
PlacedObject* Game::getRepairTarget() const {
    return m_repairTarget;
}
void Game::confirmRepair() {
    if (!m_repairTarget) return;

    int repairCost = m_repairTarget->price / 4;
    if (currentMoney >= repairCost) {
        currentMoney -= repairCost;
        m_repairTarget->isDamaged = false;
        m_repairTarget->sprite.setColor(sf::Color::White);
        m_uiManager.showNotification("Naprawiono budynek!");
        m_audioManager.play(SoundEffect::Repair);
    } else {
        m_uiManager.showNotification("Brak srodkow na naprawe!");
    }
    m_repairTarget = nullptr;
}
void Game::upgradeBuilding(PlacedObject& building) {
    if (building.level >= GameConstants::MAX_LEVEL) return;
    int cost = 0;
    const GameConstants::BuildingLevelData* data = nullptr;

    switch (building.typeId) {
    case GameConstants::SOLAR_PANEL_ID: data = &GameConstants::SOLAR_PANEL_DATA; break;
    case GameConstants::WIND_TURBINE_ID: data = &GameConstants::WIND_TURBINE_DATA; break;
    case GameConstants::ENERGY_STORAGE_ID: data = &GameConstants::STORAGE_DATA; break;
    case GameConstants::AIR_FILTER_ID: data = &GameConstants::AIR_FILTER_DATA; break;
    }
    if (data) cost = data->upgradeCost[building.level];

    if (currentMoney >= cost) {
        currentMoney -= cost;
        if (building.typeId == GameConstants::ENERGY_STORAGE_ID) {
            maxEnergy -= GameConstants::STORAGE_DATA.value[building.level - 1];
            maxEnergy += GameConstants::STORAGE_DATA.value[building.level];
        }
        building.level++;
        m_audioManager.play(SoundEffect::Upgrade);
        m_tooltip.show(building, currentMoney);
    } else {
        m_uiManager.showNotification("Brak wystarczajacych srodkow");
    }
}
void Game::placeBuilding(int typeId, int price, sf::Vector2f position, bool fromPlayerAction) {
    sf::Vector2i gridPos = m_grid.worldToGridCoords(position);
    sf::Vector2i buildingSize = getBuildingSize(typeId);

    if (fromPlayerAction && !m_grid.isAreaFree(gridPos, buildingSize)) {
        m_uiManager.showNotification("To miejsce jest juz zajete!");
        return;
    }

    PlacedObject newObj;
    newObj.typeId = typeId;
    newObj.logic = createBuildingById(typeId);
    newObj.price = price;
    newObj.level = 1;
    newObj.gridPosition = gridPos;
    newObj.sizeInCells = buildingSize;
    newObj.isDamaged = false;

    if (!newObj.logic) return;

    const auto& texture = m_buildingTextures[typeId];
    newObj.sprite.setTexture(texture);

    if (typeId == GameConstants::WIND_TURBINE_ID) {
        newObj.sprite.setTextureRect(sf::IntRect(0, 0, GameConstants::TURBINE_FRAME_WIDTH, GameConstants::TURBINE_FRAME_HEIGHT));
        float scale = (buildingSize.x * GameConstants::GRID_CELL_SIZE) / GameConstants::TURBINE_FRAME_WIDTH;
        newObj.sprite.setScale(scale, scale);
        newObj.sprite.setOrigin(GameConstants::TURBINE_FRAME_WIDTH / 2.f, GameConstants::TURBINE_FRAME_HEIGHT);
        sf::Vector2f basePosition = m_grid.gridToWorldCoords(gridPos);
        basePosition.x += (buildingSize.x * GameConstants::GRID_CELL_SIZE) / 2.f;
        basePosition.y += (buildingSize.y * GameConstants::GRID_CELL_SIZE);
        newObj.sprite.setPosition(basePosition);
    } else {
        float sourceWidth = static_cast<float>(texture.getSize().x);
        float sourceHeight = static_cast<float>(texture.getSize().y);
        sf::Vector2f targetSize(buildingSize.x * GameConstants::GRID_CELL_SIZE, buildingSize.y * GameConstants::GRID_CELL_SIZE);
        newObj.sprite.setScale(targetSize.x / sourceWidth, targetSize.y / sourceHeight);
        newObj.sprite.setOrigin(sourceWidth / 2.f, sourceHeight / 2.f);
        sf::Vector2f finalPosition = m_grid.gridToWorldCoords(gridPos);
        finalPosition.x += targetSize.x / 2.0f;
        finalPosition.y += targetSize.y / 2.0f;
        newObj.sprite.setPosition(finalPosition);
    }

    if (fromPlayerAction) {
        currentMoney -= price;
        if (typeId == GameConstants::ENERGY_STORAGE_ID) {
            maxEnergy += GameConstants::STORAGE_DATA.value[0];
        }
        m_audioManager.play(SoundEffect::BuildingPlace);
    }

    m_grid.occupyArea(gridPos, buildingSize);
    placedObjects.push_back(std::move(newObj));
}
Grid& Game::getGrid() { return m_grid; }
HUD& Game::getHUD() { return m_hud; }
AudioManager& Game::getAudioManager() { return m_audioManager; }
WeatherManager& Game::getWeatherManager() { return m_weatherManager; }
