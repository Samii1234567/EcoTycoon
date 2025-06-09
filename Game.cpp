#include "Game.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Prototyp funkcji fabrykującej z SaveManager.cpp
std::unique_ptr<Building> createBuildingById(int typeId);

Game::Game(sf::Font& font, std::vector<sf::Texture>& buildingTextures)
    : m_font(font),
    m_buildingTextures(buildingTextures),
    m_hud(font)
{
    m_gameBg.loadFromFile("images/eco_tycoon_game.png");
    m_buildMenu.initialize(m_font, m_buildingTextures);

    // ZMIANA: Wczytywanie tekstury trawy
    if (!m_grassTexture.loadFromFile("images/eco_grass.png")) {
        std::cerr << "Blad: nie mozna wczytac eco_grass.png\n";
    }
    m_grassArea.setSize({1200.f, 595.f});
    m_grassArea.setPosition(0.f, 127.f);
    m_grassArea.setTexture(&m_grassTexture);

    // ZMIANA: Inicjalizacja UI menu energii
    const unsigned WIN_W = 1200, WIN_H = 800;
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
}

void Game::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState) {
    // Zdarzenia dla głównego stanu gry
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
    // Zdarzenia dla otwartego menu energii
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
}

void Game::update(float dt) {
    totalGameTimeSeconds += dt;

    // ZMIANA: Energia jest generowana tylko, jeśli mamy magazyn
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
    window.draw(m_grassArea); // Rysowanie trawy
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
    draw(window); // Rysujemy to samo co w grze
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

// ZMIANA: Nowe metody do menu energii
void Game::updateEnergyMenu() {
    std::ostringstream energyInfo, energyGain;
    energyInfo << "Zgromadzona energia: " << static_cast<int>(currentEnergy) << " / " << static_cast<int>(maxEnergy);
    m_energyInfoText.setString(energyInfo.str());

    energyGain << "Wartosc sprzedazy: " << std::fixed << std::setprecision(2) << (currentEnergy * GameConstants::ENERGY_SELL_PRICE) << "$";
    m_energyValueText.setString(energyGain.str());
}

void Game::drawEnergyMenu(sf::RenderWindow& window) {
    // Rysujemy najpierw całą scenę gry w tle
    draw(window);
    // A na wierzchu panel
    window.draw(m_energyPanel);
    window.draw(m_energyMenuTitle);
    window.draw(m_energyInfoText);
    window.draw(m_energyValueText);
    window.draw(m_sellButton);
    window.draw(m_sellButtonText);
    window.draw(m_closeButton);
    window.draw(m_closeButtonText);
}
