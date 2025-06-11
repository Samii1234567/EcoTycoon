#include "UIManager.h"
#include "Game.h"
#include "WeatherManager.h"
#include "Contract.h"
#include "Constants.h"
#include <sstream>
#include <iomanip>
#include <iostream>

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY UIMANAGER
//
//  Zawiera logikę metod zadeklarowanych w UIManager.h.
//  Najważniejsze części to konstruktor (inicjalizujący wygląd wszystkich paneli)
//  oraz metody handleEvent, update i draw, które zarządzają cyklem życia UI.
//
// ===================================================================================

// Implementacja konstruktora.
UIManager::UIManager(sf::Font& font, Game& game)
    : m_font(font), m_game(game), m_weatherBoardExpanded(false), m_showNotification(false),
    m_showingForecastDetails(false), m_viewedContractId(-1), m_showingActiveContracts(false)
{
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::Color textColor(80, 51, 20);

    loadWeatherTextures();

    // --- Inicjalizacja UI Pogody ---
    // Ta sekcja kodu tworzy i pozycjonuje wszystkie elementy związane z wyświetlaniem
    // informacji o pogodzie na górze ekranu.
    const float boardWidth = 300.f, boardX = (WIN_W - boardWidth) / 2.f;
    const float currentPanelHeight = 54.f, forecastPanelHeight = 54.f, panelSpacing = 4.f;
    m_weatherBoardBG.setSize({boardWidth, currentPanelHeight});
    m_weatherBoardBG.setFillColor(sf::Color(0,0,0,100));
    m_weatherBoardBG.setOutlineColor(sf::Color::White);
    m_weatherBoardBG.setOutlineThickness(1.f);
    m_weatherBoardBG.setPosition(boardX, 5.f);
    m_weatherIconSprite.setPosition(boardX + 30.f, m_weatherBoardBG.getPosition().y + currentPanelHeight / 2.f);
    m_weatherStatusText.setFont(m_font); m_weatherStatusText.setCharacterSize(18); m_weatherStatusText.setFillColor(sf::Color::White);
    m_infoButtonBG.setSize({50.f, 30.f}); m_infoButtonBG.setFillColor(sf::Color(80, 120, 180, 150)); m_infoButtonBG.setOutlineColor(sf::Color::White); m_infoButtonBG.setOutlineThickness(1.f);
    m_infoButtonBG.setPosition(boardX + boardWidth - 60.f, m_weatherBoardBG.getPosition().y + (currentPanelHeight - 30.f) / 2.f);
    m_infoButtonText.setFont(m_font); m_infoButtonText.setString("Info"); m_infoButtonText.setCharacterSize(16); m_infoButtonText.setFillColor(sf::Color::White);
    sf::FloatRect rct = m_infoButtonText.getLocalBounds();
    m_infoButtonText.setOrigin(rct.left + rct.width / 2.f, rct.top + rct.height / 2.f);
    m_infoButtonText.setPosition(m_infoButtonBG.getPosition().x + m_infoButtonBG.getSize().x / 2.f, m_infoButtonBG.getPosition().y + m_infoButtonBG.getSize().y / 2.f);
    m_forecastBoardBG.setSize({boardWidth, forecastPanelHeight}); m_forecastBoardBG.setFillColor(sf::Color(0,0,0,80)); m_forecastBoardBG.setOutlineColor(sf::Color(180,180,180)); m_forecastBoardBG.setOutlineThickness(1.f);
    m_forecastBoardBG.setPosition(boardX, m_weatherBoardBG.getPosition().y + currentPanelHeight + panelSpacing);
    m_forecastIconSprite.setPosition(boardX + 30.f, m_forecastBoardBG.getPosition().y + forecastPanelHeight / 2.f);
    float forecastTextBlockY = m_forecastBoardBG.getPosition().y + (forecastPanelHeight - 30.f) / 2.f;
    m_forecastLabelText.setFont(m_font); m_forecastLabelText.setString("Prognoza:"); m_forecastLabelText.setCharacterSize(14); m_forecastLabelText.setFillColor(sf::Color(200,200,200));
    m_forecastLabelText.setPosition(boardX + 60.f, forecastTextBlockY);
    m_forecastInfoText.setFont(m_font); m_forecastInfoText.setCharacterSize(16); m_forecastInfoText.setFillColor(sf::Color::White);
    m_forecastInfoText.setPosition(boardX + 60.f, forecastTextBlockY + 15.f);
    m_forecastInfoButtonBG.setSize({50.f, 30.f}); m_forecastInfoButtonBG.setFillColor(sf::Color(80, 120, 180, 150)); m_forecastInfoButtonBG.setOutlineColor(sf::Color::White); m_forecastInfoButtonBG.setOutlineThickness(1.f);
    m_forecastInfoButtonBG.setPosition(boardX + boardWidth - 60.f, m_forecastBoardBG.getPosition().y + (forecastPanelHeight - 30.f) / 2.f);
    m_forecastInfoButtonText.setFont(m_font); m_forecastInfoButtonText.setString("Info"); m_forecastInfoButtonText.setCharacterSize(16); m_forecastInfoButtonText.setFillColor(sf::Color::White);
    rct = m_forecastInfoButtonText.getLocalBounds();
    m_forecastInfoButtonText.setOrigin(rct.left + rct.width / 2.f, rct.top + rct.height / 2.f);
    m_forecastInfoButtonText.setPosition(m_forecastInfoButtonBG.getPosition().x + m_forecastInfoButtonBG.getSize().x / 2.f, m_forecastInfoButtonBG.getPosition().y + m_forecastInfoButtonBG.getSize().y / 2.f);
    m_weatherPanel.setSize({600, 250}); m_weatherPanel.setFillColor(sf::Color(40,40,60,240)); m_weatherPanel.setOutlineColor(sf::Color::White); m_weatherPanel.setOutlineThickness(2.f);
    m_weatherPanel.setPosition((WIN_W - 600.f)/2.f, (WIN_H - 250.f)/2.f);
    m_weatherPanelTitle.setFont(m_font); m_weatherPanelTitle.setString("Szczegoly Pogody"); m_weatherPanelTitle.setCharacterSize(32); m_weatherPanelTitle.setFillColor(sf::Color::White);
    m_weatherPanelTitle.setPosition(m_weatherPanel.getPosition().x + (m_weatherPanel.getSize().x - m_weatherPanelTitle.getGlobalBounds().width)/2.f, m_weatherPanel.getPosition().y + 10.f);
    m_weatherPanelCurrentLabel.setFont(m_font); m_weatherPanelCurrentLabel.setCharacterSize(24); m_weatherPanelCurrentLabel.setFillColor(sf::Color(200,200,200));
    m_weatherPanelCurrentLabel.setPosition(m_weatherPanel.getPosition().x + 20.f, m_weatherPanel.getPosition().y + 70.f);
    m_weatherPanelCurrentDesc.setFont(m_font); m_weatherPanelCurrentDesc.setCharacterSize(20); m_weatherPanelCurrentDesc.setFillColor(sf::Color::White);
    m_weatherPanelCurrentDesc.setPosition(m_weatherPanel.getPosition().x + 20.f, m_weatherPanel.getPosition().y + 110.f);

    // --- Inicjalizacja UI Kontraktów ---
    // Ta sekcja tworzy wszystkie elementy menu kontraktów, w tym zakładki,
    // listę, przyciski oraz szczegółowy widok kontraktu na zwoju.
    m_contractListBackground.setSize({600, 500}); m_contractListBackground.setFillColor({40, 40, 60, 240}); m_contractListBackground.setOutlineColor(sf::Color::White); m_contractListBackground.setOutlineThickness(2.f);
    m_contractListBackground.setPosition((WIN_W - 600.f) / 2.f, (WIN_H - 500.f) / 2.f);
    float tabWidth = 150.f, tabHeight = 35.f;
    sf::Vector2f panelPos = m_contractListBackground.getPosition();
    m_pendingTabButton.setSize({tabWidth, tabHeight}); m_pendingTabButton.setPosition(panelPos.x, panelPos.y - tabHeight);
    m_activeTabButton.setSize({tabWidth, tabHeight}); m_activeTabButton.setPosition(panelPos.x + tabWidth, panelPos.y - tabHeight);
    m_pendingTabText.setFont(m_font); m_pendingTabText.setString("Oczekujace"); m_pendingTabText.setCharacterSize(18);
    m_activeTabText.setFont(m_font); m_activeTabText.setString("Aktywne"); m_activeTabText.setCharacterSize(18);
    auto centerText = [](sf::Text& text, const sf::RectangleShape& button) {
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        text.setPosition(button.getPosition().x + button.getSize().x / 2.f, button.getPosition().y + button.getSize().y / 2.f);
    };
    centerText(m_pendingTabText, m_pendingTabButton);
    centerText(m_activeTabText, m_activeTabButton);
    m_contractListTitle.setFont(m_font); m_contractListTitle.setString("Dostepne Kontrakty"); m_contractListTitle.setCharacterSize(32); m_contractListTitle.setStyle(sf::Text::Bold); m_contractListTitle.setFillColor(sf::Color::White);
    rct = m_contractListTitle.getLocalBounds();
    m_contractListTitle.setOrigin(rct.left + rct.width/2.f, rct.top);
    m_contractListTitle.setPosition(m_contractListBackground.getPosition().x + m_contractListBackground.getSize().x / 2.f, m_contractListBackground.getPosition().y + 20.f);
    m_noContractsText.setFont(m_font); m_noContractsText.setString("Brak nowych ofert"); m_noContractsText.setCharacterSize(30); m_noContractsText.setFillColor(sf::Color(180,180,180));
    rct = m_noContractsText.getLocalBounds();
    m_noContractsText.setOrigin(rct.left + rct.width / 2.f, rct.top + rct.height / 2.f);
    m_noContractsText.setPosition(m_contractListBackground.getPosition().x + m_contractListBackground.getSize().x / 2.f, m_contractListBackground.getPosition().y + m_contractListBackground.getSize().y / 2.f);
    m_contractListCloseButton.setSize({200, 50}); m_contractListCloseButton.setFillColor({180, 80, 80});
    m_contractListCloseButton.setPosition(m_contractListBackground.getPosition().x + (m_contractListBackground.getSize().x - 200.f)/2.f, m_contractListBackground.getPosition().y + m_contractListBackground.getSize().y - 70.f);
    m_contractListCloseText.setFont(m_font); m_contractListCloseText.setString("Zamknij"); m_contractListCloseText.setCharacterSize(24);
    rct = m_contractListCloseText.getLocalBounds();
    m_contractListCloseText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_contractListCloseText.setPosition(m_contractListCloseButton.getPosition().x + m_contractListCloseButton.getSize().x / 2.f, m_contractListCloseButton.getPosition().y + m_contractListCloseButton.getSize().y / 2.f);
    if (!m_scrollTexture.loadFromFile("images/contract_paper.png")) { std::cerr << "Nie mozna wczytac contract_paper.png\n"; }
    m_scrollSprite.setTexture(m_scrollTexture); m_scrollSprite.setPosition((WIN_W - m_scrollTexture.getSize().x) / 2.f, (WIN_H - m_scrollTexture.getSize().y) / 2.f);
    sf::Vector2f scrollPos = m_scrollSprite.getPosition(); float scrollWidth = m_scrollSprite.getGlobalBounds().width; float textLeftMargin = scrollPos.x + 120.f;
    m_contractCityText.setFont(m_font); m_contractCityText.setCharacterSize(24); m_contractCityText.setFillColor(textColor); m_contractCityText.setPosition(textLeftMargin, scrollPos.y + 150.f);
    m_contractDemandText.setFont(m_font); m_contractDemandText.setCharacterSize(24); m_contractDemandText.setFillColor(textColor); m_contractDemandText.setPosition(textLeftMargin, scrollPos.y + 200.f);
    m_contractPaymentText.setFont(m_font); m_contractPaymentText.setCharacterSize(24); m_contractPaymentText.setFillColor(textColor); m_contractPaymentText.setPosition(textLeftMargin, scrollPos.y + 250.f);
    m_acceptButton.setSize({180, 50}); m_acceptButton.setFillColor({90, 160, 90, 200}); m_acceptButton.setPosition(scrollPos.x + 80.f, scrollPos.y + 350.f);
    m_acceptText.setFont(m_font); m_acceptText.setString("Akceptuj"); m_acceptText.setCharacterSize(24); m_acceptText.setFillColor(sf::Color::White);
    rct = m_acceptText.getLocalBounds(); m_acceptText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_acceptText.setPosition(m_acceptButton.getPosition().x + m_acceptButton.getSize().x / 2.f, m_acceptButton.getPosition().y + m_acceptButton.getSize().y / 2.f);
    m_rejectButton.setSize({180, 50}); m_rejectButton.setFillColor({200, 90, 90, 200}); m_rejectButton.setPosition(scrollPos.x + scrollWidth - 80.f - 180.f, scrollPos.y + 350.f);
    m_rejectText.setFont(m_font); m_rejectText.setString("Odrzuc"); m_rejectText.setCharacterSize(24); m_rejectText.setFillColor(sf::Color::White);
    rct = m_rejectText.getLocalBounds(); m_rejectText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_rejectText.setPosition(m_rejectButton.getPosition().x + m_rejectButton.getSize().x / 2.f, m_rejectButton.getPosition().y + m_rejectButton.getSize().y / 2.f);
    m_scrollCloseButton.setFont(m_font); m_scrollCloseButton.setString("X"); m_scrollCloseButton.setCharacterSize(40); m_scrollCloseButton.setStyle(sf::Text::Bold); m_scrollCloseButton.setFillColor(sf::Color::Red);
    m_scrollCloseButton.setPosition(scrollPos.x + scrollWidth - 80.f, scrollPos.y + 45.f);

    // --- Inicjalizacja wspólnych paneli (Naprawa, Energia, Opcje) ---
    // Ta sekcja tworzy panele, które są używane w różnych momentach gry.
    m_panelOverlay.setSize({(float)WIN_W, (float)WIN_H}); m_panelOverlay.setFillColor({0,0,0,100});
    m_repairPanel.setSize({400, 200}); m_repairPanel.setFillColor({40, 40, 60, 240}); m_repairPanel.setOutlineColor(sf::Color::White); m_repairPanel.setOutlineThickness(2.f);
    m_repairPanel.setPosition((WIN_W - 400.f) / 2.f, (WIN_H - 200.f) / 2.f);
    m_repairTitle.setFont(m_font); m_repairTitle.setString("Naprawa budynku"); m_repairTitle.setCharacterSize(28); m_repairTitle.setFillColor(sf::Color::White);
    m_repairTitle.setPosition(m_repairPanel.getPosition().x + (m_repairPanel.getSize().x - m_repairTitle.getGlobalBounds().width) / 2.f, m_repairPanel.getPosition().y + 15.f);
    m_repairInfoText.setFont(m_font); m_repairInfoText.setCharacterSize(20); m_repairInfoText.setFillColor(sf::Color::White);
    m_repairInfoText.setPosition(m_repairPanel.getPosition().x + 20.f, m_repairPanel.getPosition().y + 70.f);
    m_repairConfirmButton.setSize({150, 40}); m_repairConfirmButton.setFillColor(sf::Color(80, 180, 80));
    m_repairConfirmButton.setPosition(m_repairPanel.getPosition().x + 40.f, m_repairPanel.getPosition().y + 130.f);
    m_repairConfirmText.setString("Napraw"); m_repairConfirmText.setFont(m_font); m_repairConfirmText.setCharacterSize(20);
    rct = m_repairConfirmText.getLocalBounds();
    m_repairConfirmText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_repairConfirmText.setPosition(m_repairConfirmButton.getPosition().x + m_repairConfirmButton.getSize().x / 2.f, m_repairConfirmButton.getPosition().y + m_repairConfirmButton.getSize().y / 2.f);
    m_repairCancelButton.setSize({150, 40}); m_repairCancelButton.setFillColor(sf::Color(180, 80, 80));
    m_repairCancelButton.setPosition(m_repairPanel.getPosition().x + m_repairPanel.getSize().x - 190.f, m_repairPanel.getPosition().y + 130.f);
    m_repairCancelText.setString("Anuluj"); m_repairCancelText.setFont(m_font); m_repairCancelText.setCharacterSize(20);
    rct = m_repairCancelText.getLocalBounds();
    m_repairCancelText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_repairCancelText.setPosition(m_repairCancelButton.getPosition().x + m_repairCancelButton.getSize().x / 2.f, m_repairCancelButton.getPosition().y + m_repairCancelButton.getSize().y / 2.f);
    m_energyPanel.setSize({500.f, 300.f}); m_energyPanel.setFillColor({40, 40, 60, 220}); m_energyPanel.setOutlineColor({120, 120, 150, 255}); m_energyPanel.setOutlineThickness(2.f);
    m_energyPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 300.f) / 2.f);
    m_energyMenuTitle.setString("Zarzadzanie Energia"); m_energyMenuTitle.setFont(m_font); m_energyMenuTitle.setCharacterSize(32); m_energyMenuTitle.setFillColor(sf::Color::White);
    m_energyMenuTitle.setPosition(m_energyPanel.getPosition().x + (m_energyPanel.getSize().x - m_energyMenuTitle.getLocalBounds().width) / 2.f, m_energyPanel.getPosition().y + 20.f);
    m_energyInfoText.setFont(m_font); m_energyInfoText.setCharacterSize(22); m_energyInfoText.setFillColor(sf::Color::White); m_energyInfoText.setPosition(m_energyPanel.getPosition().x + 30.f, m_energyPanel.getPosition().y + 80.f);
    m_energyValueText.setFont(m_font); m_energyValueText.setCharacterSize(22); m_energyValueText.setFillColor(sf::Color(180, 255, 180));
    m_energyValueText.setPosition(m_energyPanel.getPosition().x + 30.f, m_energyPanel.getPosition().y + 110.f);
    m_sellButton.setSize({200.f, 50.f}); m_sellButton.setFillColor(sf::Color(80, 180, 80)); m_sellButton.setPosition(m_energyPanel.getPosition().x + 50.f, m_energyPanel.getPosition().y + 190.f);
    m_sellButtonText.setString("Sprzedaj"); m_sellButtonText.setFont(m_font); m_sellButtonText.setCharacterSize(24);
    m_sellButtonText.setPosition(m_sellButton.getPosition().x + (m_sellButton.getSize().x - m_sellButtonText.getLocalBounds().width)/2.f, m_sellButton.getPosition().y + 10.f);
    m_closeButton.setSize({200.f, 50.f}); m_closeButton.setFillColor(sf::Color(180, 80, 80)); m_closeButton.setPosition(m_energyPanel.getPosition().x + m_energyPanel.getSize().x - 250.f, m_energyPanel.getPosition().y + 190.f);
    m_closeButtonText.setString("Zamknij"); m_closeButtonText.setFont(m_font); m_closeButtonText.setCharacterSize(24);
    m_closeButtonText.setPosition(m_closeButton.getPosition().x + (m_closeButton.getSize().x - m_closeButtonText.getLocalBounds().width)/2.f, m_closeButton.getPosition().y + 10.f);
    m_optionsPanel.setSize({500.f, 400.f}); m_optionsPanel.setFillColor({40, 40, 60, 220}); m_optionsPanel.setOutlineColor({120, 120, 150, 255}); m_optionsPanel.setOutlineThickness(2.f);
    m_optionsPanel.setPosition((WIN_W - 500.f) / 2.f, (WIN_H - 400.f) / 2.f);
    m_optionsMenuTitle.setString("Opcje"); m_optionsMenuTitle.setFont(m_font); m_optionsMenuTitle.setCharacterSize(32); m_optionsMenuTitle.setFillColor(sf::Color::White);
    m_optionsMenuTitle.setPosition(m_optionsPanel.getPosition().x + (m_optionsPanel.getSize().x - m_optionsMenuTitle.getLocalBounds().width) / 2.f, m_optionsPanel.getPosition().y + 20.f);
    m_musicLabel.setString("Muzyka"); m_musicLabel.setFont(m_font); m_musicLabel.setCharacterSize(24); m_musicLabel.setFillColor(sf::Color::White);
    m_musicLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 100.f);
    m_musicSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 140.f, 400.f, 6.f, 15.f);
    m_sfxLabel.setString("Efekty dzwiekowe"); m_sfxLabel.setFont(m_font); m_sfxLabel.setCharacterSize(24); m_sfxLabel.setFillColor(sf::Color::White);
    m_sfxLabel.setPosition(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 200.f);
    m_sfxSlider = std::make_unique<Slider>(m_optionsPanel.getPosition().x + 50.f, m_optionsPanel.getPosition().y + 240.f, 400.f, 6.f, 15.f);
    m_applyButton.setSize({200.f, 50.f}); m_applyButton.setFillColor(sf::Color(80, 120, 180));
    m_applyButton.setPosition(m_optionsPanel.getPosition().x + 40.f, m_optionsPanel.getPosition().y + 320.f);
    m_applyButtonText.setString("Zastosuj"); m_applyButtonText.setFont(m_font); m_applyButtonText.setCharacterSize(24);
    m_applyButtonText.setPosition(m_applyButton.getPosition().x + (m_applyButton.getSize().x - m_applyButtonText.getLocalBounds().width)/2.f, m_applyButton.getPosition().y + 10.f);
    m_cancelButton.setSize({200.f, 50.f}); m_cancelButton.setFillColor(sf::Color(180, 80, 80));
    m_cancelButton.setPosition(m_optionsPanel.getPosition().x + m_optionsPanel.getSize().x - 240.f, m_optionsPanel.getPosition().y + 320.f);
    m_cancelButtonText.setString("Anuluj"); m_cancelButtonText.setFont(m_font); m_cancelButtonText.setCharacterSize(24);
    m_cancelButtonText.setPosition(m_cancelButton.getPosition().x + (m_cancelButton.getSize().x - m_cancelButtonText.getLocalBounds().width)/2.f, m_cancelButton.getPosition().y + 10.f);
    m_notificationText.setFont(m_font); m_notificationText.setCharacterSize(32); m_notificationText.setFillColor(sf::Color::Red); m_notificationText.setOutlineColor(sf::Color::Black); m_notificationText.setOutlineThickness(2.f);
    m_notificationText.setPosition(WIN_W / 2.f, 125.f);

    // --- Inicjalizacja panelu informacyjnego o pogodzie ---
    // Ta sekcja tworzy duży, przewijany panel z opisami wszystkich zjawisk pogodowych.
    m_weatherInfoPanel.setSize({800.f, 600.f}); m_weatherInfoPanel.setFillColor({20, 20, 40, 245}); m_weatherInfoPanel.setOutlineColor(sf::Color::White); m_weatherInfoPanel.setOutlineThickness(2.f);
    m_weatherInfoPanel.setPosition((WIN_W - 800.f) / 2.f, (WIN_H - 600.f) / 2.f);
    m_weatherInfoTitle.setFont(m_font); m_weatherInfoTitle.setString("Informacje o pogodzie"); m_weatherInfoTitle.setCharacterSize(36); m_weatherInfoTitle.setFillColor(sf::Color::White);
    m_weatherInfoTitle.setPosition(m_weatherInfoPanel.getPosition().x + (m_weatherInfoPanel.getSize().x - m_weatherInfoTitle.getGlobalBounds().width) / 2.f, m_weatherInfoPanel.getPosition().y + 15.f);
    float contentStartY = m_weatherInfoPanel.getPosition().y + 65.f; float currentY = contentStartY; float paddingX = m_weatherInfoPanel.getPosition().x + 25.f;
    for (int i = 0; i <= static_cast<int>(WeatherType::Smog); ++i) {
        WeatherType type = static_cast<WeatherType>(i);
        WeatherInfoEntry entry;
        entry.name.setFont(m_font); entry.name.setString(getWeatherName(type)); entry.name.setCharacterSize(22); entry.name.setStyle(sf::Text::Bold); entry.name.setFillColor(sf::Color::White);
        entry.name.setPosition(paddingX, currentY); currentY += 30;
        entry.description.setFont(m_font); entry.description.setString(getWeatherDescription(type)); entry.description.setCharacterSize(18); entry.description.setFillColor(sf::Color(200, 200, 200));
        entry.description.setPosition(paddingX, currentY); currentY += entry.description.getGlobalBounds().height + 30;
        m_weatherInfoEntries.push_back(entry);
    }
    m_weatherInfoContentHeight = (currentY - contentStartY);
    m_weatherInfoCloseButton.setSize({150.f, 40.f}); m_weatherInfoCloseButton.setFillColor({180, 80, 80});
    m_weatherInfoCloseButton.setPosition(m_weatherInfoPanel.getPosition().x + (m_weatherInfoPanel.getSize().x - 150.f) / 2.f, m_weatherInfoPanel.getPosition().y + m_weatherInfoPanel.getSize().y - 60.f);
    m_weatherInfoCloseButtonText.setFont(m_font); m_weatherInfoCloseButtonText.setString("Zamknij"); m_weatherInfoCloseButtonText.setCharacterSize(20);
    rct = m_weatherInfoCloseButtonText.getLocalBounds();
    m_weatherInfoCloseButtonText.setOrigin(rct.left + rct.width/2.f, rct.top + rct.height/2.f);
    m_weatherInfoCloseButtonText.setPosition(m_weatherInfoCloseButton.getPosition().x + m_weatherInfoCloseButton.getSize().x / 2.f, m_weatherInfoCloseButton.getPosition().y + m_weatherInfoCloseButton.getSize().y / 2.f);
    float contentAreaHeight = m_weatherInfoPanel.getSize().y - 125.f;
    m_scrollbarBg.setSize({10.f, contentAreaHeight}); m_scrollbarBg.setFillColor(sf::Color(0, 0, 0, 100));
    m_scrollbarBg.setPosition(m_weatherInfoPanel.getPosition().x + m_weatherInfoPanel.getSize().x - 20.f, m_weatherInfoPanel.getPosition().y + 60.f);
    m_scrollbarHandle.setFillColor(sf::Color(100, 100, 100));
}

