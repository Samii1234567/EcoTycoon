#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Theme.h"
#include "OptionsTheme.h"
#include "Slider.cpp"
#include "GameBackground.h"

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

    // Czcionka do napisów
    sf::Font gameFont;
    if (!gameFont.loadFromFile("images/Quicksand-VariableFont_wght.ttf")) {
        std::cerr << "Nie można wczytać czcionki LuckiestGuy-Regular.ttf\n";
        return -1;
    }

    // --- 1) Menu główne ---
    sf::Texture menuTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png")) {
        std::cerr << "Nie można wczytać eco_tycoon_menu.png\n";
        return -1;
    }
    sf::Sprite menuSprite(menuTex);
    {
        auto [mw, mh] = menuTex.getSize();
        menuSprite.setScale(WIN_W / float(mw), WIN_H / float(mh));
    }

    float btnW = MAIN_MENU_THEME.button.size.width;
    float btnH = MAIN_MENU_THEME.button.size.height;
    float btnX = (WIN_W - btnW) / 2.f;
    sf::FloatRect hotNew  (btnX, 380.f, btnW, btnH);
    sf::FloatRect hotOpt  (btnX, 495.f, btnW, btnH);
    sf::FloatRect hotExit (btnX, 610.f, btnW, btnH);

    // --- 2) Menu opcji ---
    sf::Texture optsTex;
    if (!optsTex.loadFromFile(OPTIONS_THEME.backgroundImagePath)) {
        std::cerr << "Nie można wczytać " << OPTIONS_THEME.backgroundImagePath << "\n";
        return -1;
    }
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
        std::cerr << "Nie można wczytać assets/ui_background_final3.png\n";
        return -1;
    }

    // --- 5) Przygotowanie napisów gry ---
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

    // --- 6) Stan gry ---
    GameState state = GameState::MainMenu;

    sf::Clock gameClock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f pos = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
                if (state == GameState::MainMenu) {
                    if (hotNew.contains(pos)) {
                        state = GameState::Playing;
                        gameClock.restart();
                    }
                    else if (hotOpt.contains(pos)) {
                        backupMusic = musicSlider.getValue();
                        backupSfx = sfxSlider.getValue();
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
            }

            if (state == GameState::OptionsMenu) {
                musicSlider.handleEvent(e, window);
                sfxSlider.handleEvent(e, window);
            }
        }

        if (state == GameState::Playing) {
            sf::Time elapsed = gameClock.getElapsedTime();
            int minutes = elapsed.asSeconds() / 60;
            int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

            std::ostringstream oss;
            oss << "Czas gry: " << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
            labelTime.setString(oss.str());
        }

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
            gameBg.draw(window);
            window.draw(labelMoney);
            window.draw(labelEnergy);
            window.draw(labelEnvironment);
            window.draw(labelTime);

        }

        window.display();
    }
    return 0;
}
