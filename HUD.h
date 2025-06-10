#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD(sf::Font& font);

    // ZMIANA: Metoda przyjmuje teraz wartości int
    void update(int money, int energy, int maxEnergy, float envHealth, float totalTime);
    void draw(sf::RenderWindow& window);
    const sf::FloatRect getEnergySellButtonBounds() const;

private:
    sf::Font& m_font;
    sf::Text m_labelMoney, m_labelEnergy, m_labelEnv, m_labelTime;
    sf::Text m_valueMoney, m_valueEnergy, m_valueEnv;
    sf::CircleShape m_energySellButtonBg;
    sf::Text m_energySellIcon;
};

#endif // HUD_H
