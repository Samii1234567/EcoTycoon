#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD(sf::Font& font);

    void update(float money, float energy, float maxEnergy, float envHealth, float totalTime);
    void draw(sf::RenderWindow& window);

    // ZMIANA: Metoda do sprawdzania, czy przycisk został kliknięty
    const sf::FloatRect getEnergySellButtonBounds() const;

private:
    sf::Font& m_font;
    sf::Text m_labelMoney, m_labelEnergy, m_labelEnv, m_labelTime;
    sf::Text m_valueMoney, m_valueEnergy, m_valueEnv;

    // ZMIANA: Elementy przycisku sprzedaży energii
    sf::CircleShape m_energySellButtonBg;
    sf::Text m_energySellIcon;
};

#endif // HUD_H
