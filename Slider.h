#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

class Slider {
public:
    // sliderX, sliderY – pozycja lewego końca toru
    // width, height – wymiary toru
    // thumbRadius – promień kółka
    Slider(float sliderX, float sliderY, float width, float height, float thumbRadius);

    // obsłuż zdarzenia myszki (klik, drag)
    void handleEvent(const sf::Event& e, const sf::RenderWindow& window);

    // rysuj tor i kółko
    void draw(sf::RenderWindow& window) const;

    // pobierz wartość w [0,1]
    float getValue() const;

private:
    sf::RectangleShape _track;
    sf::CircleShape    _thumb;
    float              _x, _y, _w;
    bool               _drag = false;
};

#endif // SLIDER_H