// Implementacja wczytywania tekstur pogody.
void UIManager::loadWeatherTextures() {
    for (int i = 0; i <= static_cast<int>(WeatherType::Smog); ++i) {
        WeatherType type = static_cast<WeatherType>(i);
        std::string filename = getWeatherIconFilename(type);
        if (!filename.empty()) {
            if (!m_weatherTextures[type].loadFromFile(filename)) {
                std::cerr << "Nie udalo sie wczytac tekstury pogody: " << filename << std::endl;
            }
        }
    }
}

// Implementacja głównej metody obsługi zdarzeń.
void UIManager::handleEvent(const sf::Event& ev, sf::RenderWindow& window, GameState& currentState) {
    // Panel naprawy ma najwyższy priorytet - jeśli jest aktywny, inne interakcje są blokowane.
    if (m_game.getRepairTarget() != nullptr) {
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_repairConfirmButton.getGlobalBounds().contains(pos)) m_game.confirmRepair();
            else if (m_repairCancelButton.getGlobalBounds().contains(pos)) m_game.setRepairTarget(nullptr);
        }
        return; // Zakończ dalsze przetwarzanie
    }

    // Obsługa kliknięć w górny pasek pogody.
    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
        if (m_infoButtonBG.getGlobalBounds().contains(pos) && currentState == GameState::Playing) {
            m_showingForecastDetails = false; m_weatherBoardExpanded = true; return;
        }
        if (m_forecastInfoButtonBG.getGlobalBounds().contains(pos) && currentState == GameState::Playing) {
            if (!m_game.getWeatherManager().getForecast().empty()) { m_showingForecastDetails = true; m_weatherBoardExpanded = true; }
            return;
        }
        // Zamykanie panelu szczegółów pogody po kliknięciu poza nim.
        if (m_weatherBoardExpanded && !m_weatherPanel.getGlobalBounds().contains(pos)) {
            m_weatherBoardExpanded = false; return;
        }
    }

    // Przetwarzanie zdarzeń w zależności od aktywnego menu/panelu.
    switch (currentState) {
    case GameState::EnergyMenu:
        if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) currentState = GameState::Playing;
        if (ev.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_sellButton.getGlobalBounds().contains(pos)) {
                if (m_game.currentEnergy > 0) {
                    m_game.getAudioManager().play(SoundEffect::CashRegister);
                    m_game.currentMoney += m_game.currentEnergy * GameConstants::ENERGY_SELL_PRICE;
                    m_game.currentEnergy = 0;
                    m_game.getHUD().flashMoney();
                }
                currentState = GameState::Playing;
            } else if (m_closeButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                currentState = GameState::Playing;
            }
        }
        break;
    case GameState::InGameOptionsMenu:
        m_musicSlider->handleEvent(ev, window); m_sfxSlider->handleEvent(ev, window);
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) { syncSliderValues(); currentState = GameState::Playing; }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_applyButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                m_game.musicVolume = m_musicSlider->getValue();
                m_game.sfxVolume = m_sfxSlider->getValue();
                currentState = GameState::Playing;
            } else if (m_cancelButton.getGlobalBounds().contains(pos)) {
                m_game.getAudioManager().play(SoundEffect::ButtonClick);
                syncSliderValues();
                currentState = GameState::Playing;
            }
        }
        break;
    case GameState::ContractsMenu:
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
            if (m_viewedContractId != -1) m_viewedContractId = -1; // Jeśli oglądamy szczegóły, cofnij do listy
            else currentState = GameState::Playing; // Jeśli jesteśmy na liście, zamknij menu
        }
        if (ev.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            // Logika dla widoku szczegółów kontraktu
            if (m_viewedContractId != -1) {
                const Contract* viewed = m_game.getContractManager().getContractById(m_viewedContractId);
                if (viewed) {
                    if (viewed->status == ContractStatus::Pending) {
                        bool canAccept = m_game.getNetEnergyPerSecond() >= viewed->energyPerSecond;
                        if (canAccept && m_acceptButton.getGlobalBounds().contains(pos)) { m_game.getContractManager().acceptContract(m_viewedContractId); m_viewedContractId = -1; }
                        else if (!canAccept && m_acceptButton.getGlobalBounds().contains(pos)) { showNotification("Niewystarczajaca produkcja energii!"); }
                        else if (m_rejectButton.getGlobalBounds().contains(pos)) { m_game.getContractManager().rejectContract(m_viewedContractId); m_viewedContractId = -1; }
                    } else if (viewed->status == ContractStatus::Active) {
                        if (m_acceptButton.getGlobalBounds().contains(pos)) { m_game.getContractManager().cancelContract(m_viewedContractId); showNotification("Zrezygnowano z kontraktu."); m_viewedContractId = -1; }
                    }
                }
                if (m_scrollCloseButton.getGlobalBounds().contains(pos)) m_viewedContractId = -1;
            } else { // Logika dla widoku listy kontraktów
                if (m_pendingTabButton.getGlobalBounds().contains(pos)) { m_showingActiveContracts = false; return; }
                if (m_activeTabButton.getGlobalBounds().contains(pos)) { m_showingActiveContracts = true; return; }
                if (m_contractListCloseButton.getGlobalBounds().contains(pos)) { currentState = GameState::Playing; return; }
                std::vector<Contract> contracts = m_showingActiveContracts ? m_game.getContractManager().getActiveContracts() : m_game.getContractManager().getPendingContracts();
                float itemY = m_contractListBackground.getPosition().y + 80.f;
                for(const auto& contract : contracts) {
                    sf::FloatRect itemBounds(m_contractListBackground.getPosition().x, itemY, m_contractListBackground.getSize().x, 40.f);
                    if (itemBounds.contains(pos)) { m_viewedContractId = contract.id; break; }
                    itemY += 45.f;
                }
            }
        }
        break;
    case GameState::WeatherInfo:
        if ((ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)) {
            currentState = GameState::PauseMenu; m_weatherInfoScrollOffset = 0;
        }
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f pos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
            if (m_weatherInfoCloseButton.getGlobalBounds().contains(pos)) { currentState = GameState::PauseMenu; m_weatherInfoScrollOffset = 0; }
        }
        // Obsługa przewijania kółkiem myszy
        if (ev.type == sf::Event::MouseWheelScrolled) {
            if (ev.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                m_weatherInfoScrollOffset -= ev.mouseWheelScroll.delta * 20;
                float contentAreaHeight = m_weatherInfoPanel.getSize().y - 125.f;
                float maxScroll = m_weatherInfoContentHeight - contentAreaHeight;
                if (maxScroll < 0) maxScroll = 0;
                // Ograniczenie przewijania, aby nie wyjść poza treść
                if (m_weatherInfoScrollOffset < 0) m_weatherInfoScrollOffset = 0;
                if (m_weatherInfoScrollOffset > maxScroll) m_weatherInfoScrollOffset = maxScroll;
            }
        }
        break;
    default:
        break;
    }
}

