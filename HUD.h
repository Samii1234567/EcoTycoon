#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include <string>

class HUD {
public:
    HUD(sf::Font& font);

    void update(int money, int energy, int maxEnergy, float envHealth, float totalTime);
    void draw(sf::RenderWindow& window);
    const sf::FloatRect getEnergySellButtonBounds() const;
    void flashMoney(); // <-- ZMIANA: Publiczna funkcja do uruchamiania podświetlenia

private:
    sf::Font& m_font;
    sf::Text m_labelMoney, m_labelEnergy, m_labelEnv, m_labelTime;
    sf::Text m_valueMoney, m_valueEnergy, m_valueEnv;
    sf::CircleShape m_energySellButtonBg;
    sf::Text m_energySellIcon;

    // ZMIANA: Zmienne do podświetlania gotówki
    bool m_isFlashingMoney = false;
    sf::Clock m_moneyFlashClock;
};

#endif // HUD_H
