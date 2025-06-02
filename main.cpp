#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Theme.h"

int main() {
    const unsigned WIN_W = 1200, WIN_H = 800;
    sf::RenderWindow window(
        sf::VideoMode(WIN_W, WIN_H),
        "Eco Tycoon - Main Menu",
        sf::Style::Titlebar | sf::Style::Close
        );

    sf::Texture menuTex;
    if (!menuTex.loadFromFile("images/eco_tycoon_menu.png")) {
        std::cerr << "Błąd: nie można wczytać eco_tycoon_menu.png\n";
        return -1;
    }
    sf::Sprite menuSprite(menuTex);

    // 3. Skalowanie sprite’a, by wypełnił okno
    {
        auto [texW, texH] = menuTex.getSize();
        float scaleX = WIN_W / float(texW);
        float scaleY = WIN_H / float(texH);
        menuSprite.setScale(scaleX, scaleY);
    }

    //koordynaty hotspotów:
    float btnW = MAIN_MENU_THEME.button.size.width;
    float btnH = MAIN_MENU_THEME.button.size.height;
    float btnX = (WIN_W - btnW) / 2.f - 10;

    sf::FloatRect hotNew  (btnX, 380.f, btnW, btnH);
    sf::FloatRect hotOpt  (btnX, 495.f, btnW, btnH);
    sf::FloatRect hotExit (btnX, 610.f, btnW, btnH);

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

                if (hotNew.contains(pos)) {
                    std::cout << "Nowa gra kliknięta\n";
                    window.close();
                }
                else if (hotOpt.contains(pos)) {
                    std::cout << "Opcje kliknięte\n";
                    window.close();
                }
                else if (hotExit.contains(pos)) {
                    std::cout << "Wyjście kliknięte\n";
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(menuSprite);


        //ramki dla debugowania hotspotów
        /*sf::RectangleShape debugRect;
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineThickness(2.f);*/

        // koordynaty hotspotów:
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
