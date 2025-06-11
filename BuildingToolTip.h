#ifndef BUILDINGTOOLTIP_H
#define BUILDINGTOOLTIP_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Building.h" // Potrzebne dla definicji PlacedObject

class BuildingTooltip {
public:
    BuildingTooltip();

    void initialize(sf::Font& font);
    void show(const PlacedObject& building, int playerMoney);
    void hide();
    bool isVisible() const;
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool isUpgradeClicked() const;

private:
    sf::RectangleShape m_background;
    sf::Text m_titleText, m_levelText, m_infoText, m_upgradeButtonText;
    sf::RectangleShape m_upgradeButton;
    bool m_visible;
    bool m_upgradeClicked;
};

#endif // BUILDINGTOOLTIP_H