// Implementacja aktualizacji stanu UI.
void UIManager::update(const WeatherManager& weatherManager) {
    // Ukrywanie powiadomienia po 2 sekundach.
    if (m_showNotification && m_notificationClock.getElapsedTime().asSeconds() > 2.0f) {
        m_showNotification = false;
    }

    // Aktualizacja górnego paska pogody.
    WeatherType currentWeather = weatherManager.getCurrentWeather();
    if (m_weatherTextures.count(currentWeather)) {
        const sf::Texture& tex = m_weatherTextures.at(currentWeather);
        m_weatherIconSprite.setTexture(tex, true);
        m_weatherIconSprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
        m_weatherIconSprite.setScale(36.f / tex.getSize().x, 36.f / tex.getSize().y);
    }
    float timerValue = weatherManager.getWeatherTimer();
    int minutes = static_cast<int>(timerValue) / 60;
    int seconds = static_cast<int>(timerValue) % 60;
    std::ostringstream oss_timer;
    oss_timer << " (" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds << ")";
    m_weatherStatusText.setString(getWeatherName(currentWeather) + oss_timer.str());
    sf::FloatRect textBounds = m_weatherStatusText.getLocalBounds();
    m_weatherStatusText.setOrigin(textBounds.left, textBounds.top + textBounds.height / 2.f);
    m_weatherStatusText.setPosition(m_weatherBoardBG.getPosition().x + 60.f, m_weatherBoardBG.getPosition().y + m_weatherBoardBG.getSize().y / 2.f);

    const auto& forecast = weatherManager.getForecast();
    if (!forecast.empty()) {
        WeatherType nextWeather = forecast.front();
        m_forecastInfoText.setString(getWeatherName(nextWeather));
        if (m_weatherTextures.count(nextWeather)) {
            const sf::Texture& tex = m_weatherTextures.at(nextWeather);
            m_forecastIconSprite.setTexture(tex, true);
            m_forecastIconSprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
            m_forecastIconSprite.setScale(30.f / tex.getSize().x, 30.f / tex.getSize().y);
        }
    }

    // Aktualizacja panelu szczegółów pogody.
    if (m_weatherBoardExpanded) {
        if (m_showingForecastDetails) {
            if (!forecast.empty()) {
                WeatherType forecastWeather = forecast.front();
                m_weatherPanelCurrentLabel.setString("Prognoza: " + getWeatherName(forecastWeather));
                m_weatherPanelCurrentDesc.setString(getWeatherDescription(forecastWeather));
            }
        } else {
            m_weatherPanelCurrentLabel.setString("Aktualna pogoda: " + getWeatherName(currentWeather));
            m_weatherPanelCurrentDesc.setString(getWeatherDescription(currentWeather));
        }
    }

    // Aktualizacja panelu naprawy.
    if (PlacedObject* target = m_game.getRepairTarget()) {
        int repairCost = target->price / 4;
        m_repairInfoText.setString("Koszt naprawy: " + std::to_string(repairCost) + "$");
    }

    // Aktualizacja panelu energii.
    std::ostringstream oss;
    oss << "Zgromadzona energia: " << m_game.currentEnergy << " / " << m_game.maxEnergy;
    m_energyInfoText.setString(oss.str());
    oss.str(""); oss.clear();
    oss << "Wartosc sprzedazy: " << (m_game.currentEnergy * GameConstants::ENERGY_SELL_PRICE) << "$";
    m_energyValueText.setString(oss.str());
}

