#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Theme.h"
#include "OptionsTheme.h"

enum class GameState {
    MainMenu,
    OptionsMenu
};

int main() {
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::RenderWindow window(
        sf::VideoMode(WIN_W, WIN_H),
        "Eco Tycoon",
        sf::Style::Titlebar | sf::Style::Close
        );

    // --- 1) Główne menu ---
    sf::Texture menuTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png")) {
        std::cerr << "Nie można wczytać eco_tycoon_menu.png\n";
        return -1;
    }
    sf::Sprite menuSprite(menuTex);
    {
        auto [w, h] = menuTex.getSize();
        menuSprite.setScale(WIN_W / float(w), WIN_H / float(h));
    }

    // Hotspoty głównego menu
    float btnW = MAIN_MENU_THEME.button.size.width;
    float btnH = MAIN_MENU_THEME.button.size.height;
    float btnX = (WIN_W - btnW) / 2.f - 10.f;
    sf::FloatRect hotNew  (btnX, 380.f, btnW, btnH);
    sf::FloatRect hotOpt  (btnX, 495.f, btnW, btnH);
    sf::FloatRect hotExit (btnX, 610.f, btnW, btnH);

    // --- 2) Opcje menu ---
    sf::Texture optsTex;
    if (!optsTex.loadFromFile(OPTIONS_THEME.backgroundImagePath)) {
        std::cerr << "Nie można wczytać " << OPTIONS_THEME.backgroundImagePath << "\n";
        return -1;
    }
    sf::Sprite optsSprite(optsTex);
    {
        auto [w, h] = optsTex.getSize();
        optsSprite.setScale(WIN_W / float(w), WIN_H / float(h));
    }
    // Hotspoty Save/Cancel w opcji
    float optBtnW = OPTIONS_THEME.saveButton.size.width;
    float optBtnH = OPTIONS_THEME.saveButton.size.height;
    float optBtnX = (WIN_W - optBtnW) / 2.f;
    sf::FloatRect hotSave  (optBtnX - 140.f, OPTIONS_THEME.saveButtonY,  optBtnW, optBtnH);
    sf::FloatRect hotCancel(optBtnX + 145.f, OPTIONS_THEME.cancelButtonY, optBtnW, optBtnH);

    GameState state = GameState::MainMenu;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f pos = window.mapPixelToCoords(
                    {e.mouseButton.x, e.mouseButton.y});

                if (state == GameState::MainMenu) {
                    if (hotNew.contains(pos)) {
                        std::cout << "Nowa gra\n";
                        // TODO: start gry
                    }
                    else if (hotOpt.contains(pos)) {
                        state = GameState::OptionsMenu;
                    }
                    else if (hotExit.contains(pos)) {
                        window.close();
                    }
                }
                else if (state == GameState::OptionsMenu) {
                    if (hotSave.contains(pos)) {
                        std::cout << "Zapisano zmiany\n";
                        state = GameState::MainMenu;
                    }
                    else if (hotCancel.contains(pos)) {
                        std::cout << "Anulowano zmiany\n";
                        state = GameState::MainMenu;
                    }
                }
            }
        }

        window.clear();
        if (state == GameState::MainMenu) {
            window.draw(menuSprite);
        }
        else {  // OptionsMenu
            window.draw(optsSprite);
            // tutaj dodasz ręcznie suwaki pod etykietami z tła
        }

        // (opcjonalnie) rysowanie ramki dla debugowania
        /*sf::RectangleShape debugRect;
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineThickness(2.f);*/

        // odkomentuj, by zobaczyć hotspoty:

        /*debugRect.setPosition(hotSave.left, hotSave.top);
        debugRect.setSize({hotSave.width, hotSave.height});
        window.draw(debugRect);
        debugRect.setPosition(hotCancel.left, hotCancel.top);
        debugRect.setSize({hotCancel.width, hotCancel.height});
        window.draw(debugRect);*/

        /*debugRect.setPosition(hotNew.left, hotNew.top);
        debugRect.setSize({hotNew.width, hotNew.height});
        window.draw(debugRect);
        debugRect.setPosition(hotOpt.left, hotOpt.top);
        debugRect.setSize({hotOpt.width, hotOpt.height});
        window.draw(debugRect);
        debugRect.setPosition(hotExit.left, hotExit.top);
        debugRect.setSize({hotExit.width, hotExit.height});
        window.draw(debugRect);*/

        window.display();
    }


    return 0;
}
