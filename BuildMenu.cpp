#include "BuildMenu.h"
#include <cmath>
#include <iostream>

BuildMenu::BuildMenu()
    : visible(false)
{
}

bool BuildMenu::initialize(const sf::Font& font, const std::vector<sf::Texture>& textures) {
    itemTexturesRef = &textures;

    background.setSize({300.f, 200.f});
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
    items.resize(3);

    float bgX = background.getPosition().x;
    float bgY = background.getPosition().y;
    float btnWidth  = 280.f;
    float btnHeight = 50.f;
    float spacing   = 60.f;

    std::vector<std::pair<std::string,int>> data = {
        {"Magazyn energii",  500},
        {"Panele sloneczne", 200},
        {"Turbina wiatrowa", 500}
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

void BuildMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window, int& money) {
    if (!visible || dragState.has_value()) return;

    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
        for (const auto& it : items) {
            if (it.buttonShape.getGlobalBounds().contains(pos)) {
                if (money >= it.currentPrice) {
                    dragState = DragState{it.id, it.currentPrice};
                    dragState->sprite.setTexture((*itemTexturesRef)[it.id]);
                    auto texSize = (*itemTexturesRef)[it.id].getSize();
                    dragState->sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
                    dragState->sprite.setColor({255, 255, 255, 150});
                    std::cout << "Rozpoczeto przeciaganie: " << it.name << "\n";
                } else {
                    std::cout << "Brak srodkow na " << it.name
                              << ". Potrzebujesz " << it.currentPrice << "$.\n";
                }
                return;
            }
        }
    }
}

void BuildMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;
    window.draw(background);
    for (auto& it : items) {
        window.draw(it.buttonShape);
        window.draw(it.nameText);
        window.draw(it.priceText);
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