// Implementacja głównej metody rysującej.
void UIManager::draw(sf::RenderWindow& window, GameState currentState) {
    // Rysowanie pogody jest zawsze aktywne podczas gry.
    drawWeatherUI(window);

    // Wywołanie odpowiedniej metody rysującej w zależności od stanu gry.
    if (currentState == GameState::EnergyMenu) drawEnergyMenu(window);
    else if (currentState == GameState::InGameOptionsMenu) drawOptionsMenu(window);
    else if (currentState == GameState::ContractsMenu) drawContractsMenu(window);
    else if (currentState == GameState::WeatherInfo) drawWeatherInfoPanel(window);

    // Te elementy mogą być rysowane na wierzchu innych paneli.
    drawRepairPanel(window);
    if (m_showNotification) window.draw(m_notificationText);
}

// --- Implementacje prywatnych metod rysujących ---

void UIManager::drawWeatherUI(sf::RenderWindow& window) {
    window.draw(m_weatherBoardBG); window.draw(m_weatherIconSprite); window.draw(m_weatherStatusText);
    window.draw(m_infoButtonBG); window.draw(m_infoButtonText);
    if (!m_game.getWeatherManager().getForecast().empty()) {
        window.draw(m_forecastBoardBG); window.draw(m_forecastIconSprite); window.draw(m_forecastLabelText);
        window.draw(m_forecastInfoText); window.draw(m_forecastInfoButtonBG); window.draw(m_forecastInfoButtonText);
    }
    if (m_weatherBoardExpanded) {
        window.draw(m_weatherPanel); window.draw(m_weatherPanelTitle);
        window.draw(m_weatherPanelCurrentLabel); window.draw(m_weatherPanelCurrentDesc);
    }
}

