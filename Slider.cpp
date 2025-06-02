#include "Slider.h"
#include <algorithm>

Slider::Slider(float sliderX, float sliderY, float width, float height, float thumbRadius)
    : _x(sliderX), _y(sliderY), _w(width)
{
    // przygotuj tor
    _track.setSize({width, height});
    _track.setPosition(sliderX, sliderY);
    _track.setFillColor({155,182,109});

    // przygotuj thumb
    _thumb.setRadius(thumbRadius);
    _thumb.setOrigin(thumbRadius, thumbRadius);
    _thumb.setPosition(sliderX + width/2, sliderY + height/2);
    _thumb.setFillColor({0,136,78});
}

void Slider::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        auto p = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
        if (_thumb.getGlobalBounds().contains(p)) _drag = true;
    }
    if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
        _drag = false;
    }
    if (_drag && e.type == sf::Event::MouseMoved) {
        float mx = window.mapPixelToCoords({e.mouseMove.x, e.mouseMove.y}).x;
        mx = std::clamp(mx, _x, _x + _w);
        _thumb.setPosition(mx, _y + _track.getSize().y/2);
    }
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(_track);
    window.draw(_thumb);
}

float Slider::getValue() const {
    float mx = _thumb.getPosition().x;
    return (mx - _x) / _w;
}
