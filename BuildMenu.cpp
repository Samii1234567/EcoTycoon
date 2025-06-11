#include "BuildMenu.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

BuildMenu::BuildMenu()
    : visible(false)
{
}

bool BuildMenu::initialize(const sf::Font& font, const std::vector<sf::Texture>& textures) {
    itemTexturesRef = &textures;

    background.setSize({300.f, 260.f});
    background.setFillColor(sf::Color(222, 184, 135, 230));
    background.setPosition(10.f, 131.f);
    background.setOutlineColor(sf::Color(80, 51, 20));
    background.setOutlineThickness(2.f);

    setupItems(font);
    return true;
}

void BuildMenu::setVisible(bool v) {
    visible = v;
    if (!v) {
        cancelDragging();
    }
}

bool BuildMenu::isVisible() const {
    return visible;
}

void BuildMenu::setupItems(const sf::Font& font) {
    items.clear();
    items.resize(4);

    float bgX = background.getPosition().x;
    float bgY = background.getPosition().y;
    float btnWidth  = 280.f;
    float btnHeight = 50.f;
    float spacing   = 60.f;

    std::vector<std::pair<std::string,int>> data = {
        {"Magazyn energii",     500},
        {"Panele sloneczne",    200},
        {"Turbina wiatrowa",    500},
        {"Stacja Filtrowania", 1500}
    };

    for (std::size_t i = 0; i < items.size(); ++i) {
        BuildItem& it = items[i];
        it.id          = i;
        it.name        = data[i].first;
        it.basePrice   = data[i].second;
        it.currentPrice= it.basePrice;

        it.buttonShape.setSize({btnWidth, btnHeight});
        it.buttonShape.setPosition(bgX + 10.f, bgY + 10.f + i * spacing);
        it.buttonShape.setFillColor(sf::Color(245, 222, 179));
        it.buttonShape.setOutlineColor(sf::Color(80, 51, 20));
        it.buttonShape.setOutlineThickness(2.f);

        it.nameText.setFont(font);
        it.nameText.setString(it.name);
        it.nameText.setCharacterSize(18);
        it.nameText.setFillColor(sf::Color(80, 51, 20));
        auto nb = it.nameText.getLocalBounds();
        it.nameText.setPosition(
            bgX + 10.f + 5.f,
            bgY + 10.f + i * spacing + (btnHeight - nb.height) / 2.f - 5.f
            );

        it.priceText.setFont(font);
        it.priceText.setCharacterSize(16);
        it.priceText.setFillColor(sf::Color(80, 51, 20));
        it.priceText.setString(std::to_string(it.currentPrice) + "$");
        auto pb = it.priceText.getLocalBounds();
        it.priceText.setPosition(
            bgX + 10.f + btnWidth - pb.width - 10.f,
            bgY + 10.f + i * spacing + (btnHeight - pb.height) / 2.f - 5.f
            );
    }
}

BuildMenu::ClickResult BuildMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window, int& money) {
    if (!visible || dragState.has_value()) return ClickResult::None;

    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
        for (const auto& it : items) {
            if (it.buttonShape.getGlobalBounds().contains(pos)) {
                if (money >= it.currentPrice) {
                    dragState = DragState{it.id, it.currentPrice};
                    dragState->sprite.setTexture((*itemTexturesRef)[it.id]);

                    // ##### POCZĄTEK ZMIAN - SKALOWANIE PRZECIĄGANEGO BUDYNKU #####
                    sf::Vector2i buildingSizeInCells = getBuildingSize(it.id);
                    sf::Vector2f targetSizeInPixels = {
                        buildingSizeInCells.x * GameConstants::GRID_CELL_SIZE,
                        buildingSizeInCells.y * GameConstants::GRID_CELL_SIZE
                    };

                    if (it.id == GameConstants::WIND_TURBINE_ID) {
                        dragState->sprite.setTextureRect({0, 0, GameConstants::TURBINE_FRAME_WIDTH, GameConstants::TURBINE_FRAME_HEIGHT});

                        float scale = targetSizeInPixels.x / GameConstants::TURBINE_FRAME_WIDTH;
                        dragState->sprite.setScale(scale, scale);
                        dragState->sprite.setOrigin(GameConstants::TURBINE_FRAME_WIDTH / 2.f, GameConstants::TURBINE_FRAME_HEIGHT);
                    } else {
                        const auto& tex = (*itemTexturesRef)[it.id];
                        float sourceWidth = static_cast<float>(tex.getSize().x);
                        float sourceHeight = static_cast<float>(tex.getSize().y);
                        dragState->sprite.setScale(targetSizeInPixels.x / sourceWidth, targetSizeInPixels.y / sourceHeight);
                        dragState->sprite.setOrigin(sourceWidth / 2.f, sourceHeight / 2.f);
                    }
                    // ##### KONIEC ZMIAN #####

                    dragState->sprite.setColor({255, 255, 255, 150});
                    visible = false;
                    return ClickResult::DragStarted;
                } else {
                    m_flashItemId = it.id;
                    m_flashClock.restart();
                    return ClickResult::NotEnoughMoney;
                }
            }
        }
    }
    return ClickResult::None;
}

void BuildMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;

    if (m_flashItemId != -1 && m_flashClock.getElapsedTime().asSeconds() > 0.5f) {
        m_flashItemId = -1;
    }

    window.draw(background);
    for (auto& it : items) {
        window.draw(it.buttonShape);
        window.draw(it.nameText);

        sf::Color originalColor = it.priceText.getFillColor();
        if (it.id == m_flashItemId) {
            it.priceText.setFillColor(sf::Color::Red);
        }
        window.draw(it.priceText);
        it.priceText.setFillColor(originalColor);
    }
}

std::optional<BuildMenu::DragState>& BuildMenu::getDragState() {
    return dragState;
}

void BuildMenu::cancelDragging() {
    if (dragState.has_value()) {
        std::cout << "Anulowano budowanie.\n";
        dragState.reset();
    }
}

sf::FloatRect BuildMenu::getBackgroundBounds() const {
    return background.getGlobalBounds();
}
