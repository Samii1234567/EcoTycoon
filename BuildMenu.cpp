#include "BuildMenu.h"
#include <cmath>
#include <iostream>

// ---------------------
// Konstruktor
// ---------------------
BuildMenu::BuildMenu()
    : visible(false)
{
}

// ---------------------
// initialize: tworzy tło i przyciski w menu
// ---------------------
bool BuildMenu::initialize(const sf::Font& font) {
    // Tło panelu – pozycja (10,100) w Twojej scenie 'Playing'
    background.setSize({300.f, 260.f});                   // 4 przyciski * 60px + padding
    background.setFillColor(sf::Color(222, 184, 135, 230)); // beż (z lekką przezroczystością)
    background.setPosition(10.f, 100.f + 10.f);            // 10px poniżej górnego paska + 10px margines
    background.setOutlineColor(sf::Color(80, 51, 20));
    background.setOutlineThickness(2.f);

    setupItems(font);
    return true;
}

// ---------------------
// Pokazuje/ukrywa menu budowy
// ---------------------
void BuildMenu::setVisible(bool v) {
    visible = v;
    if (v) {
        priceClock.restart();
    }
}

bool BuildMenu::isVisible() const {
    return visible;
}

// ---------------------
// Inicjalizuje cztery przyciski: Wiatrak, Panele słoneczne, Turbina wiatrowa, Tama wodna
// ---------------------
void BuildMenu::setupItems(const sf::Font& font) {
    items.clear();
    items.resize(4);

    // Pozycja tła:
    float bgX = background.getPosition().x;
    float bgY = background.getPosition().y;
    float btnWidth  = 280.f;
    float btnHeight = 50.f;
    float spacing   = 60.f; // odległość między kolejnymi przyciskami

    // Nazwy i ceny początkowe:
    std::vector<std::pair<std::string,float>> data = {
        {"Murzyn",          100.f},
        {"Panele sloneczne", 200.f},
        {"Turbina wiatrowa", 500.f},
        {"Tama wodna",       800.f}
    };

    for (std::size_t i = 0; i < items.size(); ++i) {
        BuildItem& it = items[i];
        it.name        = data[i].first;
        it.basePrice   = data[i].second;
        it.currentPrice= it.basePrice;

        // Kształt przycisku:
        it.buttonShape.setSize({btnWidth, btnHeight});
        it.buttonShape.setPosition(bgX + 10.f, bgY + 10.f + i * spacing);
        it.buttonShape.setFillColor(sf::Color(245, 222, 179)); // jasny beż
        it.buttonShape.setOutlineColor(sf::Color(80, 51, 20));
        it.buttonShape.setOutlineThickness(2.f);

        // Tekst nazwy obiektu:
        it.nameText.setFont(font);
        it.nameText.setString(it.name);
        it.nameText.setCharacterSize(18);
        it.nameText.setFillColor(sf::Color(80, 51, 20));
        auto nb = it.nameText.getLocalBounds();
        it.nameText.setPosition(
            bgX + 10.f + 5.f,
            bgY + 10.f + i * spacing + (btnHeight - nb.height) / 2.f - 5.f
            );

        // Tekst ceny:
        it.priceText.setFont(font);
        it.priceText.setCharacterSize(16);
        it.priceText.setFillColor(sf::Color(80, 51, 20));
        it.priceText.setString(std::to_string(static_cast<int>(it.currentPrice)) + "$");
        auto pb = it.priceText.getLocalBounds();
        it.priceText.setPosition(
            bgX + 10.f + btnWidth - pb.width - 10.f,
            bgY + 10.f + i * spacing + (btnHeight - pb.height) / 2.f - 5.f
            );
    }
}

// ---------------------
// Obsługuje kliknięcia w menu budowy; odejmuje money, jeśli wystarczają środki
// ---------------------
void BuildMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window, float& money) {
    if (!visible) return;

    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
        for (auto& it : items) {
            if (it.buttonShape.getGlobalBounds().contains(pos)) {
                if (money >= it.currentPrice) {
                    money -= it.currentPrice;
                    std::cout << it.name << " zakupiono za " << it.currentPrice << "$.\n";
                    // Tutaj możesz dodać logikę do umieszczenia obiektu na mapie
                } else {
                    std::cout << "Brak srodkow na " << it.name
                              << ". Potrzebujesz " << it.currentPrice << "$.\n";
                }
            }
        }
    }
}

// ---------------------
// Rysuje tło i przyciski, jeśli menu jest widoczne
// ---------------------
void BuildMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;
    window.draw(background);
    for (auto& it : items) {
        window.draw(it.buttonShape);
        window.draw(it.nameText);
        window.draw(it.priceText);
    }
}

// ---------------------
// Co deltaTime sprawdzamy, czy upłynęło 30 s, aby podnieść ceny o 10%
// ---------------------
void BuildMenu::update(float deltaTime) {
    if (!visible) return;

    if (priceClock.getElapsedTime().asSeconds() >= 5.f) {
        increasePrices();
        priceClock.restart();
    }
}

// ---------------------
// Podnosi ceny i aktualizuje teksty
// ---------------------
void BuildMenu::increasePrices() {
    for (auto& it : items) {
        it.currentPrice = std::ceil(it.currentPrice * 1.1f);
        it.priceText.setString(std::to_string(static_cast<int>(it.currentPrice)) + "$");
        auto pb = it.priceText.getLocalBounds();
        float bx = it.buttonShape.getPosition().x;
        float by = it.buttonShape.getPosition().y;
        float bw = it.buttonShape.getSize().x;
        float bh = it.buttonShape.getSize().y;
        it.priceText.setPosition(
            bx + bw - pb.width - 10.f,
            by + (bh - pb.height) / 2.f - 5.f
            );
    }
}
