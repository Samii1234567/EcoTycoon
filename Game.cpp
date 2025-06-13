#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA GŁÓWNEJ KLASY GRY
//
//  Zawiera implementację całej logiki zadeklarowanej w Game.h.
//  To tutaj dzieje się "magia" gry - wszystkie systemy są inicjalizowane,
//  aktualizowane i rysowane.
//
// ===================================================================================

// Implementacja konstruktora.
Game::Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures, AudioManager& audioManager)
    : m_font(font),
    m_buildingTextures(buildingTextures),
    m_audioManager(audioManager),
    m_uiManager(font, *this),
    m_hud(font)
{
    // --- Inicjalizacja podstawowych ustawień i komponentów ---
    musicVolume = 0.5f;
    sfxVolume = 0.5f;
    m_gameBg.loadFromFile("images/eco_tycoon_game.png");
    m_buildMenu.initialize(m_font, buildingTextures);
    m_tooltip.initialize(m_font);

    // --- Inicjalizacja obszaru gry (trawa i siatka) ---
    m_grassArea.setSize({1200.f, 600.f});
    m_grassArea.setPosition(0.f, 121.f);
    if (!m_grassTexture.loadFromFile("images/eco_grass.png")) { std::cerr << "Blad wczytywania trawy\n"; }
    m_grassArea.setTexture(&m_grassTexture);
    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);

    // --- Inicjalizacja elementów UI w świecie gry ---
    m_buildModeOverlay.setSize({1200.f, 800.f});
    m_buildModeOverlay.setFillColor(sf::Color(0, 0, 0, 80));
    if (!m_repairIconTexture.loadFromFile("images/repair_icon.png")) { std::cerr << "Nie mozna wczytac repair_icon.png!\n"; }
    m_repairIconSprite.setTexture(m_repairIconTexture);
    m_repairIconSprite.setOrigin(m_repairIconTexture.getSize().x / 2.f, m_repairIconTexture.getSize().y / 2.f);

    if (!m_bulldozerTexture.loadFromFile("images/bulldozer.png")) { std::cerr << "Nie mozna wczytac bulldozer.png\n"; }
    m_bulldozerSprite.setTexture(m_bulldozerTexture);
    sf::Vector2u texSize = m_bulldozerTexture.getSize();
    m_bulldozerSprite.setScale(m_demolishHotspot.width / texSize.x, m_demolishHotspot.height / texSize.y);
    m_bulldozerSprite.setPosition(m_demolishHotspot.left, m_demolishHotspot.top);

    if (!m_bellTexture.loadFromFile("images/bell.png")) { std::cerr << "Nie mozna wczytac bell.png\n"; }
    m_bellSprite.setTexture(m_bellTexture);
    m_bellSprite.setScale(m_contractsHotspot.width / m_bellTexture.getSize().x, m_contractsHotspot.height / m_bellTexture.getSize().y);
    m_bellSprite.setPosition(m_contractsHotspot.left, m_contractsHotspot.top);

    // --- Inicjalizacja powiadomień (kółko i liczba) ---
    m_notificationCircle.setRadius(12.f);
    m_notificationCircle.setFillColor(sf::Color::Red);
    m_notificationCircle.setOutlineColor(sf::Color::White);
    m_notificationCircle.setOutlineThickness(1.f);
    m_notificationCircle.setPosition(m_contractsHotspot.left + m_contractsHotspot.width - m_notificationCircle.getRadius() - 5.f, m_contractsHotspot.top - 5.f);

    m_notificationCountText.setFont(m_font);
    m_notificationCountText.setCharacterSize(16);
    m_notificationCountText.setFillColor(sf::Color::White);

    m_demolishCancelText.setFont(m_font);
    m_demolishCancelText.setString("[ESC] Anuluj");
    m_demolishCancelText.setCharacterSize(24);
    m_demolishCancelText.setFillColor(sf::Color::White);
    m_demolishCancelText.setOutlineColor(sf::Color::Black);
    m_demolishCancelText.setOutlineThickness(2.f);
    sf::FloatRect textBounds = m_demolishCancelText.getLocalBounds();
    m_demolishCancelText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_demolishCancelText.setPosition(1200 / 2.f, 30.f);

    // --- Konfiguracja widoku (kamery) dla obszaru grywalnego ---
    m_gameplayView.setSize(m_playableArea.width, m_playableArea.height);
    m_gameplayView.setCenter(m_playableArea.left + m_playableArea.width / 2.f, m_playableArea.top + m_playableArea.height / 2.f);
    m_gameplayView.setViewport(sf::FloatRect(
        m_playableArea.left / 1200.f,
        m_playableArea.top / 800.f,
        m_playableArea.width / 1200.f,
        m_playableArea.height / 800.f
        ));

    // --- Finalne inicjalizacje ---
    initializeWeatherEffects();
    reset();
}

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
    m_netEnergyPerSecond = 0.f;
    m_netEnvChangePerSecond = 0.f;
    m_moneyFromContractsPerSecond = 0.f;
    m_secondAccumulator = 0.f;
    m_energyAccumulator = 0.f;
    m_grid.initialize({0.f, 121.f}, GameConstants::GRID_COLS, GameConstants::GRID_ROWS, GameConstants::GRID_CELL_SIZE);
    m_weatherManager.reset();
    m_contractManager.reset();
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
                if (m_playableArea.contains(pos.x, pos.y)) {
                    placeBuilding(drag->itemId, drag->price, pos);
                }
                m_buildMenu.cancelDragging(); m_isBuildMode = false; m_hammerPressed = false;
            } else if (ev.mouseButton.button == sf::Mouse::Right) {
                m_buildMenu.cancelDragging(); m_isBuildMode = false; m_hammerPressed = false;
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
            else if (m_contractsHotspot.contains(pos)) { uiClicked = true; m_audioManager.play(SoundEffect::ButtonClick); currentState = GameState::ContractsMenu; m_contractManager.markAsRead(); }
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
                // --- POPRAWKA: Dodano logikę odtwarzania dźwięków po kliknięciu na budynek ---
                for (auto& obj : placedObjects) {
                    if (obj.sprite.getGlobalBounds().contains(pos)) {
                        if (obj.isDamaged) {
                            // Odtwórz dźwięk i otwórz panel naprawy dla uszkodzonych budynków
                            m_audioManager.play(SoundEffect::ButtonClick);
                            setRepairTarget(&obj);
                        } else {
                            // Odtwórz dźwięk specyficzny dla danego typu budynku
                            switch (obj.typeId) {
                            case GameConstants::SOLAR_PANEL_ID:
                                m_audioManager.play(SoundEffect::SolarPanel);
                                break;
                            case GameConstants::WIND_TURBINE_ID:
                                m_audioManager.play(SoundEffect::WindTurbine);
                                break;
                            case GameConstants::ENERGY_STORAGE_ID:
                                m_audioManager.play(SoundEffect::EnergyStorage);
                                break;
                            // Dla Stacji Filtrowania można użyć domyślnego dźwięku
                            case GameConstants::AIR_FILTER_ID:
                                m_audioManager.play(SoundEffect::ButtonClick);
                                break;
                            }
                        }
                        // Po znalezieniu klikniętego budynku, przerywamy dalsze sprawdzanie
                        return;
                    }
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
    m_secondAccumulator += dt;

    // --- Faza obliczeń ---
    float baseEnergyPerSecond = 0.f;
    m_netEnvChangePerSecond = 0.f;

    for (const auto& obj : placedObjects) {
        if (obj.isDamaged) continue;
        const auto* data = getBuildingData(obj.typeId);
        if (data) {
            float envEffect = data->envEffect[obj.level - 1];
            m_netEnvChangePerSecond += envEffect;
            if (obj.typeId == GameConstants::SOLAR_PANEL_ID) {
                baseEnergyPerSecond += data->value[obj.level - 1] * m_weatherManager.getWeatherMultiplierSolar();
            } else if (obj.typeId == GameConstants::WIND_TURBINE_ID) {
                baseEnergyPerSecond += data->value[obj.level - 1] * m_weatherManager.getWeatherMultiplierWind();
            } else if (obj.typeId == GameConstants::AIR_FILTER_ID) {
                baseEnergyPerSecond -= data->value[obj.level - 1];
            }
        }
    }

    float contractEnergyDrain = 0.f;
    m_contractManager.calculateActiveContracts(contractEnergyDrain, m_moneyFromContractsPerSecond);

    m_netEnergyPerSecond = baseEnergyPerSecond - contractEnergyDrain;

    m_energyAccumulator += m_netEnergyPerSecond * dt;
    if (m_energyAccumulator >= 1.0f) {
        int energyGained = static_cast<int>(floor(m_energyAccumulator));
        currentEnergy += energyGained;
        m_energyAccumulator -= energyGained;
    } else if (m_energyAccumulator <= -1.0f) {
        int energyLost = static_cast<int>(ceil(m_energyAccumulator));
        currentEnergy += energyLost;
        m_energyAccumulator -= energyLost;
    }

    switch(m_weatherManager.getCurrentWeather()) {
    case WeatherType::Rain: m_netEnvChangePerSecond += 0.05f; break;
    case WeatherType::AcidRain: m_netEnvChangePerSecond -= 0.2f; break;
    case WeatherType::Smog: m_netEnvChangePerSecond -= 0.5f; break;
    default: break;
    }

    // --- Faza aktualizacji menedżerów ---
    m_weatherManager.update(dt, environmentHealth, placedObjects,
                            [this](const std::string& msg){ this->m_uiManager.showNotification(msg); });
    m_contractManager.update(dt, *this);

    if (m_secondAccumulator >= 1.0f) {
        environmentHealth += m_netEnvChangePerSecond;
        currentMoney += static_cast<int>(m_moneyFromContractsPerSecond);
        m_secondAccumulator -= 1.0f;
        if (environmentHealth < 0) environmentHealth = 0;
        if (environmentHealth > 100) environmentHealth = 100;
    }

    for (auto& obj : placedObjects) {
        if(obj.logic && !obj.isDamaged) {
            obj.logic->update(dt, *this, obj);
            sf::IntRect textureRect = obj.logic->getTextureRect();
            if (textureRect.width != 0) {
                obj.sprite.setTextureRect(textureRect);
            }
        }
    }

    // --- Faza finalizacji i aktualizacji UI ---
    if (m_demolishModeActive) m_bulldozerSprite.setColor(sf::Color(180, 255, 180));
    else m_bulldozerSprite.setColor(sf::Color::White);

    if (currentEnergy > maxEnergy) currentEnergy = maxEnergy;
    if (currentEnergy < 0) currentEnergy = 0;

    if (currentEnergy == 0 && m_netEnergyPerSecond < 0) {
        if (m_contractManager.hasActiveContracts()) {
            m_contractManager.terminateAllActiveContracts();
            m_uiManager.showNotification("Zerwanie kontraktów - brak energii");
        }
    }

    m_hud.update(currentMoney, m_moneyFromContractsPerSecond, currentEnergy, maxEnergy, m_netEnergyPerSecond, environmentHealth, m_netEnvChangePerSecond, totalGameTimeSeconds);
    m_uiManager.update(m_weatherManager);

    updateWeatherOverlay();
    updateRainEffect(dt);
    updateCloudEffect(dt);
    updateWindEffect(dt);
    updateSmogEffect(dt);
    updateHeatwaveEffect(dt);
}

void Game::draw(sf::RenderWindow& window, GameState currentState) {
    window.setView(window.getDefaultView());
    m_gameBg.draw(window);

    window.setView(m_gameplayView);
    window.draw(m_grassArea);
    for (const auto& obj : placedObjects) {
        window.draw(obj.sprite);
    }
    drawDamagedIcons(window);
    drawWeatherOverlay(window);
    drawDynamicWeatherEffects(window);
    drawRainEffect(window);

    window.setView(window.getDefaultView());

    window.draw(m_bulldozerSprite);
    window.draw(m_bellSprite);

    int unreadCount = m_contractManager.getUnreadCount();
    if (unreadCount > 0) {
        window.draw(m_notificationCircle);
        m_notificationCountText.setString(std::to_string(unreadCount));
        sf::FloatRect textBounds = m_notificationCountText.getLocalBounds();

        m_notificationCountText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 1.8f);

        m_notificationCountText.setPosition(m_notificationCircle.getPosition().x + m_notificationCircle.getRadius(), m_notificationCircle.getPosition().y + m_notificationCircle.getRadius());
        window.draw(m_notificationCountText);
    }

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
            const sf::FloatRect buildingBounds = obj.sprite.getGlobalBounds();
            const sf::Vector2u textureSize = m_repairIconTexture.getSize();
            if (textureSize.x > 0 && textureSize.y > 0) {
                float scaleX = buildingBounds.width / textureSize.x * 0.5f;
                float scaleY = buildingBounds.height / textureSize.y * 0.5f;
                m_repairIconSprite.setScale(scaleX, scaleY);
            }
            m_repairIconSprite.setPosition(obj.sprite.getPosition());
            window.draw(m_repairIconSprite);
        }
    }
}

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
    const GameConstants::BuildingLevelData* data = getBuildingData(building.typeId);

    if (data) cost = data->upgradeCost[building.level -1];

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

    float sourceWidth = static_cast<float>(texture.getSize().x);
    float sourceHeight = static_cast<float>(texture.getSize().y);
    sf::Vector2f targetSize(buildingSize.x * GameConstants::GRID_CELL_SIZE, buildingSize.y * GameConstants::GRID_CELL_SIZE);
    newObj.sprite.setScale(targetSize.x / sourceWidth, targetSize.y / sourceHeight);
    newObj.sprite.setOrigin(sourceWidth / 2.f, sourceHeight / 2.f);

    sf::Vector2f finalPosition = m_grid.gridToWorldCoords(gridPos);
    finalPosition.x += (buildingSize.x * GameConstants::GRID_CELL_SIZE) / 2.0f;
    finalPosition.y += (buildingSize.y * GameConstants::GRID_CELL_SIZE) / 2.0f;
    newObj.sprite.setPosition(finalPosition);

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
ContractManager& Game::getContractManager() { return m_contractManager; }
const ContractManager& Game::getContractManager() const { return m_contractManager; }
float Game::getNetEnergyPerSecond() const { return m_netEnergyPerSecond; }

