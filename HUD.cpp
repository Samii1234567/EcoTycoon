#include "HUD.h"
#include <iomanip>
#include <sstream>

HUD::HUD(sf::Font& font) : m_font(font) {
    sf::Color brown(101, 67, 33);

    m_labelMoney.setFont(m_font); m_labelMoney.setString("Stan konta"); m_labelMoney.setCharacterSize(24); m_labelMoney.setFillColor(brown); m_labelMoney.setPosition(77.f, 726.f);
    m_valueMoney.setFont(m_font); m_valueMoney.setCharacterSize(24); m_valueMoney.setFillColor(brown); m_valueMoney.setPosition(77.f, 752.f);
    m_labelEnergy.setFont(m_font); m_labelEnergy.setString("Energia"); m_labelEnergy.setCharacterSize(24); m_labelEnergy.setFillColor(brown); m_labelEnergy.setPosition(390.f, 726.f);
    m_valueEnergy.setFont(m_font); m_valueEnergy.setCharacterSize(24); m_valueEnergy.setFillColor(brown); m_valueEnergy.setPosition(390.f, 752.f);
    m_labelEnv.setFont(m_font); m_labelEnv.setString("Stan srodowiska"); m_labelEnv.setCharacterSize(24); m_labelEnv.setFillColor(brown); m_labelEnv.setPosition(654.f, 726.f);
    m_valueEnv.setFont(m_font); m_valueEnv.setCharacterSize(24); m_valueEnv.setFillColor(brown); m_valueEnv.setPosition(654.f, 752.f);
    m_labelTime.setFont(m_font); m_labelTime.setCharacterSize(30); m_labelTime.setFillColor(brown); m_labelTime.setPosition(980.f, 744.f);

    m_energySellButtonBg.setRadius(15.f);
    m_energySellButtonBg.setFillColor(sf::Color(90, 160, 90));
    m_energySellIcon.setFont(m_font);
    m_energySellIcon.setString("$");
    m_energySellIcon.setCharacterSize(24);
    m_energySellIcon.setFillColor(sf::Color(255, 165, 0));
    m_energySellIcon.setStyle(sf::Text::Bold);

    float buttonCenterX = m_labelEnergy.getPosition().x + m_labelEnergy.getLocalBounds().width + 25.f;
    float buttonCenterY = m_labelEnergy.getPosition().y + m_labelEnergy.getLocalBounds().height / 2.f + 5.f;

    m_energySellButtonBg.setOrigin(15.f, 15.f);
    m_energySellButtonBg.setPosition(buttonCenterX, buttonCenterY);

    sf::FloatRect textBounds = m_energySellIcon.getLocalBounds();
    m_energySellIcon.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_energySellIcon.setPosition(buttonCenterX, buttonCenterY);
}

void HUD::update(int money, int energy, int maxEnergy, float envHealth, float totalTime) {
    m_valueMoney.setString(std::to_string(money) + "$");
    m_valueEnergy.setString(std::to_string(energy) + "/" + std::to_string(maxEnergy));
    m_valueEnv.setString(std::to_string(static_cast<int>(envHealth)) + "%");

    int m = static_cast<int>(totalTime) / 60;
    int s = static_cast<int>(totalTime) % 60;
    std::ostringstream oss;
    oss << "Czas gry: " << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
    m_labelTime.setString(oss.str());
}

void HUD::draw(sf::RenderWindow& window) {
    window.draw(m_labelMoney); window.draw(m_valueMoney);
    window.draw(m_labelEnergy); window.draw(m_valueEnergy);
    window.draw(m_labelEnv); window.draw(m_valueEnv);
    window.draw(m_labelTime);
    window.draw(m_energySellButtonBg);
    window.draw(m_energySellIcon);
}

const sf::FloatRect HUD::getEnergySellButtonBounds() const {
    return m_energySellButtonBg.getGlobalBounds();
}
