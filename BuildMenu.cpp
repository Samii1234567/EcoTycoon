#include "BuildMenu.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY BUILDMENU
//
//  Zawiera logikę metod zadeklarowanych w BuildMenu.h.
//
// ===================================================================================

// Implementacja konstruktora.
BuildMenu::BuildMenu()
    : visible(false)
{
}

// Implementacja inicjalizacji menu.
bool BuildMenu::initialize(const sf::Font& font, const std::vector<sf::Texture>& textures) {
    // Przechowujemy referencję (jako wskaźnik) do tekstur.
    itemTexturesRef = &textures;

    // Konfiguracja wyglądu tła menu.
    background.setSize({300.f, 260.f});
    background.setFillColor(sf::Color(222, 184, 135, 230)); // Półprzezroczysty brązowy
    background.setPosition(10.f, 131.f);
    background.setOutlineColor(sf::Color(80, 51, 20));
    background.setOutlineThickness(2.f);

    // Wywołanie metody konfigurującej przyciski w menu.
    setupItems(font);
    return true;
}

// Implementacja ustawiania widoczności.
void BuildMenu::setVisible(bool v) {
    visible = v;
    // Jeśli ukrywamy menu, anulujemy ewentualne przeciąganie.
    if (!v) {
        cancelDragging();
    }
}

bool BuildMenu::isVisible() const {
    return visible;
}

// Implementacja konfiguracji przycisków w menu.
void BuildMenu::setupItems(const sf::Font& font) {
    items.clear();
    items.resize(4); // Menu ma 4 pozycje

    // Ustawienia geometryczne dla przycisków.
    float bgX = background.getPosition().x;
    float bgY = background.getPosition().y;
    float btnWidth  = 280.f;
    float btnHeight = 50.f;
    float spacing   = 60.f;

    // Dane dla poszczególnych budynków (nazwa i cena).
    std::vector<std::pair<std::string,int>> data = {
        {"Magazyn energii",     500},
        {"Panele sloneczne",    200},
        {"Turbina wiatrowa",    500},
        {"Stacja Filtrowania", 1500}
    };

    // Pętla tworząca każdy przycisk.
    for (std::size_t i = 0; i < items.size(); ++i) {
        BuildItem& it = items[i];
        it.id          = i;
        it.name        = data[i].first;
        it.basePrice   = data[i].second;
        it.currentPrice= it.basePrice;

        // Ustawianie kształtu i wyglądu przycisku.
        it.buttonShape.setSize({btnWidth, btnHeight});
        it.buttonShape.setPosition(bgX + 10.f, bgY + 10.f + i * spacing);
        it.buttonShape.setFillColor(sf::Color(245, 222, 179));
        it.buttonShape.setOutlineColor(sf::Color(80, 51, 20));
        it.buttonShape.setOutlineThickness(2.f);

        // Ustawianie tekstu nazwy budynku.
        it.nameText.setFont(font);
        it.nameText.setString(it.name);
        it.nameText.setCharacterSize(18);
        it.nameText.setFillColor(sf::Color(80, 51, 20));
        auto nb = it.nameText.getLocalBounds();
        it.nameText.setPosition(
            bgX + 10.f + 5.f,
            bgY + 10.f + i * spacing + (btnHeight - nb.height) / 2.f - 5.f
            );

        // Ustawianie tekstu ceny budynku.
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

// Implementacja obsługi zdarzeń.
BuildMenu::ClickResult BuildMenu::handleEvent(const sf::Event& ev, sf::RenderWindow& window, int& money) {
    // Ignoruj zdarzenia, jeśli menu jest niewidoczne lub już coś przeciągamy.
    if (!visible || dragState.has_value()) return ClickResult::None;

    // Sprawdzamy, czy wciśnięto lewy przycisk myszy.
    if (ev.type == sf::Event::MouseButtonPressed &&
        ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
        // Sprawdzamy, czy kliknięto któryś z przycisków w menu.
        for (const auto& it : items) {
            if (it.buttonShape.getGlobalBounds().contains(pos)) {
                // Jeśli gracz ma wystarczająco pieniędzy...
                if (money >= it.currentPrice) {
                    // ...rozpoczynamy przeciąganie.
                    dragState = DragState{it.id, it.currentPrice};
                    dragState->sprite.setTexture((*itemTexturesRef)[it.id]);

                    // Skalujemy "ducha" budynku do jego docelowych wymiarów na siatce.
                    sf::Vector2i buildingSizeInCells = getBuildingSize(it.id);
                    sf::Vector2f targetSizeInPixels = {
                        buildingSizeInCells.x * GameConstants::GRID_CELL_SIZE,
                        buildingSizeInCells.y * GameConstants::GRID_CELL_SIZE
                    };

                    const auto& tex = (*itemTexturesRef)[it.id];
                    float sourceWidth = static_cast<float>(tex.getSize().x);
                    float sourceHeight = static_cast<float>(tex.getSize().y);
                    dragState->sprite.setScale(targetSizeInPixels.x / sourceWidth, targetSizeInPixels.y / sourceHeight);
                    dragState->sprite.setOrigin(sourceWidth / 2.f, sourceHeight / 2.f);

                    // Ustawiamy przezroczystość i ukrywamy menu budowania.
                    dragState->sprite.setColor({255, 255, 255, 150});
                    visible = false;
                    return ClickResult::DragStarted;
                } else {
                    // Jeśli brakuje pieniędzy, aktywujemy miganie ceny.
                    m_flashItemId = it.id;
                    m_flashClock.restart();
                    return ClickResult::NotEnoughMoney;
                }
            }
        }
    }
    return ClickResult::None;
}

// Implementacja rysowania menu.
void BuildMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;

    // Wyłącz miganie ceny po 0.5 sekundy.
    if (m_flashItemId != -1 && m_flashClock.getElapsedTime().asSeconds() > 0.5f) {
        m_flashItemId = -1;
    }

    window.draw(background);
    for (auto& it : items) {
        window.draw(it.buttonShape);
        window.draw(it.nameText);

        // Tymczasowa zmiana koloru ceny na czerwony, jeśli miganie jest aktywne.
        sf::Color originalColor = it.priceText.getFillColor();
        if (it.id == m_flashItemId) {
            it.priceText.setFillColor(sf::Color::Red);
        }
        window.draw(it.priceText);
        it.priceText.setFillColor(originalColor); // Przywrócenie oryginalnego koloru.
    }
}

// Proste gettery i metody pomocnicze.
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
