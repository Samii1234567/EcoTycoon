#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Theme.h"
#include "OptionsTheme.h"
#include "Slider.h"
#include "GameBackground.h"
#include "BuildMenu.h"
#include "SaveManager.h"

enum class GameState {
    MainMenu,
    EnterSaveName,  // NOWY: wpisywanie nazwy przy „Nowa gra”
    LoadMenu,       // wybór zapisu do wczytania
    OptionsMenu,
    Playing,
};

int main() {
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::RenderWindow window(
        sf::VideoMode(WIN_W, WIN_H),
        "Eco Tycoon",
        sf::Style::Titlebar | sf::Style::Close
        );
    window.setVerticalSyncEnabled(true);

    // --- Czcionka ---
    sf::Font gameFont;
    if (!gameFont.loadFromFile("images/Quicksand-VariableFont_wght.ttf")) {
        std::cerr << "Nie można wczytać czcionki\n";
        return -1;
    }

    // --- Zasoby gry ---
    float currentMoney      = 1000.f;
    float currentEnergy     = 0.f;
    float maxEnergy         = 100.f;
    float environmentHealth = 100.f;

    // --- 1) Menu główne ---
    sf::Texture menuTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png")) {
        std::cerr << "Błąd: nie można wczytać eco_tycoon_menu.png\n";
        return -1;
    }
    sf::Sprite menuSprite(menuTex);
    {
        auto [mw, mh] = menuTex.getSize();
        menuSprite.setScale(WIN_W / float(mw), WIN_H / float(mh));
    }

    float btnW = MAIN_MENU_THEME.button.size.width;
    float btnH = MAIN_MENU_THEME.button.size.height;
    float btnX = (WIN_W - btnW) / 2.f - 10.f;
    sf::FloatRect hotNew  (btnX, 353.f, btnW + 18.f, btnH);
    sf::FloatRect hotLoad (btnX, 455.f, btnW + 18.f, btnH);
    sf::FloatRect hotOpt  (btnX, 560.f, btnW + 18.f, btnH);
    sf::FloatRect hotExit (btnX, 660.f, btnW + 18.f, btnH);

    // --- 2) Menu opcji ---
    sf::Texture optsTex;
    optsTex.loadFromFile(OPTIONS_THEME.backgroundImagePath);
    sf::Sprite optsSprite(optsTex);
    {
        auto [ow, oh] = optsTex.getSize();
        optsSprite.setScale(WIN_W / float(ow), WIN_H / float(oh));
    }
    float optBW = OPTIONS_THEME.saveButton.size.width;
    float optBH = OPTIONS_THEME.saveButton.size.height;
    float optX  = (WIN_W - optBW) / 2.f;
    sf::FloatRect hotSave   (optX - 140.f, OPTIONS_THEME.saveButtonY,  optBW, optBH);
    sf::FloatRect hotCancel (optX + 140.f, OPTIONS_THEME.cancelButtonY, optBW, optBH);

    Slider musicSlider(
        (WIN_W - OPTIONS_THEME.sliderWidth) / 2.f,
        OPTIONS_THEME.sliderMusicY,
        OPTIONS_THEME.sliderWidth,
        OPTIONS_THEME.sliderHeight,
        OPTIONS_THEME.thumbRadius
        );
    Slider sfxSlider(
        (WIN_W - OPTIONS_THEME.sliderWidth) / 2.f,
        OPTIONS_THEME.sliderSfxY,
        OPTIONS_THEME.sliderWidth,
        OPTIONS_THEME.sliderHeight,
        OPTIONS_THEME.thumbRadius
        );
    /*float backupMusic = musicSlider.getValue();
    float backupSfx   = sfxSlider.getValue();*/

    // --- 3) Tło gry ---
    GameBackground gameBg;
    gameBg.loadFromFile("images/eco_tycoon_game.png");

    // --- 4) BuildMenu ---
    BuildMenu buildMenu;
    buildMenu.initialize(gameFont);
    bool hammerPressed = false;
    sf::FloatRect hammerHotspot(14.f, 18.f, 90.f, 90.f);

    // --- 5) Napisy dolnego paska ---
    sf::Color brown(101, 67, 33);
    sf::Text labelMoney("Stan konta", gameFont, 24);
    labelMoney.setFillColor(brown);
    labelMoney.setPosition(77.f, 726.f);

    sf::Text labelEnergy("Energia", gameFont, 24);
    labelEnergy.setFillColor(brown);
    labelEnergy.setPosition(390.f, 726.f);

    sf::Text labelEnvironment("Stan srodowiska", gameFont, 24);
    labelEnvironment.setFillColor(brown);
    labelEnvironment.setPosition(654.f, 726.f);

    sf::Text labelTime("Czas gry:", gameFont, 30);
    labelTime.setFillColor(brown);
    labelTime.setPosition(980.f, 744.f);

    sf::Text moneyValue(std::to_string(currentMoney) + "$", gameFont, 24);
    moneyValue.setFillColor(brown);
    moneyValue.setPosition(77.f, 752.f);

    sf::Text energyValue(std::to_string(currentEnergy) + "/" + std::to_string(maxEnergy), gameFont, 24);
    energyValue.setFillColor(brown);
    energyValue.setPosition(390.f, 752.f);

    sf::Text envValue(std::to_string(environmentHealth) + "%", gameFont, 24);
    envValue.setFillColor(brown);
    envValue.setPosition(654.f, 752.f);

    // --- 6) Wpisywanie nazwy nowego zapisu ---
    std::string saveName;
    sf::Text inputPrompt("Nazwa zapisu:", gameFont, 24);
    inputPrompt.setFillColor(brown);
    inputPrompt.setPosition(350.f, 300.f);
    sf::Text inputText("", gameFont, 24);
    inputText.setFillColor(brown);
    inputText.setPosition(350.f, 350.f);

    // --- 7) Ekran wyboru zapisu ---
    std::vector<std::string> saves;
    sf::Text loadListTitle("Wybierz zapis:", gameFont, 24);
    loadListTitle.setFillColor(brown);
    loadListTitle.setPosition(350.f, 280.f);

    // --- 8) Stan gry i zegary ---
    GameState state = GameState::MainMenu;
    sf::Clock gameClock;
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            // Zamknięcie
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            // TekstEntered dla wpisywania nazwy
            if (state == GameState::EnterSaveName && e.type == sf::Event::TextEntered) {
                if (e.text.unicode == '\r' || e.text.unicode == '\n') {
                    // Enter → zapis
                    if (!saveName.empty()) {
                        SaveManager::saveGame(saveName,
                                              currentMoney,
                                              currentEnergy,
                                              maxEnergy,
                                              environmentHealth);
                    }
                    state = GameState::Playing;
                }
                else if (e.text.unicode == 8) { // Backspace
                    if (!saveName.empty()) saveName.pop_back();
                }
                else if (e.text.unicode < 128) {
                    saveName.push_back(static_cast<char>(e.text.unicode));
                }
                inputText.setString(saveName);
            }

            // Kliknięcia w menu…
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

                if (state == GameState::MainMenu) {
                    if (hotNew.contains(pos)) {
                        // Nowa gra → wpisujemy nazwę
                        saveName.clear();
                        inputText.setString("");
                        state = GameState::EnterSaveName;
                    }
                    else if (hotLoad.contains(pos)) {
                        // Wczytaj grę → pobieramy listę i przechodzimy
                        saves = SaveManager::listSaves();
                        state = GameState::LoadMenu;
                    }
                    else if (hotOpt.contains(pos)) {
                        state = GameState::OptionsMenu;
                    }
                    else if (hotExit.contains(pos)) {
                        window.close();
                    }
                }
                else if (state == GameState::LoadMenu) {
                    // Klik w któryś zapis
                    for (size_t i = 0; i < saves.size(); ++i) {
                        float y0 = 330.f + float(i)*30.f;
                        sf::FloatRect r(350.f, y0, 500.f, 28.f);
                        if (r.contains(pos)) {
                            SaveManager::loadGame(saves[i],
                                                  currentMoney,
                                                  currentEnergy,
                                                  maxEnergy,
                                                  environmentHealth);
                            state = GameState::Playing;
                        }
                    }
                }
                else if (state == GameState::OptionsMenu) {
                    if (hotSave.contains(pos)) {
                        // … jak wcześniej …
                        state = GameState::MainMenu;
                    }
                    else if (hotCancel.contains(pos)) {
                        // … jak wcześniej …
                        state = GameState::MainMenu;
                    }
                }
                else if (state == GameState::Playing) {
                    if (hammerHotspot.contains(pos)) {
                        hammerPressed = !hammerPressed;
                        buildMenu.setVisible(hammerPressed);
                    }
                    if (buildMenu.isVisible()) {
                        buildMenu.handleEvent(e, window, *(float*)&currentMoney);
                    }
                }
            }

            // suwaki w opcjach
            if (state == GameState::OptionsMenu) {
                musicSlider.handleEvent(e, window);
                sfxSlider.handleEvent(e, window);
            }
        }

        // Aktualizacje
        if (state == GameState::Playing) {
            // czas gry
            sf::Time elapsed = gameClock.getElapsedTime();
            int min = elapsed.asSeconds() / 60;
            int sec = int(elapsed.asSeconds()) % 60;
            std::ostringstream oss;
            oss << min<<":"<<std::setw(2)<<std::setfill('0')<<sec;
            labelTime.setString("Czas gry: "+oss.str());
            // aktualizacja wartości
            moneyValue.setString(std::to_string(currentMoney)+"$");
            energyValue.setString(std::to_string(currentEnergy)+"/"+std::to_string(maxEnergy));
            envValue.setString(std::to_string(environmentHealth)+"%");

        }

        // Rysowanie
        window.clear();

        if (state == GameState::MainMenu) {
            window.draw(menuSprite);
        }
        else if (state == GameState::EnterSaveName) {
            // ramka i input
            sf::RectangleShape box({600.f,200.f});
            box.setFillColor({200,200,200,220});
            box.setPosition(300.f, 280.f);
            window.draw(box);
            window.draw(inputPrompt);
            window.draw(inputText);
        }
        else if (state == GameState::LoadMenu) {
            // ramka + lista
            sf::RectangleShape box({600.f,400.f});
            box.setFillColor({200,200,200,220});
            box.setPosition(300.f, 240.f);
            window.draw(box);
            window.draw(loadListTitle);
            for (size_t i=0; i<saves.size(); ++i) {
                sf::Text t(saves[i], gameFont, 24);
                t.setFillColor(brown);
                t.setPosition(350.f, 330.f+float(i)*30.f);
                window.draw(t);
            }
        }
        else if (state == GameState::OptionsMenu) {
            window.draw(optsSprite);
            musicSlider.draw(window);
            sfxSlider.draw(window);
        }
        else if (state == GameState::Playing) {
            gameBg.draw(window);
            window.draw(labelMoney);
            window.draw(labelEnergy);
            window.draw(labelEnvironment);
            window.draw(labelTime);
            window.draw(moneyValue);
            window.draw(energyValue);
            window.draw(envValue);
            if (hammerPressed) buildMenu.draw(window);
        }
        // (opcjonalnie) rysowanie ramki dla debugowania
        sf::RectangleShape debugRect;
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineThickness(2.f);

        // odkomentuj, by zobaczyć hotspoty:

        /*debugRect.setPosition(hotSave.left, hotSave.top);
        debugRect.setSize({hotSave.width, hotSave.height});
        window.draw(debugRect);
        debugRect.setPosition(hotCancel.left, hotCancel.top);
        debugRect.setSize({hotCancel.width, hotCancel.height});
        window.draw(debugRect);*/

        debugRect.setPosition(hotNew.left, hotNew.top);
        debugRect.setSize({hotNew.width, hotNew.height});
        window.draw(debugRect);
        debugRect.setPosition(hotOpt.left, hotOpt.top);
        debugRect.setSize({hotOpt.width, hotOpt.height});
        window.draw(debugRect);
        debugRect.setPosition(hotExit.left, hotExit.top);
        debugRect.setSize({hotExit.width, hotExit.height});
        window.draw(debugRect);
        debugRect.setPosition(hotLoad.left, hotLoad.top);
        debugRect.setSize({hotLoad.width, hotLoad.height});
        window.draw(debugRect);

        /*debugRect.setPosition(hammerHotspot.left, hammerHotspot.top);
        debugRect.setSize({hammerHotspot.width, hammerHotspot.height});
        window.draw(debugRect);*/

        window.display();
    }

    return 0;
}