void Game::initializeWeatherEffects() {
    m_weatherOverlay.setSize({m_playableArea.width, m_playableArea.height});
    m_weatherOverlay.setPosition(m_playableArea.left, m_playableArea.top);
    m_weatherOverlay.setFillColor(sf::Color::Transparent);

    if (!m_rainTexture.loadFromFile("images/eco_rain.png")) { std::cerr << "Nie udalo sie wczytac tekstury deszczu images/eco_rain.png\n"; }
    else {
        float scale = 1200.f / m_rainTexture.getSize().x;
        m_rainSprite1.setTexture(m_rainTexture); m_rainSprite1.setScale(scale, scale);
        m_rainSprite2.setTexture(m_rainTexture); m_rainSprite2.setScale(scale, scale);
        m_rainSprite1.setPosition(0, 0);
        m_rainSprite2.setPosition(0, -m_rainSprite1.getGlobalBounds().height);
    }

    std::mt19937 gen(std::random_device{}());

    if (!m_cloudTexture.loadFromFile("images/eco_cloud.png")) { std::cerr << "Nie udalo sie wczytac tekstury chmur images/eco_cloud.png\n"; }
    else {
        std::uniform_real_distribution<float> xDist(0, m_playableArea.width);
        std::uniform_real_distribution<float> yDist(m_playableArea.top, m_playableArea.top + 150);
        std::uniform_real_distribution<float> scaleDist(0.5f, 1.2f);
        m_cloudSprites.resize(4);
        for (auto& cloud : m_cloudSprites) {
            cloud.setTexture(m_cloudTexture);
            cloud.setPosition(xDist(gen), yDist(gen));
            float s = scaleDist(gen);
            cloud.setScale(s, s);
        }
    }

    if (!m_leafTexture.loadFromFile("images/eco_leaf.png")) { std::cerr << "Nie udalo sie wczytac tekstury lisci images/eco_leaf.png\n"; }
    else {
        std::uniform_real_distribution<float> xDist(0, m_playableArea.width);
        std::uniform_real_distribution<float> yDist(m_playableArea.top, m_playableArea.top + m_playableArea.height);
        std::uniform_real_distribution<float> velDist(100.f, 300.f);
        std::uniform_real_distribution<float> rotDist(-90.f, 90.f);
        m_windParticles.resize(20);
        for (auto& particle : m_windParticles) {
            particle.sprite.setTexture(m_leafTexture);
            particle.sprite.setPosition(xDist(gen), yDist(gen));
            particle.sprite.setScale(0.1f, 0.1f);
            particle.velocity.x = velDist(gen);
            particle.rotationSpeed = rotDist(gen);
        }
    }

    if (!m_smogTexture.loadFromFile("images/eco_smog.png")) { std::cerr << "Nie udalo sie wczytac tekstury smogu images/eco_smog.png\n"; }
    else {
        std::uniform_real_distribution<float> xDist(0, m_playableArea.width);
        std::uniform_real_distribution<float> yDist(m_playableArea.top + 100, m_playableArea.top + m_playableArea.height);
        std::uniform_real_distribution<float> scaleDist(0.8f, 1.5f);
        m_smogSprites.resize(5);
        for (auto& smog : m_smogSprites) {
            smog.setTexture(m_smogTexture);
            smog.setPosition(xDist(gen), yDist(gen));
            float s = scaleDist(gen);
            smog.setScale(s, s);
        }
    }

    if (!m_tumbleweedTexture.loadFromFile("images/eco_tumbleweed.png")) { std::cerr << "Nie udalo sie wczytac tekstury krzewu images/eco_tumbleweed.png\n"; }
    else {
        std::uniform_real_distribution<float> xDist(m_playableArea.left, m_playableArea.left + m_playableArea.width);
        std::uniform_real_distribution<float> yDist(m_playableArea.top, m_playableArea.top + m_playableArea.height);
        std::uniform_real_distribution<float> rotDist(-80.f, 80.f);
        std::uniform_real_distribution<float> velXDist(100.f, 200.f);
        std::uniform_real_distribution<float> velYDist(-40.f, 40.f);
        m_tumbleweedParticles.resize(3);
        for (auto& p : m_tumbleweedParticles) {
            p.sprite.setTexture(m_tumbleweedTexture);
            p.sprite.setOrigin(m_tumbleweedTexture.getSize().x / 2.f, m_tumbleweedTexture.getSize().y / 2.f);
            float desiredSize = GameConstants::GRID_CELL_SIZE;
            float scale = desiredSize / m_tumbleweedTexture.getSize().x;
            p.sprite.setScale(scale, scale);
            p.sprite.setPosition(xDist(gen), yDist(gen));
            p.rotationSpeed = rotDist(gen);
            p.velocity = {velXDist(gen), velYDist(gen)};
        }
    }
}

