#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Theme.h"
#include "OptionsTheme.h"
#include "Slider.cpp"        // Pozostawione tak, jak masz w projekcie
#include "GameBackground.h"
#include "BuildMenu.h"       // Nowy nagłówek

enum class GameState {
    MainMenu,
    OptionsMenu,
    Playing
};

int main() {
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::RenderWindow window(
        sf::VideoMode(WIN_W, WIN_H),
        "Eco Tycoon",
        sf::Style::Titlebar | sf::Style::Close
        );
    window.setVerticalSyncEnabled(true);

    // 1) Załaduj czcionkę Comfortaa (używana w BuildMenu i etykietach)
    sf::Font gameFont;
    if (!gameFont.loadFromFile("images/Quicksand-VariableFont_wght.ttf")) {
        std::cerr << "Nie można wczytać czcionki Comfortaa-Regular.ttf\n";
        return -1;
    }

    // --- 1) Menu główne ---
    sf::Texture menuTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png")) {
        std::cerr << "Błąd: nie można wczytać eco_tycoon_menu.png\n";
        return -1;
    }
    sf::Sprite menuSprite(menuTex);
    {
        auto [mw, mh] = menuTex.getSize();
        float scaleX = WIN_W / float(mw);
        float scaleY = WIN_H / float(mh);
        menuSprite.setScale(scaleX, scaleY);
    }

    float btnW = MAIN_MENU_THEME.button.size.width;
    float btnH = MAIN_MENU_THEME.button.size.height;
    float btnX = (WIN_W - btnW) / 2.f - 10.f;
    sf::FloatRect hotNew (btnX, 380.f, btnW, btnH);
    sf::FloatRect hotOpt (btnX, 495.f, btnW, btnH);
    sf::FloatRect hotExit(btnX, 610.f, btnW, btnH);

    // --- 2) Menu opcji ---
    sf::Texture optsTex;
    if (!optsTex.loadFromFile(OPTIONS_THEME.backgroundImagePath)) {
        std::cerr << "Błąd: nie można wczytać " << OPTIONS_THEME.backgroundImagePath << "\n";
        return -1;
    }
    sf::Sprite optsSprite(optsTex);
    {
        auto [ow, oh] = optsTex.getSize();
        float scaleX = WIN_W / float(ow);
        float scaleY = WIN_H / float(oh);
        optsSprite.setScale(scaleX, scaleY);
    }

    float optBW = OPTIONS_THEME.saveButton.size.width;
    float optBH = OPTIONS_THEME.saveButton.size.height;
    float optX  = (WIN_W - optBW) / 2.f;
    sf::FloatRect hotSave   (optX - 140.f, OPTIONS_THEME.saveButtonY,  optBW, optBH);
    sf::FloatRect hotCancel (optX + 140.f, OPTIONS_THEME.cancelButtonY, optBW, optBH);

    // --- 3) Inicjalizacja sliderów ---
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
    float backupMusic = musicSlider.getValue();
    float backupSfx   = sfxSlider.getValue();

    // --- 4) Wczytanie tła gry ---
    GameBackground gameBg;
    if (!gameBg.loadFromFile("images/eco_tycoon_game.png")) {
        std::cerr << "Błąd: nie można wczytać images/eco_tycoon_game.png\n";
        return -1;
    }

    // --- 5) BuildMenu ---
    BuildMenu buildMenu;
    if (!buildMenu.initialize(gameFont)) {
        std::cerr << "Nie można zainicjować BuildMenu\n";
        return -1;
    }
    float money = 1000.f;        // stan konta gracza
    bool  hammerPressed = false; // flaga widoczności BuildMenu

    sf::FloatRect hammerHotspot(14.f, 18.f, 90.f, 90.f);

    // --- 6) Przygotowanie napisów gry ---
    sf::Color brown(101, 67, 33); // Brązowy kolor

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
    labelTime.setPosition(950.f, 742.f);

    // --- 8) Stan gry ---
    GameState state = GameState::MainMenu;
    sf::Clock gameClock;   // odmierzanie czasu w stanie Playing
    sf::Clock deltaClock;  // do update'owania BuildMenu

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            // Obsługa kliknięcia lewym przyciskiem myszy
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});

                if (state == GameState::MainMenu) {
                    if (hotNew.contains(pos)) {
                        state = GameState::Playing;
                        gameClock.restart();
                        deltaClock.restart();
                    }
                    else if (hotOpt.contains(pos)) {
                        backupMusic = musicSlider.getValue();
                        backupSfx   = sfxSlider.getValue();
                        state = GameState::OptionsMenu;
                    }
                    else if (hotExit.contains(pos)) {
                        window.close();
                    }
                }
                else if (state == GameState::OptionsMenu) {
                    if (hotSave.contains(pos)) {
                        float volMusic = musicSlider.getValue();
                        float volSfx   = sfxSlider.getValue();
                        std::cout << "Zapisano: muzyka=" << volMusic << " sfx=" << volSfx << "\n";
                        state = GameState::MainMenu;
                    }
                    else if (hotCancel.contains(pos)) {
                        musicSlider.setValue(backupMusic);
                        sfxSlider.setValue(backupSfx);
                        std::cout << "Anulowano zmiany\n";
                        state = GameState::MainMenu;
                    }
                }
                else if (state == GameState::Playing) {
                    // 7.1 Klik w młotek: wyświetlenie/ukrycie BuildMenu
                    if (hammerHotspot.contains(pos)) {
                        hammerPressed = !hammerPressed;
                        buildMenu.setVisible(hammerPressed);
                    }
                    // (pozostałe kliknięcia w Playing rutynowo ignorujemy tutaj)
                }

                // 7.2 Przekażemy kliknięcie do BuildMenu, jeśli jest widoczne
                if (buildMenu.isVisible()) {
                    buildMenu.handleEvent(e, window, money);
                }
            }

            // Obsługa sliderów w OptionsMenu (jeśli używasz)
            if (state == GameState::OptionsMenu) {
                musicSlider.handleEvent(e, window);
                sfxSlider.handleEvent(e, window);
            }
        }

        // Aktualizacja stanu gry
        if (state == GameState::Playing) {
            // 8.1 Aktualizacja zegara i etykiety czasu
            sf::Time elapsed = gameClock.getElapsedTime();
            int minutes = elapsed.asSeconds() / 60;
            int seconds = static_cast<int>(elapsed.asSeconds()) % 60;
            std::ostringstream oss;
            oss << "Czas: " << minutes << ":"
                << std::setw(2) << std::setfill('0') << seconds;
            labelTime.setString(oss.str());

            // 8.2 Podnoszenie cen w BuildMenu (co delta)
            float delta = deltaClock.restart().asSeconds();
            buildMenu.update(delta);
        }

        // Rysowanie
        window.clear();

        if (state == GameState::MainMenu) {
            window.draw(menuSprite);
        }
        else if (state == GameState::OptionsMenu) {
            window.draw(optsSprite);
            musicSlider.draw(window);
            sfxSlider.draw(window);
        }
        else if (state == GameState::Playing) {
            // 9.1 Rysujemy tło (pełne 1200×800, z górnym i dolnym paskiem)
            gameBg.draw(window);

            // 9.2 Rysujemy dolne etykiety
            window.draw(labelMoney);
            window.draw(labelEnergy);
            window.draw(labelEnvironment);
            window.draw(labelTime);

            // 9.3 Rysujemy BuildMenu, jeśli młotek wciśnięty
            if (buildMenu.isVisible()) {
                buildMenu.draw(window);
            }
        }

        // (opcjonalnie) rysowanie ramki dla debugowania
        sf::RectangleShape debugRect;
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineThickness(2.f);

        // odkomentuj, by zobaczyć hotspoty:

        debugRect.setPosition(hotSave.left, hotSave.top);
        debugRect.setSize({hotSave.width, hotSave.height});
        window.draw(debugRect);
        debugRect.setPosition(hotCancel.left, hotCancel.top);
        debugRect.setSize({hotCancel.width, hotCancel.height});
        window.draw(debugRect);

        debugRect.setPosition(hotNew.left, hotNew.top);
        debugRect.setSize({hotNew.width, hotNew.height});
        window.draw(debugRect);
        debugRect.setPosition(hotOpt.left, hotOpt.top);
        debugRect.setSize({hotOpt.width, hotOpt.height});
        window.draw(debugRect);
        debugRect.setPosition(hotExit.left, hotExit.top);
        debugRect.setSize({hotExit.width, hotExit.height});
        window.draw(debugRect);

        debugRect.setPosition(hammerHotspot.left, hammerHotspot.top);
        debugRect.setSize({hammerHotspot.width, hammerHotspot.height});
        window.draw(debugRect);

        window.display();
    }

    return 0;
}
