#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

struct BuildItem {
    int                  id;
    std::string          name;
    int                  basePrice;
    int                  currentPrice;
    sf::RectangleShape   buttonShape;
    sf::Text             nameText;
    sf::Text             priceText;
};

class BuildMenu {
public:
    BuildMenu();
    ~BuildMenu() = default;

    struct DragState {
        int itemId;
        int price;
        sf::Sprite sprite;
    };

    bool initialize(const sf::Font& font, const std::vector<sf::Texture>& textures);
    void setVisible(bool visible);
    bool isVisible() const;
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window, int& money);
    void draw(sf::RenderWindow& window);

    std::optional<DragState>& getDragState();
    void cancelDragging();

private:
    std::vector<BuildItem> items;
    sf::RectangleShape     background;
    bool                   visible;
    const std::vector<sf::Texture>* itemTexturesRef = nullptr;
    std::optional<DragState> dragState;

    void setupItems(const sf::Font& font);
};