void Game::updateWeatherOverlay() {
    sf::Color overlayColor = sf::Color::Transparent;
    WeatherType current = m_weatherManager.getCurrentWeather();
    switch (current) {
    case WeatherType::Cloudy: overlayColor = sf::Color(150, 150, 150, 90); break;
    case WeatherType::Rain: overlayColor = sf::Color(70, 80, 100, 100); break;
    case WeatherType::Storm: overlayColor = sf::Color(40, 40, 80, 120); break;
    case WeatherType::AcidRain: overlayColor = sf::Color(80, 120, 80, 100); break;
    case WeatherType::Smog: overlayColor = sf::Color(130, 110, 90, 120); break;
    default: break;
    }
    m_weatherOverlay.setFillColor(overlayColor);
    if (current == WeatherType::Heatwave) {
        float elapsedSeconds = m_effectsClock.getElapsedTime().asSeconds();
        float alpha = 80 + 50 * (sin(elapsedSeconds * 2.0f) + 1.0f) / 2.0f;
        m_weatherOverlay.setFillColor(sf::Color(230, 150, 40, static_cast<sf::Uint8>(alpha)));
    }
}

void Game::updateRainEffect(float dt) {
    WeatherType current = m_weatherManager.getCurrentWeather();
    if (!(current == WeatherType::Rain || current == WeatherType::Storm || current == WeatherType::AcidRain)) return;

    sf::Color rainColor = sf::Color::White;
    float rainSpeed = 300.f;
    if (current == WeatherType::Storm) rainSpeed = 700.f;
    else if (current == WeatherType::AcidRain) rainColor = sf::Color(100, 250, 100, 255);

    m_rainSprite1.setColor(rainColor);
    m_rainSprite2.setColor(rainColor);
    m_rainSprite1.move(0, rainSpeed * dt);
    m_rainSprite2.move(0, rainSpeed * dt);

    float spriteHeight = m_rainSprite1.getGlobalBounds().height;
    if (m_rainSprite1.getPosition().y >= m_playableArea.top + m_playableArea.height) m_rainSprite1.setPosition(0, m_rainSprite2.getPosition().y - spriteHeight);
    if (m_rainSprite2.getPosition().y >= m_playableArea.top + m_playableArea.height) m_rainSprite2.setPosition(0, m_rainSprite1.getPosition().y - spriteHeight);
}

