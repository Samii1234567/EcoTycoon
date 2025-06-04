#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Struktura pojedynczego elementu do zbudowania
struct BuildItem {
    std::string          name;
    float                basePrice;
    float                currentPrice;
    sf::RectangleShape   buttonShape;
    sf::Text             nameText;
    sf::Text             priceText;
};

class BuildMenu {
public:
    BuildMenu();
    ~BuildMenu() = default;

    // Musisz wywołać tę funkcję w main() po załadowaniu czcionki (gameFont)
    bool initialize(const sf::Font& font);

    // Włącza/wyłącza widoczność całego menu
    void setVisible(bool visible);
    bool isVisible() const;

    // Obsługuje zdarzenia myszy, np. kliknięcie w jedną z opcji budowy.
    // Podaje przez referencję stan konta (money), żeby odjąć cenę, jeśli zakup możliwy.
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, float& money);

    // Rysuje całe menu, jeśli jest widoczne
    void draw(sf::RenderWindow& window);

    // Powinieneś wywoływać to co klatkę (np. w sekcji Playing), aby podnosić ceny co 30 sek.
    void update(float deltaTime);

private:
    std::vector<BuildItem> items;
    sf::RectangleShape     background;
    bool                   visible;
    sf::Clock              priceClock;

    // Pomocnicze:
    void setupItems(const sf::Font& font);
    void increasePrices();
};
