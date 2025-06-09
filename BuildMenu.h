#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

// Struktura pojedynczego elementu do zbudowania
struct BuildItem {
    int                  id;
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

    // Struktura przechowująca informacje o przeciąganym obiekcie
    struct DragState {
        int itemId;
        float price;
        sf::Sprite sprite;
    };

    // Inicjalizacja przyjmuje teraz referencję do tekstur wczytanych w main
    bool initialize(const sf::Font& font, const std::vector<sf::Texture>& textures);

    void setVisible(bool visible);
    bool isVisible() const;
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, float& money);
    void draw(sf::RenderWindow& window);

    std::optional<DragState>& getDragState();
    void cancelDragging();

private:
    std::vector<BuildItem> items;
    sf::RectangleShape     background;
    bool                   visible;

    // Wskaźnik do tekstur, których właścicielem jest main.cpp
    const std::vector<sf::Texture>* itemTexturesRef = nullptr;

    std::optional<DragState> dragState;

    void setupItems(const sf::Font& font);
};
