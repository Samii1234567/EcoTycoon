#include "BuildingTooltip.h"
#include "Constants.h" // Potrzebne dla danych budynków

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY BUILDINGTOOLTIP
//
//  Zawiera logikę metod zadeklarowanych w BuildingTooltip.h.
//
// ===================================================================================

// Implementacja konstruktora.
BuildingTooltip::BuildingTooltip() : m_visible(false), m_upgradeClicked(false) {}

// Implementacja inicjalizacji domyślnego wyglądu dymka.
void BuildingTooltip::initialize(sf::Font& font) {
    // Konfiguracja tła.
    m_background.setSize({220.f, 140.f});
    m_background.setFillColor(sf::Color(255, 255, 255, 240)); // Lekko przezroczyste białe tło
    m_background.setOutlineColor(sf::Color::Black);
    m_background.setOutlineThickness(2.f);

    // Konfiguracja stylów tekstu.
    m_titleText.setFont(font);
    m_titleText.setCharacterSize(16);
    m_titleText.setStyle(sf::Text::Bold);
    m_titleText.setFillColor(sf::Color::Black);

    m_levelText.setFont(font);
    m_levelText.setCharacterSize(14);
    m_levelText.setFillColor(sf::Color::Black);

    m_infoText.setFont(font);
    m_infoText.setCharacterSize(14);
    m_infoText.setFillColor(sf::Color::Black);

    // Konfiguracja przycisku ulepszenia.
    m_upgradeButton.setSize({200.f, 30.f});
    m_upgradeButtonText.setFont(font);
    m_upgradeButtonText.setCharacterSize(14);
    m_upgradeButtonText.setFillColor(sf::Color::White);
}

// Implementacja wyświetlania dymka z danymi konkretnego budynku.
void BuildingTooltip::show(const PlacedObject& building, int playerMoney) {
    m_visible = true;
    m_upgradeClicked = false;

    // Zmienne do przechowywania danych pobranych na podstawie typu budynku.
    std::string name;
    std::string info;
    int cost = 0;
    float envEffect = 0.f;
    bool maxLevel = (building.level >= GameConstants::MAX_LEVEL);

    // Pobieranie danych z `GameConstants` w zależności od typu budynku.
    switch (building.typeId) {
    case GameConstants::SOLAR_PANEL_ID:
        name = "Panel sloneczny";
        info = "Produkcja: " + std::to_string(GameConstants::SOLAR_PANEL_DATA.value[building.level - 1]) + " E/s";
        envEffect = GameConstants::SOLAR_PANEL_DATA.envEffect[building.level - 1];
        if (!maxLevel) cost = GameConstants::SOLAR_PANEL_DATA.upgradeCost[building.level -1]; // Poprawka indeksu
        break;
    case GameConstants::WIND_TURBINE_ID:
        name = "Turbina wiatrowa";
        info = "Produkcja: " + std::to_string(GameConstants::WIND_TURBINE_DATA.value[building.level - 1]) + " E/s";
        envEffect = GameConstants::WIND_TURBINE_DATA.envEffect[building.level - 1];
        if (!maxLevel) cost = GameConstants::WIND_TURBINE_DATA.upgradeCost[building.level -1]; // Poprawka indeksu
        break;
    case GameConstants::ENERGY_STORAGE_ID:
        name = "Magazyn energii";
        info = "Pojemnosc: +" + std::to_string(GameConstants::STORAGE_DATA.value[building.level - 1]);
        envEffect = GameConstants::STORAGE_DATA.envEffect[building.level - 1];
        if (!maxLevel) cost = GameConstants::STORAGE_DATA.upgradeCost[building.level -1]; // Poprawka indeksu
        break;
    case GameConstants::AIR_FILTER_ID:
        name = "Stacja Filtrowania";
        info = "Koszt: " + std::to_string(GameConstants::AIR_FILTER_DATA.value[building.level - 1]) + " E/s";
        envEffect = GameConstants::AIR_FILTER_DATA.envEffect[building.level - 1];
        if (!maxLevel) cost = GameConstants::AIR_FILTER_DATA.upgradeCost[building.level -1]; // Poprawka indeksu
        break;
    }

    // Formatowanie i dodawanie informacji o wpływie na środowisko.
    int envEffectPercent = static_cast<int>(envEffect * 100);
    std::string sign = (envEffectPercent >= 0) ? "+" : "";
    info += "\nSrodowisko: " + sign + std::to_string(envEffectPercent) + "%/s";

    // Ustawienie sformatowanych tekstów.
    m_titleText.setString(name);
    m_levelText.setString("Poziom: " + std::to_string(building.level));
    m_infoText.setString(info);

    // Logika wyglądu przycisku ulepszenia.
    if (maxLevel) {
        m_upgradeButtonText.setString("Maksymalny poziom");
        m_upgradeButton.setFillColor(sf::Color(100, 100, 100)); // Szary przycisk
    } else {
        m_upgradeButtonText.setString("Ulepsz (" + std::to_string(cost) + "$)");
        // Zmiana koloru przycisku w zależności od tego, czy gracza stać na ulepszenie.
        if(playerMoney >= cost) {
            m_upgradeButton.setFillColor(sf::Color(90, 160, 90)); // Zielony
        } else {
            m_upgradeButton.setFillColor(sf::Color(200, 90, 90)); // Czerwony
        }
    }

    // Ustawienie pozycji wszystkich elementów dymka względem pozycji budynku.
    m_background.setPosition(building.sprite.getPosition().x - m_background.getSize().x / 2, building.sprite.getPosition().y - m_background.getSize().y - 10);
    m_titleText.setPosition(m_background.getPosition() + sf::Vector2f(10, 5));
    m_levelText.setPosition(m_background.getPosition() + sf::Vector2f(10, 30));
    m_infoText.setPosition(m_background.getPosition() + sf::Vector2f(10, 50));
    m_upgradeButton.setPosition(m_background.getPosition() + sf::Vector2f(10, 105));

    // Wyśrodkowanie tekstu na przycisku.
    sf::FloatRect textRect = m_upgradeButtonText.getLocalBounds();
    m_upgradeButtonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_upgradeButtonText.setPosition(m_upgradeButton.getPosition().x + m_upgradeButton.getSize().x / 2.0f, m_upgradeButton.getPosition().y + m_upgradeButton.getSize().y / 2.0f);
}

// Implementacja ukrywania dymka.
void BuildingTooltip::hide() {
    m_visible = false;
    m_upgradeClicked = false;
}

bool BuildingTooltip::isVisible() const {
    return m_visible;
}

// Implementacja obsługi zdarzeń.
void BuildingTooltip::handleEvent(const sf::Event& ev, sf::RenderWindow& window) {
    if (!m_visible) return;
    m_upgradeClicked = false;

    // Sprawdzamy, czy wciśnięto lewy przycisk myszy.
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        // Jeśli kliknięcie było wewnątrz przycisku "Ulepsz", ustawiamy flagę.
        if (m_upgradeButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            m_upgradeClicked = true;
        }
    }
}

// Implementacja rysowania.
void BuildingTooltip::draw(sf::RenderWindow& window) {
    // Rysujemy cokolwiek tylko wtedy, gdy dymek jest widoczny.
    if (!m_visible) return;
    window.draw(m_background);
    window.draw(m_titleText);
    window.draw(m_levelText);
    window.draw(m_infoText);
    window.draw(m_upgradeButton);
    window.draw(m_upgradeButtonText);
}

bool BuildingTooltip::isUpgradeClicked() const {
    return m_upgradeClicked;
}