void UIManager::drawRepairPanel(sf::RenderWindow& window) {
    if (m_game.getRepairTarget() != nullptr) {
        window.draw(m_panelOverlay); window.draw(m_repairPanel); window.draw(m_repairTitle);
        window.draw(m_repairInfoText); window.draw(m_repairConfirmButton); window.draw(m_repairConfirmText);
        window.draw(m_repairCancelButton); window.draw(m_repairCancelText);
    }
}

void UIManager::drawContractsMenu(sf::RenderWindow& window) {
    window.draw(m_panelOverlay);
    if (m_viewedContractId != -1) {
        drawContractDetail(window);
    } else {
        sf::Color activeTabColor(70, 70, 90, 240), inactiveTabColor(40, 40, 60, 240);
        m_pendingTabButton.setFillColor(m_showingActiveContracts ? inactiveTabColor : activeTabColor);
        m_activeTabButton.setFillColor(m_showingActiveContracts ? activeTabColor : inactiveTabColor);
        window.draw(m_pendingTabButton); window.draw(m_activeTabButton);
        window.draw(m_pendingTabText); window.draw(m_activeTabText);
        if (m_showingActiveContracts) drawActiveContractList(window);
        else drawContractList(window);
    }
}

void UIManager::drawContractList(sf::RenderWindow& window) {
    window.draw(m_contractListBackground);
    m_contractListTitle.setString("Oczekujace Kontrakty");
    window.draw(m_contractListTitle);
    auto pending = m_game.getContractManager().getPendingContracts();
    if (pending.empty()) {
        m_noContractsText.setString("Brak nowych ofert");
        window.draw(m_noContractsText);
    } else {
        float startX = m_contractListBackground.getPosition().x + 20.f, itemY = m_contractListBackground.getPosition().y + 80.f;
        sf::Text itemText("", m_font, 22);
        for(const auto& contract : pending) {
            bool canAccept = m_game.getNetEnergyPerSecond() >= contract.energyPerSecond;
            itemText.setFillColor(canAccept ? sf::Color::White : sf::Color(150, 150, 150));
            std::stringstream ss;
            ss << "Oferta od: " << contract.cityName << " (Wymaga: " << contract.energyPerSecond << " E/s, Zysk: " << contract.paymentPerSecond << "$/s)";
            itemText.setString(ss.str()); itemText.setPosition(startX, itemY);
            window.draw(itemText);
            itemY += 45.f;
        }
    }
    window.draw(m_contractListCloseButton); window.draw(m_contractListCloseText);
}