void Game::updateCloudEffect(float dt) {
    if (m_weatherManager.getCurrentWeather() != WeatherType::Cloudy) return;
    float cloudSpeed = 20.f;
    for (auto& cloud : m_cloudSprites) {
        cloud.move(cloudSpeed * dt, 0);
        float elapsedSeconds = m_effectsClock.getElapsedTime().asSeconds();
        float alphaValue = 100 + 155 * (sin(elapsedSeconds * 0.5f + cloud.getPosition().y * 0.05f) + 1) / 2;
        cloud.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alphaValue)));
        if (cloud.getPosition().x > m_playableArea.width) cloud.setPosition(-cloud.getGlobalBounds().width, cloud.getPosition().y);
    }
}

void Game::updateWindEffect(float dt) {
    if (m_weatherManager.getCurrentWeather() != WeatherType::Windy) return;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> yDist(m_playableArea.top, m_playableArea.top + m_playableArea.height);
    for (auto& particle : m_windParticles) {
        particle.sprite.move(particle.velocity * dt);
        particle.sprite.rotate(particle.rotationSpeed * dt);
        if (particle.sprite.getPosition().x > m_playableArea.width) {
            particle.sprite.setPosition(-particle.sprite.getGlobalBounds().width, yDist(gen));
        }
    }
}

