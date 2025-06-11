#include "HUD.h"
#include <iomanip>
#include <sstream>

// Definiujemy współrzędne X dla środków poszczególnych sekcji na dolnym pasku (Twoje wartości)
const float MONEY_SECTION_CENTER_X = 138.f;
const float ENERGY_SECTION_CENTER_X = 422.f;
const float ENV_SECTION_CENTER_X = 742.f;
const float TIME_SECTION_CENTER_X = 1053.f;

// Definiujemy współrzędne Y dla etykiet i wartości (Twoje wartości)
const float LABEL_Y = 730.f;
const float VALUE_Y = 758.f;
const float TIME_Y = 742.f;


HUD::HUD(sf::Font& font) : m_font(font) {
    sf::Color brown(101, 67, 33);

    m_labelMoney.setFont(m_font); m_labelMoney.setString("Stan konta"); m_labelMoney.setCharacterSize(24); m_labelMoney.setFillColor(brown);
    m_labelMoney.setOrigin(m_labelMoney.getLocalBounds().left + m_labelMoney.getLocalBounds().width / 2.f, 0.f);
    m_labelMoney.setPosition(MONEY_SECTION_CENTER_X, LABEL_Y);

    m_valueMoney.setFont(m_font); m_valueMoney.setCharacterSize(24); m_valueMoney.setFillColor(brown);

    m_labelEnergy.setFont(m_font); m_labelEnergy.setString("Energia"); m_labelEnergy.setCharacterSize(24); m_labelEnergy.setFillColor(brown);
    m_labelEnergy.setOrigin(m_labelEnergy.getLocalBounds().left + m_labelEnergy.getLocalBounds().width / 2.f, 0.f);
    m_labelEnergy.setPosition(ENERGY_SECTION_CENTER_X, LABEL_Y);

    m_valueEnergy.setFont(m_font); m_valueEnergy.setCharacterSize(24); m_valueEnergy.setFillColor(brown);

    m_labelEnv.setFont(m_font); m_labelEnv.setString("Stan srodowiska"); m_labelEnv.setCharacterSize(24); m_labelEnv.setFillColor(brown);
    m_labelEnv.setOrigin(m_labelEnv.getLocalBounds().left + m_labelEnv.getLocalBounds().width / 2.f, 0.f);
    m_labelEnv.setPosition(ENV_SECTION_CENTER_X, LABEL_Y);

    m_valueEnv.setFont(m_font); m_valueEnv.setCharacterSize(24); m_valueEnv.setFillColor(brown);

    m_labelTime.setFont(m_font); m_labelTime.setCharacterSize(30); m_labelTime.setFillColor(brown);

    m_energySellButtonBg.setRadius(15.f);
    m_energySellButtonBg.setFillColor(sf::Color(90, 160, 90));
    m_energySellIcon.setFont(m_font);
    m_energySellIcon.setString("$");
    m_energySellIcon.setCharacterSize(24);
    m_energySellIcon.setFillColor(sf::Color(255, 165, 0));
    m_energySellIcon.setStyle(sf::Text::Bold);

    float buttonCenterX = ENERGY_SECTION_CENTER_X + (m_labelEnergy.getLocalBounds().width / 2.f) + 30.f;
    float buttonCenterY = LABEL_Y + m_labelEnergy.getLocalBounds().height / 2.f + 5.f;

    m_energySellButtonBg.setOrigin(15.f, 15.f);
    m_energySellButtonBg.setPosition(buttonCenterX, buttonCenterY);

    sf::FloatRect textBounds = m_energySellIcon.getLocalBounds();
    m_energySellIcon.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_energySellIcon.setPosition(buttonCenterX, buttonCenterY);
}

void HUD::update(int money, int energy, int maxEnergy, float envHealth, float totalTime) {
    m_valueMoney.setString(std::to_string(money) + "$");
    m_valueMoney.setOrigin(m_valueMoney.getLocalBounds().left + m_valueMoney.getLocalBounds().width / 2.f, 0.f);
    m_valueMoney.setPosition(MONEY_SECTION_CENTER_X, VALUE_Y);

    m_valueEnergy.setString(std::to_string(energy) + "/" + std::to_string(maxEnergy));
    m_valueEnergy.setOrigin(m_valueEnergy.getLocalBounds().left + m_valueEnergy.getLocalBounds().width / 2.f, 0.f);
    m_valueEnergy.setPosition(ENERGY_SECTION_CENTER_X, VALUE_Y);

    m_valueEnv.setString(std::to_string(static_cast<int>(envHealth)) + "%");
    m_valueEnv.setOrigin(m_valueEnv.getLocalBounds().left + m_valueEnv.getLocalBounds().width / 2.f, 0.f);
    m_valueEnv.setPosition(ENV_SECTION_CENTER_X, VALUE_Y);

    int m = static_cast<int>(totalTime) / 60;
    int s = static_cast<int>(totalTime) % 60;
    std::ostringstream oss;
    oss << "Czas gry: " << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s;
    m_labelTime.setString(oss.str());
    m_labelTime.setOrigin(m_labelTime.getLocalBounds().left + m_labelTime.getLocalBounds().width / 2.f, 0.f);
    m_labelTime.setPosition(TIME_SECTION_CENTER_X, TIME_Y);
}

void HUD::draw(sf::RenderWindow& window) {
    // ZMIANA: Logika wyłączania podświetlenia i rysowania z tymczasową zmianą koloru
    if (m_isFlashingMoney && m_moneyFlashClock.getElapsedTime().asSeconds() > 0.5f) {
        m_isFlashingMoney = false;
    }

    window.draw(m_labelMoney);

    sf::Color originalColor = m_valueMoney.getFillColor();
    if (m_isFlashingMoney) {
        m_valueMoney.setFillColor(sf::Color::Green);
    }
    window.draw(m_valueMoney);
    m_valueMoney.setFillColor(originalColor); // Przywróć oryginalny kolor

    window.draw(m_labelEnergy); window.draw(m_valueEnergy);
    window.draw(m_labelEnv); window.draw(m_valueEnv);
    window.draw(m_labelTime);
    window.draw(m_energySellButtonBg);
    window.draw(m_energySellIcon);
}

const sf::FloatRect HUD::getEnergySellButtonBounds() const {
    return m_energySellButtonBg.getGlobalBounds();
}

// ZMIANA: Implementacja funkcji uruchamiającej podświetlenie
void HUD::flashMoney() {
    m_isFlashingMoney = true;
    m_moneyFlashClock.restart();
}