void UIManager::drawActiveContractList(sf::RenderWindow& window) {
    window.draw(m_contractListBackground);
    m_contractListTitle.setString("Aktywne Kontrakty");
    window.draw(m_contractListTitle);
    auto active = m_game.getContractManager().getActiveContracts();
    if (active.empty()) {
        m_noContractsText.setString("Brak podpisanych kontraktow");
        window.draw(m_noContractsText);
    } else {
        float startX = m_contractListBackground.getPosition().x + 20.f, itemY = m_contractListBackground.getPosition().y + 80.f;
        sf::Text itemText("", m_font, 22); itemText.setFillColor(sf::Color(180, 255, 180));
        for(const auto& contract : active) {
            std::stringstream ss;
            ss << "Dostawa dla: " << contract.cityName << " (" << contract.energyPerSecond << " E/s za " << contract.paymentPerSecond << "$/s)";
            itemText.setString(ss.str()); itemText.setPosition(startX, itemY);
            window.draw(itemText);
            itemY += 45.f;
        }
    }
    window.draw(m_contractListCloseButton); window.draw(m_contractListCloseText);
}

void UIManager::drawContractDetail(sf::RenderWindow& window) {
    const Contract* viewed = m_game.getContractManager().getContractById(m_viewedContractId);
    if (!viewed) return;
    if (viewed->status == ContractStatus::Pending) {
        bool canAccept = m_game.getNetEnergyPerSecond() >= viewed->energyPerSecond;
        if (canAccept) { m_acceptButton.setFillColor({90, 160, 90, 200}); m_acceptText.setString("Akceptuj"); }
        else { m_acceptButton.setFillColor({100, 100, 100, 200}); m_acceptText.setString("Brak mocy"); }
        m_rejectButton.setFillColor({200, 90, 90, 200});
        m_contractCityText.setString("Oferta od: " + viewed->cityName);
        m_contractDemandText.setString("Zapotrzebowanie: " + std::to_string(viewed->energyPerSecond) + " E/s");
        m_contractPaymentText.setString("Platnosc: " + std::to_string(viewed->paymentPerSecond) + "$/s");
        window.draw(m_rejectButton); window.draw(m_rejectText);
    } else if (viewed->status == ContractStatus::Active) {
        m_acceptButton.setFillColor({220, 120, 80, 200}); m_acceptText.setString("Zrezygnuj");
        m_contractCityText.setString("Dostawa dla: " + viewed->cityName);
        m_contractDemandText.setString("Zobowiazanie: " + std::to_string(viewed->energyPerSecond) + " E/s");
        m_contractPaymentText.setString("Przychód: " + std::to_string(viewed->paymentPerSecond) + "$/s");
    }
    window.draw(m_scrollSprite); window.draw(m_contractCityText); window.draw(m_contractDemandText);
    window.draw(m_contractPaymentText); window.draw(m_acceptButton); window.draw(m_acceptText);
    window.draw(m_scrollCloseButton);
}