void Game::updateSmogEffect(float dt) {
    if (m_weatherManager.getCurrentWeather() != WeatherType::Smog) return;
    float smogSpeed = 15.f;
    for (auto& smog : m_smogSprites) {
        smog.move(smogSpeed * dt, 0);
        float elapsedSeconds = m_effectsClock.getElapsedTime().asSeconds();
        float alphaValue = 80 + 100 * (sin(elapsedSeconds * 0.3f + smog.getPosition().y * 0.05f) + 1) / 2;
        smog.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alphaValue)));
        if (smog.getPosition().x > m_playableArea.width) smog.setPosition(-smog.getGlobalBounds().width, smog.getPosition().y);
    }
}

void Game::updateHeatwaveEffect(float dt) {
    if (m_weatherManager.getCurrentWeather() != WeatherType::Heatwave) return;
    for (auto& p : m_tumbleweedParticles) {
        p.sprite.move(p.velocity * dt);
        p.sprite.rotate(p.rotationSpeed * dt);
        sf::Vector2f pos = p.sprite.getPosition();
        sf::FloatRect bounds = p.sprite.getGlobalBounds();
        if (pos.x - bounds.width / 2.f < m_playableArea.left) { p.velocity.x *= -1; p.sprite.setPosition(m_playableArea.left + bounds.width / 2.f, pos.y); }
        if (pos.x + bounds.width / 2.f > m_playableArea.left + m_playableArea.width) { p.velocity.x *= -1; p.sprite.setPosition(m_playableArea.left + m_playableArea.width - bounds.width / 2.f, pos.y); }
        if (pos.y - bounds.height / 2.f < m_playableArea.top) { p.velocity.y *= -1; p.sprite.setPosition(pos.x, m_playableArea.top + bounds.height / 2.f); }
        if (pos.y + bounds.height / 2.f > m_playableArea.top + m_playableArea.height) { p.velocity.y *= -1; p.sprite.setPosition(pos.x, m_playableArea.top + m_playableArea.height - bounds.height / 2.f); }
    }
}

void Game::drawWeatherOverlay(sf::RenderWindow& window) {
    window.draw(m_weatherOverlay);
}

void Game::drawRainEffect(sf::RenderWindow& window) {
    WeatherType current = m_weatherManager.getCurrentWeather();
    if (current == WeatherType::Rain || current == WeatherType::Storm || current == WeatherType::AcidRain) {
        window.draw(m_rainSprite1);
        window.draw(m_rainSprite2);
    }
}

void Game::drawDynamicWeatherEffects(sf::RenderWindow& window) {
    WeatherType current = m_weatherManager.getCurrentWeather();
    if (current == WeatherType::Cloudy) for (const auto& cloud : m_cloudSprites) window.draw(cloud);
    else if (current == WeatherType::Windy) for (const auto& particle : m_windParticles) window.draw(particle.sprite);
    else if (current == WeatherType::Smog) for (const auto& smog : m_smogSprites) window.draw(smog);
    else if (current == WeatherType::Heatwave) for (const auto& p : m_tumbleweedParticles) window.draw(p.sprite);
}