void UIManager::drawEnergyMenu(sf::RenderWindow& window) {
    window.draw(m_panelOverlay); window.draw(m_energyPanel); window.draw(m_energyMenuTitle); window.draw(m_energyInfoText);
    window.draw(m_energyValueText); window.draw(m_sellButton); window.draw(m_sellButtonText);
    window.draw(m_closeButton); window.draw(m_closeButtonText);
}

void UIManager::drawOptionsMenu(sf::RenderWindow& window) {
    window.draw(m_panelOverlay); window.draw(m_optionsPanel); window.draw(m_optionsMenuTitle); window.draw(m_musicLabel);
    m_musicSlider->draw(window); window.draw(m_sfxLabel); m_sfxSlider->draw(window);
    window.draw(m_applyButton); window.draw(m_applyButtonText); window.draw(m_cancelButton);
    window.draw(m_cancelButtonText);
}

void UIManager::drawWeatherInfoPanel(sf::RenderWindow& window) {
    window.draw(m_panelOverlay);
    window.draw(m_weatherInfoPanel); window.draw(m_weatherInfoTitle);

    // Użycie `sf::View` do stworzenia "kamery" tylko na obszar treści, co umożliwia przewijanie.
    sf::View defaultView = window.getView();
    sf::View contentView;
    float contentAreaY = m_weatherInfoPanel.getPosition().y + 65.f;
    float contentAreaHeight = m_weatherInfoPanel.getSize().y - 125.f;
    contentView.setSize(m_weatherInfoPanel.getSize().x, contentAreaHeight);
    contentView.setCenter(m_weatherInfoPanel.getSize().x / 2.f, contentAreaHeight / 2.f);
    contentView.setViewport(sf::FloatRect(m_weatherInfoPanel.getPosition().x / window.getSize().x, contentAreaY / window.getSize().y, m_weatherInfoPanel.getSize().x / window.getSize().x, contentAreaHeight / window.getSize().y));
    window.setView(contentView);

    // Rysowanie wszystkich wpisów z uwzględnieniem przesunięcia (scroll).
    for (auto& entry : m_weatherInfoEntries) {
        sf::Vector2f namePos = entry.name.getPosition(), descPos = entry.description.getPosition();
        entry.name.setPosition(namePos.x, namePos.y - m_weatherInfoScrollOffset);
        entry.description.setPosition(descPos.x, descPos.y - m_weatherInfoScrollOffset);
        window.draw(entry.name); window.draw(entry.description);
        entry.name.setPosition(namePos); entry.description.setPosition(descPos); // Przywrócenie pozycji
    }
    window.setView(defaultView); // Przywrócenie domyślnego widoku

    // Rysowanie paska przewijania.
    float viewableRatio = contentAreaHeight / m_weatherInfoContentHeight;
    if (viewableRatio < 1.f) {
        window.draw(m_scrollbarBg);
        float handleHeight = m_scrollbarBg.getSize().y * viewableRatio;
        m_scrollbarHandle.setSize({10.f, handleHeight});
        float maxScroll = m_weatherInfoContentHeight - contentAreaHeight;
        if(maxScroll <= 0) maxScroll = 1;
        float scrollPercentage = m_weatherInfoScrollOffset / maxScroll;
        float handleY = m_scrollbarBg.getPosition().y + scrollPercentage * (m_scrollbarBg.getSize().y - handleHeight);
        m_scrollbarHandle.setPosition(m_scrollbarBg.getPosition().x, handleY);
        window.draw(m_scrollbarHandle);
    }
    window.draw(m_weatherInfoCloseButton); window.draw(m_weatherInfoCloseButtonText);
}

// Implementacja wyświetlania powiadomień.
void UIManager::showNotification(const std::string& message) {
    m_notificationText.setString(message);
    sf::FloatRect textRect = m_notificationText.getLocalBounds();
    m_notificationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_showNotification = true;
    m_notificationClock.restart();
}

// Implementacja synchronizacji suwaków.
void UIManager::syncSliderValues() {
    m_musicSlider->setValue(m_game.musicVolume);
    m_sfxSlider->setValue(m_game.sfxVolume);
}
